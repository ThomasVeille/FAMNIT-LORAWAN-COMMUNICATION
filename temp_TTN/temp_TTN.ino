#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// ---------- Fonctions RAK3172 ----------

String rak3172_sendCmd(const String& cmd, unsigned long timeout_ms = 3000) {
  while (Serial2.available()) Serial2.read();

  Serial2.println(cmd);
  String response = "";
  unsigned long start = millis();
  while (millis() - start < timeout_ms) {
    while (Serial2.available()) {
      char c = Serial2.read();
      Serial.write(c);
      response += c;
    }
    if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
      delay(100);
      while (Serial2.available()) { char c = Serial2.read(); Serial.write(c); response += c; }
      break;
    }
  }
  return response;
}

bool rak3172_waitFor(const String& token, unsigned long timeout_ms) {
  unsigned long start = millis();
  String buffer = "";
  while (millis() - start < timeout_ms) {
    while (Serial2.available()) {
      char c = Serial2.read();
      Serial.write(c);
      buffer += c;
      if (buffer.indexOf(token) != -1) return true;
    }
  }
  return false;
}

String textToHex(const String& text) {
  String hex = "";
  for (unsigned int i = 0; i < text.length(); i++) {
    char buf[3];
    sprintf(buf, "%02X", (unsigned char)text[i]);
    hex += buf;
  }
  return hex;
}

// ---------- Envoi de la temperature ----------

void sendTemperature(float temperature) {
  String value = String(temperature, 2); // ex: "23.45"
  String hex = textToHex(value);
  Serial.println(">> Envoi temperature : " + value + " (" + hex + ")");
  rak3172_sendCmd("AT+SEND=1:" + hex, 5000);
}

// ---------- Setup / Loop ----------

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  delay(2500);

  Serial.println("Test UART RAK3172");
  rak3172_sendCmd("AT");

  String adr = rak3172_sendCmd("AT+ADR=1");
  Serial.println("AT+ADR=1 -> " + adr);

  String njs = rak3172_sendCmd("AT+NJS=?");
  bool alreadyJoined = (njs.indexOf("1") != -1 && njs.indexOf("OK") != -1);

  if (!alreadyJoined) {
    Serial.println(">> Pas encore connecte, lancement du join...");
    Serial2.println("AT+JOIN");
    bool joined = rak3172_waitFor("+EVT:JOINED", 30000);
    if (!joined) Serial.println(">> ECHEC du join");
  } else {
    Serial.println(">> Deja connecte au reseau");
  }

  rak3172_sendCmd("AT+CFM=0");

  // Init capteur MPU-6050 (I2C : SDA=21, SCL=22)
  Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sendTemperature(temp.temperature);

  delay(180000); // on garde 60s, coherent avec le duty cycle observe
}
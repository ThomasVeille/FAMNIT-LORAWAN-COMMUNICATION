#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  //Module RAKK3172 initialization (trnasmitter)
  Serial.begin(115200);      
  Serial1.begin(115200);     

  delay(1000);
  Serial.println("RAK3172 initialization (transmitter)");

  //Configuration command for RAK3172
  sendAT("AT");
  sendAT("AT+NWM=0");   // P2P Mode
  sendAT("AT+PRECV=0");  // Disable reception
  sendAT("AT+P2P=868000000:7:125:0:10:14");  // P2P configuration
  sendAT("AT+PRECV=65535");  // Enable reception


  Serial.println("RAK3172 ready (transmitter)");

  //Captor MPU-6050 

    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  String value;
  value = String(temp.temperature);
  value = value + "deg";

  sendMessage(value);
  delay(5000);

}

// ---------------------- //

// Send AT command fonction
void sendAT(String cmd) {
  Serial1.println(cmd);
  delay(200);
  // while (Serial1.available()) {
  //   Serial.println("[RAK] " + Serial1.readStringUntil('\n'));
  // }
}

// Send Serial Monitor message fonction
void sendMessage(String text) {
  // HEX conversion
  String hex = "";

  for (int i = 0; i < text.length(); i++) {
    unsigned char c = text[i];

    // edit ° to deg
    if (c == 0xC2 && i + 1 < text.length()) {
      unsigned char next = text[i + 1];

      if (next == 0xB0) {   // ° code 
        hex += "646567";    // "deg" -- HEX
        i++;                
        continue;
      }

      if (next == 0xB5) {   // µ code
        hex += "75";        // "u" -- HEX
        i++;
        continue;
      }
    }

    // HEX conversion
    char buf[3];
    sprintf(buf, "%02X", c);
    hex += buf;
  }

  Serial.println("Message sent : " + text + " (" + hex + ")");
    
  sendAT("AT+PRECV=0"); // Disable reception
  Serial1.println("AT+PSEND=" + hex); // Send message
  delay(200);
  sendAT("AT+PRECV=65535"); // Enable reception
}
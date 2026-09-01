#include <HardwareSerial.h>

HardwareSerial NoiseSensor(1); // UART1


uint16_t modbusCRC16(uint8_t* buf, int len) {
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];
    for (int i = 0; i < 8; i++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}


float readNoiseLevel() {
  
  uint8_t query[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
  uint16_t crc = modbusCRC16(query, 6);
  query[6] = crc & 0xFF;        
  query[7] = (crc >> 8) & 0xFF; 

  
  while (NoiseSensor.available()) NoiseSensor.read();

  NoiseSensor.write(query, 8);

  
  unsigned long start = millis();
  uint8_t response[7];
  int idx = 0;
  while (millis() - start < 500 && idx < 7) {
    if (NoiseSensor.available()) {
      response[idx++] = NoiseSensor.read();
    }
  }

  if (idx < 7) {
    Serial.println(">> Answer incorrect timeout");
    return -1;
  }

  
  if (response[0] != 0x01 || response[1] != 0x03) {
    Serial.println(">> Answer incorrect");
    return -1;
  }

  uint16_t rawValue = (response[3] << 8) | response[4]; 
  float noiseDb = rawValue / 10.0;
  return noiseDb;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  NoiseSensor.begin(9600, SERIAL_8N1, 25, 26); // RX=25, TX=26
  delay(500);

  Serial.println("Noise sensor RS-ZS-BZ-TTL-05");
}

void loop() {
  float db = readNoiseLevel();
  if (db >= 0) {
    Serial.println("Microphone RS-ZS-BZ : " + String(db, 1) + " dB");
  }
  delay(1000);
}
// Arduino UNO R4 WiFi + RAK3172 LoRa P2P
// Receiver part

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  delay(1000);
  Serial.println("RAK3172 initialization (receiver)");

  //Configuration command for RAK3172
  sendAT("AT");
  sendAT("AT+NWM=0");   // P2P Mode
  sendAT("AT+PRECV=0");  // Disable reception
  sendAT("AT+P2P=868000000:7:125:0:10:14");  // P2P configuration
  sendAT("AT+PRECV=65535");  // Enable reception

  Serial.println("Receiver LoRa ready");
}

void loop() {
  // Read ALL available ligne
  while (Serial1.available()) {
    String line = Serial1.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      // Serial.println("[RX] " + line);
      int pos = line.lastIndexOf(':');
      String hexamessage = line.substring(pos+1,line.length());
      String ascii = convertmessage(hexamessage);
      Serial.println("Message received : " + ascii);

      // If Lora message received, enable reception
      if (line.startsWith("+EVT:RXP2P")) {
        sendAT("AT+PRECV=65535");
      }
    }
  }
}

// ---------------------- // 

// Send AT command fonction
void sendAT(String cmd) {
  Serial1.println(cmd);
  delay(200);
  while (Serial1.available()) {
    Serial.println("[RAK] " + Serial1.readStringUntil('\n'));
  }
}

String convertmessage(String hexmessage){
  String message = "";
  for (int i = 0; i < hexmessage.length(); i+=2) {
    String hex = hexmessage.substring(i,i+2);
    int value = strtol(hex.c_str(), NULL, 16);
    char c = (char)value;
    message +=c;

  }

  return message;
}

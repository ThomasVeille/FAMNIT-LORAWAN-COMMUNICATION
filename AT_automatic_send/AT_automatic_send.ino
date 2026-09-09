#include <HardwareSerial.h>

HardwareSerial RAK(2);

// OTAA KEYS (paste on TTN). Maybe need to be change with the device created
const char* DEVEUI = "70B3D57ED00788CD";                   
const char* APPEUI = "1209632526987120";                  
const char* APPKEY = "01ADEA15B24E142D4C4EFAE25EE524E6"; 


const unsigned long TX_INTERVAL = 60000UL; 
const int LORA_PORT = 2;                   

String rxLine = "";
bool joined = false;
unsigned long lastSend = 0;

bool sendAT(const String& cmd, unsigned long timeout = 3000) {
    while (RAK.available()) RAK.read(); 

    Serial.print(F(">> "));
    Serial.println(cmd);
    RAK.print(cmd);
    RAK.print("\r\n");

    String response = "";
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while (RAK.available()) {
            response += (char) RAK.read();
        }
        if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
            break;
        }
    }
    response.trim();

    if (response.length() > 0) {
        Serial.print(F("<< "));
        Serial.println(response);
    } else {
        Serial.println(F("<< (No answer : timeout, check the power/wiring)"));
    }

    return response.indexOf("OK") != -1;
}

String bytesToHex(uint8_t* data, size_t len) {
    String hex = "";
    for (size_t i = 0; i < len; i++) {
        if (data[i] < 0x10) hex += "0";
        hex += String(data[i], HEX);
    }
    hex.toUpperCase();
    return hex;
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println(F("=== ESP32 <-> RAK3172 (AT) -> TTN, OTAA EU868 ==="));

    RAK.begin(115200, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17
    delay(500);

    Serial.println(F("Communication Test"));
    if (!sendAT("AT")) {
        Serial.println(F("!!! No answer of RAK3172"));
        Serial.println(F("!!! Check the RX/TX wiring"));
    }

    Serial.println(F("--- Module Configuaration ---"));
    sendAT("AT+NWM=1");    
    sendAT("AT+NJM=1");    
    sendAT("AT+CLASS=A"); 
    sendAT("AT+BAND=4");   

    sendAT(String("AT+DEVEUI=") + DEVEUI);
    sendAT(String("AT+APPEUI=") + APPEUI);
    sendAT(String("AT+APPKEY=") + APPKEY);

    Serial.println(F("--- JOIN OTAA started---"));
    sendAT("AT+JOIN=1:0:10:8", 5000);

    Serial.println(F("Wait '+EVT:JOINED' (can take few secondes)"));
}

void loop() {

    while (RAK.available()) {
        char c = RAK.read();
        if (c == '\n') {
            rxLine.trim();
            if (rxLine.length() > 0) {
                Serial.print(F("[RAK3172] "));
                Serial.println(rxLine);

                if (rxLine.indexOf("+EVT:JOINED") != -1) {
                    joined = true;
                    Serial.println(F(">>> JOIN DONE ! <<<"));
                } else if (rxLine.indexOf("+EVT:JOIN_FAILED") != -1) {
                    Serial.println(F(">>> JOIN FAIL : check DEVEUI/APPEUI/APPKEY on TTN,"));
                } else if (rxLine.indexOf("DUTYCYCLE") != -1 || rxLine.indexOf("BUSY") != -1) {
                    Serial.println(F(">>> Le module applique le duty cycle EU868 : attends un peu avant de renvoyer."));
                }
            }
            rxLine = "";
        } else if (c != '\r') {
            rxLine += c;
        }
    }

    if (joined && (millis() - lastSend > TX_INTERVAL)) {
        lastSend = millis();

        uint8_t payload[1] = { 0x42 };
        String hexPayload = bytesToHex(payload, sizeof(payload));

        String cmd = "AT+SEND=" + String(LORA_PORT) + ":" + hexPayload;
        sendAT(cmd, 5000);
    }
}

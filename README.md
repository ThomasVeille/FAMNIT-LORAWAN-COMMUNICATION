# LORAWAN COMMUNICATION WITH EU868 Unit module

## Overview

This projet group few test code to communicate between two card like ESP32 card with the P2P solution and with the using of TTN. The following link corresponds to the information for the ESP32 board used (https://github.com/blazjerman/ESP32-Based-Sensor-PCB/tree/main).

The file use different sensor like temperature sensor or microphone. For each file, the different PINOUT are indicate in the "Code file" parts.
For the project, a 3D design was created to simulate a real conditions for the LoRaWAN antenna and the microphone.

## Connection with the ESP32 board
### A- LoRaWAN module

| LoRaWAN Pin | ESP32 Pin |
|--------------------|--------|
|**5V**              | 5V     |
|**GND**              | GND     |
|**TXD**              | RXD     |
|**RXD**              | TXD     |

### B-MPU-6050 

| MPU-6050 | ESP32 Pin |
|--------------------|--------|
|**VCC**              | 5V     |
|**GND**              | GND     |
|**SDA**              | 22     |
|**SCL**              | 21     |

### C-ICS43434 

| ICS43434 | ESP32 Pin |
|--------------------|--------|
|**SEL**              | GND     |
|**LRCL**              | 33     |
|**DOUT**              | 34     |
|**BCLK**              | 32     |
|**GND**              | GND     |
|**3V**              | 3V3     |

---------------

## 1. Creation of TTN Application

To use the TTN communication, you need to create one application on The Things Network. First, you need to create an account to use TTN. Then, need to create the first application. Go to **Create application** and complete the Application ID and Application name. You can also add an description of the app. The application is now created et can be used to save the devices.

## 2. Save the device

To save one device, you need to go to **Applications** and select the application created. On the menu, go to **End devices** and after click to **Register end device**.

### 2.1 Configuration of the device

On the **Register end device** page, need to chose **Enter end device specifics manually** in the **Input method** section.
Then follow the table to complete the next settings : 

| Setting name | Setting |
|--------------------|--------|
|**Frenquency plan**              | Europe 863-870 MHz for RX2 - recommended     |
|**LoRaWAN version**              | LoRaWAN Specification 1.03     |
|**Regional Parameters version**              | RP001 Regional Parameters 1.0.3 revision A     |

Then on the **Provisioning informaion**, need to complete the **JoinEUI** part with an random value. After you just need to genrated the **DevEUI** and **AppKey**. After add one name to the device and clik to **Register end device** to finish the registration.

### 2.2 Check the connexion between the LoRaWAN module and the ESP32

In the **AT_command** code, you can check the connexion between the ESP32 and the LoRaWAN module. On the table, you can use the following command to have different information :

| Command | What the command return |
|--------------------|--------|
|**AT+VER=?**              | Know the firmware version    |
|**AT+NWM=?**              | Know the actual mode - P2P = 0 / LoRa = 1     |
|**AT+BAND=?**              | Know the region - EU868 = 4 for Europe   |
|**AT+DEVEUI=?**              | Know the DEVEUI of the RAK3172 - They must be the same via the TTN DevEUI  |
|**AT+APPEUI=?**              | Check if it is the same like the LoRaWAN server   |
|**AT+APPKEY=?**              | Check the configuration of the AppKey  |
|**AT+NJS=?**              | Know the Network Join statut - No Connected = 0 / Connected = 1 |

## 3. Connect the RAK3172 to the LoRaWAN network

Once the configuration has been checked, the RAK3172 can attempt to join the LoRaWAN network.
Use the following command : **AT+JOIN=1**.
A successful attempt returns : **OK** followed by : **+EVT:JOINED**. 
The **+EVT:JOINED** message confirms that the RAK3172 successfully joined the LoRaWAN network. The connection can also be checked with : **AT+NJS=?**.
On the TTN Live DATA page, you see the message of the join between the network and the RAK. After you can send the following command to send a message : **AT+SEND=2:12345678**. Then the message must be appears on the Live DATA page.

## 4. Automatic connection to TTN

When the **AT_command** code return you the data on the Live DATA page on the TTN, you can use the **AT_automatic_send** code to make automatically the connection between the 2 parts.
If the connection works with success, on TTN, the message appear on the Live Data page after the join message.

## 5. Send automatic message come from one sensor

Now, the **MPU_6050_TTN** code allow to send message of the MPU_6050 sensor. After communication is working, the inforamtion are available one the Live Data page.

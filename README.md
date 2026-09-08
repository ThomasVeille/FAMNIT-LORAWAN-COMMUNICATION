# LORAWAN COMMUNICATION WITH EU868 Unit module

## Overview

This projet group few test code to communicate between two card like ESP32 card with the P2P solution and with the using of TTN. The following link corresponds to the information for the ESP32 board used (https://github.com/blazjerman/ESP32-Based-Sensor-PCB/tree/main).

The file use different sensor like temperature sensor or microphone. For each file, the different PINOUT are indicate in the "Code file" parts.
For the project, a 3D design was created to simulate a real conditions for the LoRaWAN antenna and the microphone.

## Connection with the ESP32 board
### A-receiver 
For this file, you just need to connect the LoRaWAN module to the card

| LoRaWAN Pin | ESP32 Pin |
|--------------------|--------|
|**5V**              | 5V     |
|**GND**              | GND     |
|**TXD**              | RXD     |
|**RXD**              | TXD     |

### B-sensor_transmitter 
For this file, the MPU-6050 sensor is connect to the ESP32. 

| MPU-6050 | ESP32 Pin |
|--------------------|--------|
|**VCC**              | 5V     |
|**GND**              | GND     |
|**SDA**              | 22     |
|**SCL**              | 21     |

The LoRaWAN antenna is connected like the receiver code.

### C-temp_TTN 
This code **temp_TTN** use the same configuration like the sensor_transmitter. However, they use TTN to receive the information.
### D-RS-ZS-BZ 
The coce **RS-ZS-BZ** file measure the noise with the RS-ZS-BZ sensor. Need to assemble this one with the ESP32 card.
### E-ICS43434 
This **ICS43434** measure the noise with the ICS43434 sensor.

| ICS43434 | ESP32 Pin |
|--------------------|--------|
|**SEL**              | GND     |
|**LRCL**              | 33     |
|**DOUT**              | 34     |
|**BCLK**              | 32     |
|**GND**              | GND     |
|**3V**              | 3V3     |

## What does the code do ?
### A-receiver 

The code **receiver** allow to configure one board with the reception setting for the LoRaWAN module. To receive information you also need to configure another board with the **sensor_transmitter** code.

### B-sensor_transmitter 

The code **sensor_transmitter** allow to configure one board with the transmitter setting for the LoRaWAN module. The message send to the other board come from the **MPU-6050** sensor.

### C-temp_TTN 

The code **temp_TTN** allow to send the data from the sensor to TTN website. You need to configure one object on the following link : https://eu1.cloud.thethings.network/console/. First, create an account and then you will be able to start to configure/create one device and one application.
To create, you just need to go the **Crate application** link and complete the different part with name, ID and description. For the second part, need to go to the **Add end device** link and chose the application created.

### D-RS-ZS-BZ 

The code **RS-ZS-BZ** allow to measure the noise with e RS-ZS-BZ sensor.

### E-ICS43434 

The code **ICS43434** allow to measure the noise with the ICS43434 sensor.


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
|**AT+JOIN=?**              | Ask to RAK join the network   |
|**AT+SEND=2:12345678**              | Try to send a LoRaWAN message  |

Before send one message with the last command, you need to receive **+EVT:JOINED** $. This message confirme you the RAK is connected with the network.

# LORAWAN COMMUNICATION WITH EU868 Unit module

## Overview

This projet group few test code to communicate between two card like ESP32 card with the P2P solution and with the using of TTN. The following link corresponds to the information for the ESP32 board used (https://github.com/blazjerman/ESP32-Based-Sensor-PCB/tree/main).

The file use different sensor like temperature sensor or microphone. For each file, the different PINOUT are indicate in the "Code file" parts.
For the project, a 3D design was created to simulate a real conditions for the LoRaWAN antenna and the microphone.

## Code file
  - receiver -> For this file, you just need to connect the LoRaWAN module to the card

| LoRaWAN Pin | ESP32 Pin |
|--------------------|--------|
|**5V**              | 5V     |
|**GND**              | GND     |
|**TXD**              | RXD     |
|**RXD**              | TXD     |

  - sensor_transmitter -> For this file, the MPU-6050 sensor is connect to the ESP32. 

| MPU-6050 | ESP32 Pin |
|--------------------|--------|
|**VCC**              | 5V     |
|**GND**              | GND     |
|**SDA**              | 22     |
|**SCL**              | 21     |

The LoRaWAN antenna is connected like the receiver code.

  - temp_TTN -> This file use the same configuration like the sensor_transmitter. However, they use TTN to receive the information.
  - RS-ZS-BZ -> This file measure the noise with the RS-ZS-BZ sensor. Need to assemble this one with the ESP32 card
  - ICS43434 -> This file measure the noise with the ICS43434 sensor

| ICS43434 | ESP32 Pin |
|--------------------|--------|
|**SEL**              | GND     |
|**LRCL**              | 33     |
|**DOUT**              | 34     |
|**BCLK**              | 32     |
|**GND**              | GND     |
|**3V**              | 3V3     |


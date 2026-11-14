# CAT 14 – BME280 and MQTT-Based Environmental Monitoring

## Overview

This practical focuses on the monitoring and visualization of environmental sensor data using MQTT and Node-RED.

Sensor readings obtained from the BME280 are transmitted through the MQTT communication system and made available for visualization using Node-RED.

## Objective

To receive environmental sensor data through MQTT and use Node-RED to organize and visualize the incoming information.

## System Components

- STM32 NUCLEO-F410RB
- BME280 Environmental Sensor
- ESP8266 / Wemos D1 Mini
- MQTT Broker
- MQTTX
- Node-RED

## System Working

1. The BME280 sensor measures temperature, humidity and atmospheric pressure.
2. The STM32 processes the sensor readings.
3. The ESP8266 establishes Wi-Fi connectivity.
4. Sensor measurements are transmitted through MQTT.
5. Node-RED receives the published messages.
6. The incoming data is processed for visualization and monitoring.

## Communication Technologies

| Technology | Application |
|---|---|
| I2C | Sensor communication |
| UART | STM32–ESP8266 communication |
| Wi-Fi | Network connectivity |
| MQTT | Message transmission |
| Node-RED | Data processing and visualization |

## Software Requirements

- STM32CubeMX
- Keil MDK
- Node-RED
- MQTTX
- MQTT Broker

## Expected Outcome

Environmental measurements are transmitted through MQTT and made available for monitoring and visualization using Node-RED.

# CAT 14 – BME280 and Node-RED Integration

## Overview

This folder contains the STM32 source code and supporting BME280 sensor driver files used in the environmental monitoring application.

The project focuses on acquiring environmental measurements and integrating the sensor-data communication system with MQTT and Node-RED.

## File Description

| File | Description |
|---|---|
| `main.c` | Main application program |
| `bme280.c` | BME280 sensor driver implementation |
| `bme280.h` | BME280 API declarations |
| `bme280_defs.h` | Sensor register definitions and configuration constants |
| `stm32f4xx_hal_msp.c` | STM32 peripheral low-level initialization |
| `NodeRED_Flow_Reference.png` | Reference image of the Node-RED flow |

## Working Principle

The STM32 application communicates with the BME280 sensor through I2C and obtains environmental measurements.

The readings are formatted for transmission through the communication system. MQTT provides the messaging interface, while Node-RED is used for receiving and visualizing the sensor data.

## Development Environment

- STM32CubeMX
- Keil MDK
- STM32 HAL Library
- Bosch BME280 Sensor API
- Node-RED
- MQTTX

## Note

This folder contains application source files and supporting references. It should be used alongside the corresponding STM32 project configuration and required libraries.

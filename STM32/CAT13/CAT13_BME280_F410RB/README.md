# CAT 13 – BME280 Source Code

## Overview

This folder contains the source files required for interfacing the BME280 environmental sensor with the STM32 NUCLEO-F410RB microcontroller.

The implementation uses the Bosch BME280 Sensor API along with STM32 HAL functions for I2C communication.

## File Description

| File | Description |
|---|---|
| `main.c` | Main application code for sensor initialization, data acquisition and MQTT publishing |
| `bme280.c` | Bosch BME280 sensor driver implementation |
| `bme280.h` | BME280 driver function declarations and structures |
| `bme280_defs.h` | Sensor register definitions, constants and configuration parameters |
| `stm32f4xx_hal_msp.c` | Peripheral low-level initialization and GPIO configuration |
| `CAT12_BME280_Circuit.png` | BME280 sensor interfacing circuit reference |

## Implementation

The application initializes the BME280 sensor through I2C1 and obtains compensated temperature, humidity and pressure measurements.

The ESP8266 communication library is used to establish Wi-Fi connectivity and publish the sensor readings to an MQTT broker.

## Development Environment

- STM32CubeMX
- Keil MDK
- STM32 HAL
- Bosch BME280 Sensor API

## Note

The source files are intended to be used with the corresponding STM32 project configuration and supporting libraries.

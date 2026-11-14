# CAT 12 – BME280 Sensor Interfacing using I2C

## Overview

This practical demonstrates interfacing the BME280 environmental sensor with the STM32 NUCLEO-F410RB development board using the I2C communication protocol.

The sensor provides temperature, humidity and atmospheric pressure measurements, which are processed by the STM32 and transmitted through UART.

## Objective

To establish I2C communication between the STM32 microcontroller and BME280 sensor, retrieve environmental measurements and transmit the readings in JSON format through UART.

## Hardware Components

- STM32 NUCLEO-F410RB
- BME280 Environmental Sensor
- USB-to-Serial Connection
- Jumper Wires

## Communication Interfaces

| Interface | Purpose |
|---|---|
| I2C1 | Communication with BME280 |
| USART2 | Transmission of sensor readings |
 
## Working Principle

1. Initialize the STM32 peripherals and system clock.
2. Configure I2C1 for communication with the BME280 sensor.
3. Initialize the Bosch BME280 Sensor API.
4. Read temperature, humidity and pressure values.
5. Construct a JSON payload containing the sensor readings.
6. Transmit the generated JSON data through UART.

## Software Requirements

- STM32CubeMX
- Keil MDK
- STM32 HAL Library
- Bosch BME280 Sensor API
- Serial Terminal

## Expected Outcome

The STM32 successfully communicates with the BME280 sensor and periodically displays the measured temperature, humidity and pressure values in JSON format through the serial terminal.

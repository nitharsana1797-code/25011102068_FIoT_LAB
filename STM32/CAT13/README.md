# CAT 13 – BME280 Sensor Interfacing using I2C and MQTT

## Overview

This practical demonstrates the integration of the BME280 environmental sensor with the STM32 NUCLEO-F410RB microcontroller and ESP8266 Wi-Fi module.

The system collects environmental measurements and transmits them wirelessly to an MQTT broker using JSON-formatted messages.

## Objective

To acquire temperature, humidity and atmospheric pressure readings from the BME280 sensor through I2C and publish the measurements to an MQTT broker using the ESP8266 Wi-Fi module.

## Hardware Components

- STM32 NUCLEO-F410RB
- BME280 Environmental Sensor
- ESP8266 / Wemos D1 Mini
- Jumper Wires

## Communication Interfaces

| Interface | Purpose |
|---|---|
| I2C1 | BME280 sensor communication |
| USART1 | STM32 and ESP8266 communication |
| MQTT | Wireless sensor-data transmission |

## Working Principle

1. Initialize the STM32 system clock and peripherals.
2. Configure I2C1 and initialize the BME280 sensor using the Bosch Sensor API.
3. Initialize the ESP8266 in station mode.
4. Connect the ESP8266 to the available Wi-Fi network.
5. Establish a TCP connection with the MQTT broker.
6. Configure the MQTT client.
7. Read temperature, humidity and pressure measurements.
8. Convert the readings into a JSON payload.
9. Publish the sensor data periodically to the MQTT topic.

## MQTT Configuration

| Parameter | Configuration |
|---|---|
| Broker | broker.emqx.io |
| Port | 1883 |
| Topic | agrisense/rice/bme280 |
| Protocol | MQTT |
| Payload | JSON |

## Software Requirements

- STM32CubeMX
- Keil MDK
- STM32 HAL Library
- Bosch BME280 Sensor API
- ESP8266 MQTT Library
- MQTTX

## Expected Outcome

The system continuously collects environmental readings from the BME280 sensor and publishes them as JSON messages through the ESP8266 to the MQTT broker. The published messages can be monitored using MQTTX.

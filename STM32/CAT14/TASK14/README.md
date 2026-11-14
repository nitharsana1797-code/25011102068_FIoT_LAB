# CAT 13 — BME280 Sensor Interfacing using I2C and MQTT

**Board:** NUCLEO-F410RB (STM32F410RB)  
**Sensor:** BME280  
**Wi-Fi module:** ESP8266 / Wemos D1 mini  
**Protocol:** I2C1 + USART1 + MQTT  
**MQTT broker:** `broker.emqx.io`, TCP port `1883`  
**MQTT topic:** `agrisense/rice/bme280`

## Objective

Read compensated temperature, humidity and pressure values from the BME280 over I2C, construct a JSON payload on the STM32, publish it through the ESP8266 using the supplied custom ESP8266/MQTT library, and receive the JSON in Node-RED for visualization/analytics.

## Files in this folder

- `main.c` — BME280 initialization, Wi-Fi/MQTT setup, periodic JSON publishing.
- `bme280.c`, `bme280.h`, `bme280_defs.h` — Bosch BME280 driver.
- `ESPLib.c`, `ESPLib.h` — supplied ESP8266/MQTT library, adapted to STM32F4 HAL.
- `stm32f4xx_hal_msp.c` — I2C1 and USART1 pin/clock initialization for NUCLEO-F410RB.
- `CAT13_BME280_MQTT_Circuit.png` — wiring reference.
- `NodeRED_Flow_Reference.png` — screenshot supplied for the Node-RED MQTT flow.

## Pin connections

### BME280 to NUCLEO-F410RB

| BME280 | NUCLEO-F410RB |
|---|---|
| VCC | 3.3 V |
| GND | GND |
| SCL | PB8 (I2C1_SCL, AF4) |
| SDA | PB9 (I2C1_SDA, AF4) |
| SDO | GND for I2C address 0x76 |

The code uses `BME280_I2C_ADDR_PRIM` (normally 0x76). If SDO is tied HIGH, change it to the secondary address (normally 0x77).

### ESP8266 to NUCLEO-F410RB

| ESP8266 | NUCLEO-F410RB |
|---|---|
| RX | PA9 (USART1_TX) |
| TX | PA10 (USART1_RX) |
| GND | GND |
| VCC | Regulated 3.3 V supply |
| EN / CH_PD | 3.3 V |

UART settings: 115200 baud, 8 data bits, no parity, 1 stop bit. The ESP8266 AT firmware baud rate must match.

**Power warning:** ESP8266 is a 3.3 V device. Do not connect its VCC or UART pins to 5 V. Use a stable 3.3 V supply capable of handling Wi-Fi current peaks, and connect the STM32 and ESP8266 grounds together.

## Keil / STM32 project integration

1. Keep the project device selected as **STM32F410RB**. Do not change it to F401RE/F401RB.
2. Add `main.c`, `bme280.c`, and `ESPLib.c` to the project source group.
3. Add this folder to the compiler include paths so `main.h`, `bme280.h`, `bme280_defs.h`, `ESPLib.h`, and the STM32F4 HAL headers can be found.
4. If the CubeMX project already has `stm32f4xx_hal_msp.c`, merge the I2C1 PB8/PB9 and USART1 PA9/PA10 configuration into its existing MSP functions. Do not compile duplicate `HAL_MspInit`, `HAL_I2C_MspInit`, or `HAL_UART_MspInit` definitions.
5. Confirm the project has the STM32F4 HAL I2C and UART modules enabled.

The supplied CAT13 PDF's example uses USART2, but the `main.c` provided for this package explicitly uses **USART1**. The circuit diagram and MSP file therefore follow the supplied code's USART1 choice.

## Wi-Fi credentials

Before programming, replace the placeholders in `main.c`:

```c
ESP8266_wifiConnect("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
```

Use your own network credentials. Do not commit a real Wi-Fi password to a public GitHub repository.

## MQTT and Node-RED setup

1. Configure an MQTT broker connection to `broker.emqx.io` on TCP port `1883`.
2. In Node-RED, configure an **mqtt in** node with topic `agrisense/rice/bme280`.
3. Connect the MQTT-in node to a **json** node if the payload arrives as a string.
4. Connect the JSON node to a **debug** node or dashboard widgets.
5. Deploy the flow. The payload is published approximately once per second and has this shape:

```json
{"temperature":24.22,"humidity":62.44,"pressure":1011.77}
```

The values above are illustrative only; live readings depend on the sensor and environment.

## Notes / verification

- `main.c` preserves the supplied BME280 measurement conversion: temperature in °C, pressure converted from Pa to hPa, and humidity in %RH.
- The code publishes only after a successful BME280 read.
- The `ESPLib.h` HAL include has been changed from STM32F1 HAL to STM32F4 HAL for the F410RB target.
- `BUFFERSIZE` is set to 300 to match the 300-byte receive-buffer wrap limit used by the supplied `ESPLib.c`.
- This package contains source files and wiring documentation, not a complete CubeMX/Keil project. Add the files to your existing generated project and verify the selected clock source and peripheral configuration against your actual board.

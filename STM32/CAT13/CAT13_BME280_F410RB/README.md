# CAT 12 — BME280 Sensor Interfacing (NUCLEO-F410RB)

## Board choice
This package targets **STM32 NUCLEO-F410RB (STM32F410RB)**, matching the board used in your laboratory. Do not change the project target to STM32F401RE just because a CAT handout or template mentions another variant.

## What this package implements
- BME280 sensor communication using I2C1.
- Bosch BME280 Sensor API initialization and calibration-data loading.
- Temperature, pressure and humidity oversampling configuration.
- Normal measurement mode.
- Periodic compensated sensor data acquisition.
- JSON payload construction.
- HTTP/1.1 response construction with `Content-Length` calculated using `strlen()`.
- UART2 output for the JSON payload and HTTP response preview.

**Important scope note:** The code you supplied was only the BME280/I2C acquisition portion. The CAT 12 handout also describes sending the HTTP response through an ESP8266 using `AT+CIPSEND`, followed by Node-RED analytics. This package prepares and prints the JSON/HTTP response over UART2, but does not claim that Wi-Fi connection, ESP8266 AT-command setup, HTTP client/server routing, or Node-RED dashboard integration is complete. Those require the ESP8266 wiring, UART selection, Wi-Fi/network details and server endpoint.

## Folder structure
- `Core/Src/main.c` — application code.
- `Core/Src/stm32f4xx_hal_msp.c` — I2C1 and USART2 pin alternate-function setup.
- `Drivers/BME280/bme280.c` — Bosch sensor driver.
- `Core/Inc/bme280.h` — Bosch driver API header.
- `Core/Inc/bme280_defs.h` — Bosch driver definitions.
- `Documentation/circuit_diagram.png` — wiring reference.

## Hardware connections
### BME280 to NUCLEO-F410RB (I2C1)
| BME280 module | NUCLEO-F410RB |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SCL | PB8 (I2C1_SCL, AF4) |
| SDA | PB9 (I2C1_SDA, AF4) |
| SDO | GND for address 0x76 |
| CSB | 3V3 for I2C mode |

Use a 3.3 V-compatible BME280 breakout. Many breakout boards include I2C pull-up resistors; if yours does not, add suitable pull-ups to 3.3 V.

### UART output
- USART2 TX: PA2
- USART2 RX: PA3
- Baud rate: 115200, 8 data bits, no parity, 1 stop bit.
- On common NUCLEO configurations USART2 is connected to the ST-LINK virtual COM port. Confirm your board revision and jumper/solder-bridge configuration.

## Bosch driver placement (as specified by the CAT PDF)
- `bme280.c` belongs in the project source folder / Keil source group.
- `bme280.h` and `bme280_defs.h` belong in the include folder.
- Add the include folder to Keil Options for Target → C/C++ → Include Paths.
- Add `bme280.c` to the project build; copying the file alone is not enough.

## Clock
The code uses the previously used NUCLEO-F410RB configuration:
- HSE bypass input assumed at 8 MHz.
- PLL M=4, N=100, P=2.
- SYSCLK/HCLK = 100 MHz.
- APB1 = HCLK/2; APB2 = HCLK.
- Flash latency = 3.

Confirm the actual HSE clock source and frequency in your CubeMX-generated project before flashing. If your existing CubeMX clock configuration differs, retain the known-good generated `SystemClock_Config()` instead of blindly replacing it.

## Build and test
1. Create/open a CubeMX/Keil project for **STM32F410RB**.
2. Ensure the STM32F4 HAL, CMSIS startup and device files match STM32F410RB.
3. Add `Core/Src/main.c` to the project.
4. Add `Drivers/BME280/bme280.c` to the project source group.
5. Add `Core/Inc` to the compiler include paths.
6. Add/merge `stm32f4xx_hal_msp.c`. If CubeMX already generated MSP functions, merge the I2C1 PB8/PB9 and USART2 PA2/PA3 configuration into the existing functions; do not compile duplicate `HAL_I2C_MspInit` or `HAL_UART_MspInit` definitions.
7. Connect the BME280 and open a serial terminal at 115200 baud.
8. Flash and check for the initialization message, then JSON and HTTP response preview once per second.

## Expected UART output shape
`{"status":"success","temperature":28.45,"humidity":62.30,"pressure":1008.72}`

The numerical values above are only an example from the handout, not guaranteed sensor readings.

## Driver source
The included Bosch driver files are the files supplied with this CAT: `bme280.c`, `bme280.h`, and `bme280_defs.h`. Bosch's BSD-3-Clause license header is retained in the driver files.

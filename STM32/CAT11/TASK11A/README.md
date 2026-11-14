# Task 10 — HTTP Connection using Wemos ESP8266

This is the HTTP-server continuation of the Wemos/ESP8266 work. It is kept as **Task 10** separately from Tasks 9A–9C, following the numbering distinction in the class announcement.

## What the supplied firmware does

1. Initializes the STM32F410RB and USART1.
2. Sends ESP8266 AT commands to check the modem, select station mode, join Wi-Fi, and query the IP address.
3. Enables multiple connections and starts an HTTP server on port 80.
4. Waits for a browser request (`+IPD` notification).
5. Sends an HTTP 200 response containing a small HTML page.
6. Closes the client connection.

## Before building

Edit `Core/Src/main.c` and set:

- `WIFI_NAME`
- `WIFI_PASSWORD`

Do not publish real Wi-Fi credentials in a public repository.

## Hardware / UART

- MCU: STM32F410RBT6 (as selected in the supplied `.ioc` file)
- USART1 TX: PA9 → Wemos RX
- USART1 RX: PA10 ← Wemos TX
- GND: STM32 GND ↔ Wemos GND
- UART: 115200 baud, 8-N-1
- Wemos D1 mini uses 3.3 V UART logic. Power the Wemos from its USB connector or a suitable regulated supply; do not connect its 3V3 pin to 5 V.

The supplied project clock configuration expects an 8 MHz external clock input (HSE bypass), as recorded in the original CubeMX configuration. Confirm the board's clock source before changing the clock setup.

## Testing

1. Build and flash the STM32F410 project.
2. Connect the Wemos to the STM32 UART and power it.
3. Wait for the ESP8266 to join the configured Wi-Fi network.
4. Find the station IP printed/returned by `AT+CIFSR` (the current source does not forward this response to a PC terminal).
5. From a device on the same Wi-Fi network, open `http://<ESP8266-IP>/`.

**Note:** This project is a basic HTTP server returning a static HTML page. It does not yet read a sensor or display live sensor values.

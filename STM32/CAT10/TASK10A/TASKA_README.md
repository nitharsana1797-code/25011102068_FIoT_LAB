# CAT10 — Wemos ESP8266 Wi-Fi Modem Interface

This project follows the task labels printed in the supplied Continuous Lab Evaluation-10 PDF:

- **Task 9A:** Establish STM32 ↔ Wemos UART communication.
- **Task 9B:** Explore the ESP8266 AT-command interface (investigation table).
- **Task 9C:** Connect Wemos to Wi-Fi.
- **Task 10:** HTTP server continuation, provided as a separate STM32F410 project in the sibling `TASK10_HTTP_STM32F410` folder.

## STM32F103 Wemos project

`Core/Src/main.c` is configured to run Task 9C by default. To run Task 9A, replace the active `Core/Src/main.c` with `Task_Source_Codes/TASK9A/main.c`, or copy the Task 9A source into `Core/Src/main.c`. Do not add multiple `main.c` files to the active Keil build.

Task 9B is documented in `Task_Source_Codes/TASK9B/README.md` because the handout asks students to discover and record AT commands rather than implement a separate program.

## UART configuration

- USART1 (PA9 TX, PA10 RX): STM32 ↔ Wemos D1 mini
- USART2 (PA2 TX, PA3 RX): STM32 ↔ PC USB-to-TTL/serial terminal
- Serial format: 115200 baud, 8 data bits, no parity, 1 stop bit (8-N-1)
- Common ground is required.
- Wemos UART logic is 3.3 V. Do not apply 5 V logic to its RX pin.

## Wi-Fi credentials

Before running Task 9C, edit `WIFI_SSID` and `WIFI_PASSWORD` in `Core/Src/main.c` with the network credentials used for the lab. Keep credentials private and do not publish them in a public repository.

See `circuit.png` for the connection diagram.

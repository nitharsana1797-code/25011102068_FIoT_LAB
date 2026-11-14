# CAT11 — Wemos ESP8266 Wi-Fi Modem and HTTP Continuation

This repository combines the supplied ESP8266/Wemos STM32 project and the HTTP continuation. Folder names follow the numbering distinction in the lab handout and class announcement.

## Numbering used

| Folder / task | What it contains |
|---|---|
| `STM32F103_Wemos_UART/Task_Source_Codes/TASK10A` | Task 10A — prove STM32 ↔ Wemos UART communication |
| `STM32F103_Wemos_UART/Task_Source_Codes/TASK10B` | Task 10B — AT-command investigation table (the handout asks students to investigate commands) |
| `STM32F103_Wemos_UART/Task_Source_Codes/TASK10C` | Task 10C — configure station mode, join Wi-Fi, query IP |
| `TASK10_HTTP` | HTTP server continuation, using the separate STM32F410RB project |

The CAT10 PDF labels the Wemos work as Tasks **10A, 10B and 10C**. The HTTP continuation is kept separately as **Task 10** rather than renaming the handout's 10A–10C tasks.

## Hardware projects are different

- Tasks 10A–10C use the supplied **STM32F103C8T6** CubeMX project (`ESP8266_A.ioc`).
- Task 10 HTTP uses the supplied **STM32F410RBT6** CubeMX project (`http_protocol.ioc`).

Do not mix the two MCU projects or copy one project's `main.c` into the other.

## Circuit diagrams

- `circuit.png` — STM32F103C8T6, Wemos D1 mini and PC terminal connections for Tasks 10A–10C.
- `TASK10_HTTP/circuit.png` — STM32F410RBT6 to Wemos D1 mini UART connections for the HTTP continuation.

Editable SVG versions are supplied beside the PNG files.

## How to select a task build

For the STM32F103 project, `Core/Src/main.c` is set to Task 9C by default. To run Task 10A, copy `Task_Source_Codes/TASK10A/main.c` over `Core/Src/main.c`. Keep only one `main.c` in the active Keil build.

Task 10B is a command investigation; follow `Task_Source_Codes/TASK10B/README.md` and use the Task 10A UART bridge to enter commands from a PC terminal.

For CAT 10, open `TASK10_HTTP/STM32F410_HTTP/http_protocol.ioc` or its Keil project and build that project independently.

## UART and power safety

- UART format in the supplied firmware: 115200 baud, 8 data bits, no parity, 1 stop bit (8-N-1).
- Cross TX/RX connections and connect grounds together.
- Wemos D1 mini UART signals are 3.3 V logic. Never apply 5 V to its RX pin.
- Power the Wemos through USB or a suitable regulated supply. Do not connect 5 V to its 3V3 pin.
- Before testing Task 10C or CAT 11, set Wi-Fi credentials locally in the relevant `main.c`. Placeholder credentials are included so real passwords are not published.

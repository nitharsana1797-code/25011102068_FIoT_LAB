# STM32 UART Command-Controlled LED

## Hardware and serial configuration
- MCU/project target: STM32F103C8T6 (Blue Pill configuration)
- UART peripheral: USART2
- USART2_TX: PA2
- USART2_RX: PA3
- Baud rate: 115200
- Format: 8 data bits, no parity, 1 stop bit (8N1)
- LED: onboard PC13, active-low on the typical Blue Pill board

## Task 9A — Basic UART Communication
File: `Task_Source_Codes/TASK9A/main.c`

- Sends the character `A` to the PC terminal once every second.
- Receives one character from the PC and transmits the same character back (echo).
- Uses polling HAL UART APIs.

## Task 9B — String-Based Application Logic
File: `Task_Source_Codes/TASK9B/main.c`

Enter one command per line in the serial terminal:
- `LED ON` — turns the onboard LED on.
- `LED OFF` — turns the onboard LED off.
- `LED TOGGLE` — changes the LED state.
- `HELP` — prints the command list.

Commands are case-insensitive. Press Enter to submit a command.

## Project organization
- `Core/Src/main.c` is the active project source and currently contains Task 9B.
- `Task_Source_Codes/TASK9A/main.c` and `TASK9B/main.c` are standalone alternatives for submission/reference.
- Only one `main()` implementation should be included in a build at a time.

## PC terminal settings
Use PuTTY or another serial terminal:
- Select the COM port assigned to the USB-to-TTL adapter.
- 115200 baud, 8 data bits, no parity, 1 stop bit.
- Flow control: None.
- Connect adapter TX to board PA3 (USART2_RX), adapter RX to board PA2 (USART2_TX), and connect GND to GND.
- Use 3.3 V UART logic levels; do not connect 5 V TTL signals to STM32 pins.

## Important board note
This project targets STM32F103C8T6 / Blue Pill and assumes its onboard LED is on PC13 and active-low. If your actual board is a different STM32 development board, confirm its onboard LED pin and polarity and update `LED_PORT`, `LED_PIN`, and GPIO initialization accordingly.

## Build
Open the existing Keil project in `MDK-ARM` and build with the active `Core/Src/main.c`. To build Task 9A instead, replace the active `Core/Src/main.c` with the contents of `Task_Source_Codes/TASK9A/main.c`. Do not add both task source files to the same build.

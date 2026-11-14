# CAT 10 – STM32 and Wemos UART Interface

## Overview
This CAT focuses on UART communication between the STM32F103C8T6 microcontroller and the Wemos D1 Mini (ESP8266), exploring serial communication and Wi-Fi connectivity using AT commands.

## Tasks

### Task 10A – UART Communication Test
Establishes UART communication between STM32 and Wemos, verifies ESP8266 responses and implements a two-way UART bridge with the PC terminal.

### Task 10B – ESP8266 AT Command Investigation
Explores different ESP8266 AT commands for checking firmware information, configuring Wi-Fi mode, scanning networks, connecting to access points and establishing TCP connections.

### Task 10C – Wi-Fi Connection Using AT Commands
Configures the ESP8266 in Station Mode, connects it to a Wi-Fi network and retrieves its assigned IP address.

## Hardware Used
- STM32F103C8T6
- Wemos D1 Mini (ESP8266)
- USB-to-TTL Serial Adapter

## Communication
- USART1 – STM32 to Wemos
- USART2 – STM32 to PC Terminal
- Baud Rate – 115200

## Result
UART communication was established between the STM32 and ESP8266, and AT commands were explored to test the module and configure wireless network connectivity.

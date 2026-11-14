# TASK 10C – Connecting Wemos D1 Mini to Wi-Fi Using AT Commands

## 1. Aim

To configure the ESP8266 Wi-Fi module on the Wemos D1 Mini using AT commands sent from the STM32F103C8T6 microcontroller and establish a connection with a wireless network.

## 2. Hardware Components

- STM32F103C8T6 (Blue Pill)
- Wemos D1 Mini (ESP8266)
- USB-to-TTL Serial Adapter
- Jumper Wires
- Wi-Fi Network

## 3. Software Requirements

- STM32CubeMX
- Keil MDK
- PuTTY / Tera Term
- ESP8266 AT Firmware

## 4. Circuit Connections

Two UART interfaces are used for communication.

| STM32 Pin | Connected To |
|---|---|
| PA9 (USART1 TX) | Wemos RX |
| PA10 (USART1 RX) | Wemos TX |
| PA2 (USART2 TX) | USB-to-TTL RX |
| PA3 (USART2 RX) | USB-to-TTL TX |
| GND | Common Ground |

**UART Configuration:**
- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1

## 5. Working Principle

The STM32 communicates with the ESP8266 through USART1 and displays the responses on the PC terminal using USART2.

The following AT commands are executed:

| AT Command | Purpose |
|---|---|
| `AT` | Checks communication with ESP8266 |
| `AT+CWMODE=1` | Configures ESP8266 in Station Mode |
| `AT+CWJAP="SSID","PASSWORD"` | Connects to the specified Wi-Fi network |
| `AT+CIFSR` | Retrieves the assigned IP address |

The program follows these steps:

1. Initialize the system clock, GPIO and UART peripherals.
2. Verify communication with the ESP8266 using the `AT` command.
3. Configure the Wi-Fi operating mode as Station Mode.
4. Send the Wi-Fi SSID and password to connect to the selected network.
5. Wait for the connection response from the ESP8266.
6. Retrieve and display the assigned IP address using `AT+CIFSR`.

The UART communication uses a command-response mechanism, where the STM32 sends AT commands and reads the corresponding ESP8266 responses.

## 6. Expected Output

The PC serial terminal displays:

- ESP8266 response to the `AT` command.
- Successful configuration of Station Mode.
- Wi-Fi connection status.
- IP address assigned to the Wemos D1 Mini.

## 7. Result

The ESP8266 Wi-Fi module is configured as a Station and connected to the specified wireless network using AT commands through the STM32F103C8T6. The assigned IP address is retrieved and displayed on the PC serial terminal.

# Task 9B — ESP8266 AT Command Investigation

The lab sheet asks students to investigate the ESP8266 AT-command interface and record the command used for each operation. This task is therefore documented as an investigation table, not as a separate ready-made firmware program.

| Requirement | AT command to investigate | Purpose / expected information |
|---|---|---|
| Check modem response | `AT` | Checks whether the ESP8266 responds; expected `OK` |
| Check firmware/version | `AT+GMR` | Displays AT firmware and SDK version information |
| Set Wi-Fi station mode | `AT+CWMODE=1` | Configures the ESP8266 as a Wi-Fi station |
| Scan available networks | `AT+CWLAP` | Lists visible Wi-Fi access points |
| Join an access point | `AT+CWJAP="SSID","PASSWORD"` | Connects to the selected Wi-Fi network |
| Obtain network information | `AT+CIFSR` | Displays the assigned station IP address |
| Establish a TCP connection | `AT+CIPSTART="TCP","host",port` | Opens a TCP connection to a server |
| Send application data | `AT+CIPSEND=length` | Enters data-send mode for the specified number of bytes |

## Suggested investigation procedure

1. Connect the STM32 and Wemos UART pins as shown in `../circuit.png`.
2. Open a serial terminal at the configured baud rate (115200, 8-N-1 for the supplied project).
3. Send each command with carriage return and line feed (`\r\n`).
4. Record the exact response displayed by the module.
5. Note that command availability and response text can vary with ESP8266 AT-firmware version.

## Important

- Replace `SSID` and `PASSWORD` with your own Wi-Fi credentials only when testing the join command.
- Do not commit personal Wi-Fi credentials to a public GitHub repository.
- The exact TCP host, port, and payload depend on the server used in your lab.
- The supplied Task 9A firmware provides a UART bridge so commands can be entered from the PC terminal.

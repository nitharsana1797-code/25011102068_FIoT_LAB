# CAT 6 – UART Communication using 8051

## Overview
This practical introduces serial communication using the Universal Asynchronous Receiver Transmitter (UART) available in the AT89S52 microcontroller.

The experiments demonstrate character transmission and interrupt-driven reception between the microcontroller and a computer using a serial terminal.

## Experiments

### TASK 6A – UART Initialization and Character Transmission
Configure the 8051 UART in Mode 1 for asynchronous serial communication. Timer 1 is configured to generate the required baud rate, and characters are transmitted through the serial port.

The transmitted message can be viewed using a terminal application such as PuTTY.

### TASK 6B – Interrupt-Driven UART Echo
Implement UART reception using serial interrupts. Received characters are stored in a circular buffer and transmitted back to the computer, demonstrating an interrupt-driven echo mechanism.

## Concepts Covered
- UART configuration
- Serial communication
- Baud-rate generation
- Timer 1 in auto-reload mode
- Serial data transmission and reception
- UART interrupts
- Circular buffer implementation

## Hardware and Software
- AT89S52 Microcontroller
- USB-to-TTL Serial Adapter
- 8051 Development Board
- Computer with PuTTY or equivalent serial terminal
- Keil µVision IDE

## Expected Outcome
The microcontroller successfully transmits serial messages to a computer and receives and echoes incoming characters, demonstrating UART communication and interrupt-based serial data handling.

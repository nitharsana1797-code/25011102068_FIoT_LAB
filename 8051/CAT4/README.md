# CAT 4 – Human Machine Interface using Matrix Keypad

## Overview
This practical demonstrates the development of a basic Human Machine Interface (HMI) using an 8051 microcontroller. A matrix keypad is interfaced with LEDs and a seven-segment display to perform different operations based on user input.

## Experiment
**HMI using Matrix Keypad, LEDs and Seven-Segment Display**

The user interacts with the system through a matrix keypad. The pressed key is displayed on the seven-segment display and the corresponding LED operation is executed.

## Key Functions
| Key | Operation |
|---|---|
| 1–4 | Control individual LEDs |
| 5 | Running LED pattern |
| 6 | Alternating LED pattern |
| 7 | Binary counter |
| 8 | All LEDs blink |
| 9 | Turn OFF all LEDs |
| 0 | Reset the system and display |

## Working Principle
The microcontroller continuously scans the matrix keypad to identify the pressed key. Based on the input, the corresponding digit is displayed on the seven-segment display and the associated LED operation is performed.

Separate functions are used for keypad scanning, display control and LED operations.

## Hardware and Software
- AT89S52 Microcontroller
- Matrix Keypad
- LEDs
- Seven-Segment Display
- 8051 Development Board
- Keil µVision IDE

## Expected Outcome
The system responds to keypad inputs by displaying the corresponding digit and performing the assigned LED operation, demonstrating keypad interfacing and coordinated peripheral control.

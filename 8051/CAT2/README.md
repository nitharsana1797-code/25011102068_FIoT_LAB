# CAT 2 – LED Blinking using 8051 Microcontroller

## Overview
This practical introduces basic digital output interfacing using the AT89S52 microcontroller. An LED is connected to one of the GPIO pins and programmed to blink continuously with a suitable time delay.

## Experiment
**LED Blinking using 8051**

The program uses Port 1, Pin 0 (P1.0) to control the LED. A software delay is introduced between the ON and OFF states to produce a visible blinking effect.

## Working Principle
- Configure P1.0 as the LED control output.
- Set the pin LOW to turn the LED ON.
- Introduce a time delay.
- Set the pin HIGH to turn the LED OFF.
- Repeat the sequence continuously.

## Hardware and Software
- AT89S52 Microcontroller
- LED and current-limiting resistor
- 8051 Development Board
- Keil µVision IDE

## Project Files
The task folder contains the Keil project, C source code, startup assembly file, circuit diagram and compiled output files.

## Expected Outcome
The LED blinks continuously at a visible interval, demonstrating GPIO output control and basic delay generation using the 8051 microcontroller.

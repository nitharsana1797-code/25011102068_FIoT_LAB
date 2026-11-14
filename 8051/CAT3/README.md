# CAT 3 – Four LED, Four Mode Controller

## Overview
This practical focuses on GPIO programming and push-button interfacing using the AT89S52 microcontroller. Four LEDs are controlled through different operating modes, with a push-button used to switch between them.

## Experiment
**Four LED, Four Mode Controller**

The system implements four LED patterns:

1. **Running LED:** LEDs glow sequentially from LED1 to LED4.
2. **Binary Counter:** LEDs display a binary counting sequence.
3. **Alternating Pattern:** Two alternating LED patterns are displayed.
4. **All LED Flash:** All four LEDs blink together.

Each valid button press advances the system to the next operating mode.

## Working Principle
The four LEDs are configured as digital outputs, while the push-button acts as a digital input. The controller detects button presses and changes the LED pattern accordingly.

A software debounce mechanism is used to prevent multiple mode transitions from a single button press.

## Hardware and Software
- AT89S52 Microcontroller
- Four LEDs
- Push-button
- Current-limiting resistors
- 8051 Development Board
- Keil µVision IDE

## Expected Outcome
The four LEDs operate in four distinct modes, demonstrating GPIO control, digital input handling, conditional logic and basic switch debouncing.

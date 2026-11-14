# CAT 5 – Timer, PWM and Motor Control

## Overview
This practical explores timer configuration, interrupt-based programming, pulse-width modulation and motor control using the AT89S52 microcontroller.

## Experiments

### TASK 5A – Timer Interrupt-Based Time Base
Configure Timer 0 in 16-bit mode to generate periodic interrupts. The interrupts are used to maintain a software time base and control LED toggling without relying on continuous software delay loops.

### TASK 5B – PWM LED Fading and Square-Wave Generation
Implement software PWM using timer interrupts to control LED brightness. The task also demonstrates square-wave generation through a digital output pin.

### TASK 5C – Stepper Motor Speed and Direction Control
Control a stepper motor using a predefined stepping sequence. Timer-based intervals are used to determine motor stepping speed, while changing the sequence order allows direction control.

## Concepts Covered
- Timer 0 configuration
- Timer interrupts
- Software time-base generation
- Pulse-width modulation (PWM)
- Square-wave generation
- Stepper motor sequencing
- Motor speed and direction control

## Hardware and Software
- AT89S52 Microcontroller
- LEDs
- Stepper Motor
- Suitable Motor Driver
- 8051 Development Board
- Keil µVision IDE

## Expected Outcome
The practical demonstrates the use of hardware timers and interrupts for periodic operations, PWM-based output control and sequential stepper motor operation.

# STM32 GPIO Bring-Up and Switch-Controlled LED (Tasks 8A–8C)

**Board/MCU in supplied project:** Blue Pill, STM32F103C8T6.

## Pin connections
- PA0: push-button input, configured with internal pull-up; connect the switch between PA0 and GND (pressed = LOW).
- PA3: LED output. Confirm the external LED polarity and use a suitable series resistor. The code assumes active-high LED operation (SET = ON).
- Supply the board using its regulated 3.3 V arrangement; do not apply 5 V directly to GPIO pins.

## Source files
- `Task_Source_Codes/TASK8A/main.c` — Task 8A, LED follows the current switch level.
- `Task_Source_Codes/TASK8B/main.c` — Task 8B, debounced press event toggles the LED once.
- `Task_Source_Codes/TASK8C/main.c` — Task 8C, short/long press behavior with non-blocking blinking.
- `Core/Src/main.c` — active project source, set to Task 8C.

## How to run a different task
The Cube project compiles `Core/Src/main.c`. To test Task 8A or 8B, back up `Core/Src/main.c` and copy the desired task's `main.c` from `Task_Source_Codes/TASK8A/` or `TASK8B/` into `Core/Src/main.c`. Build one task at a time; do not add all three `main.c` files to the same build because each defines `main()`.

## Notes
- The existing CubeMX clock configuration and generated GPIO initialization are retained. The `.ioc` file identifies the MCU as STM32F103C8T6 and configures HSE with PLL ×9 (the usual 8 MHz HSE to 72 MHz system clock setup). Verify that the board actually has the configured 8 MHz HSE source.
- PA0 is active-low due to `GPIO_PULLUP`.
- Task 8B and 8C use a 30 ms software debounce interval. Task 8C uses a 1000 ms long-press threshold and 500 ms LED toggle interval.
- The code was prepared from the supplied project files; compile and test it in STM32CubeIDE/Keil with the exact board before hardware submission.

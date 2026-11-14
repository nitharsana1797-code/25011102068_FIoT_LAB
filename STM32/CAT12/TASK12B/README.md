# CAT 11 — Task B — On-board Switch Control with State Remembrance

## Actual board used in the laboratory
**NUCLEO-F410RB — STM32F410RB (Cortex-M4)**

The CAT evaluation PDF names STM32F401RE, but this repository version is intentionally prepared for the **NUCLEO-F410RB** board supplied/used during the experiment, as specified by the student.

## Objective
Each debounced press of B1 (PC13) toggles LD2 (PA5) once. The LED retains its state after the button is released.

## On-board connections
- LD2 user LED: PA5, active HIGH.
- B1 user push button: PC13.
- No external LED or push-button wiring is required for the standard NUCLEO-F410RB user LED/button arrangement. Verify the silkscreen and board revision.

## Clock configuration used
- HSE: 8 MHz external clock input, HSE bypass mode.
- PLL: M = 4, N = 100, P = 2, Q = 4, R = 2.
- SYSCLK / HCLK: 100 MHz.
- APB1: HCLK / 2.
- APB2: HCLK / 1.
- Flash latency: 3 wait states.

This follows the 100 MHz clock configuration shared for the NUCLEO-F410RB experiment. Confirm the HSE source/frequency in CubeMX for the exact board revision before flashing.

## Files
- `main.c` — complete HAL application source for this task.
- `circuit_diagram.png` — reference diagram showing the on-board LED and user button.

## Build instructions
1. Open/create a STM32CubeIDE or Keil project for **STM32F410RB**.
2. Select the correct STM32F410RB device pack, startup file, CMSIS files, and STM32F4 HAL drivers.
3. Configure the clock as described above (or generate the equivalent configuration in CubeMX).
4. Replace the generated project's `main.c` with this task's `main.c`.
5. Build, flash the NUCLEO-F410RB, and test the task.

## Compatibility note
Do not select STM32F401RE just because the CAT PDF names it. This package targets the actual NUCLEO-F410RB used in the experiment. The F410RB and F401RE are different MCU variants; use the matching device selection and startup/HAL configuration.

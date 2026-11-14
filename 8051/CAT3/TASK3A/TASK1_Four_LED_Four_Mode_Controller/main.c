/*
 * TASKD - Four LED, Four Mode Controller (AT89S52)
 * Assumptions to verify against the actual board:
 *   LEDs: P1.0, P1.1, P1.2, P1.3 (active-low)
 *   Button: P3.2 / INT0 (active-low, external pull-up)
 *   Clock: board crystal frequency must be confirmed.
 * Bare-metal register-level GPIO; no HAL/RTOS.
 */
#include <REGX52.H>

#define LED_MASK 0x0F

sbit BUTTON = P3^2;

/* Approximate delay; calibrate for crystal and compiler. */
void delay_ms(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 115; j++)
            ;
}

/* 0=all OFF, 1=running, 2=binary, 3=alternating, 4=all flash */
unsigned char mode = 0;
unsigned char count = 0;

void set_leds(unsigned char pattern)
{
    /* LEDs active-low on assumed P1.0..P1.3; upper P1 bits preserved. */
    P1 = (P1 & 0xF0) | ((~pattern) & LED_MASK);
}

void wait_release(void)
{
    while (BUTTON == 0)
        delay_ms(10);
    delay_ms(25); /* release debounce */
}

void main(void)
{
    unsigned char running_led = 0;
    unsigned char flash_on = 0;

    /* 8051 quasi-bidirectional port: write 1 to input pin. */
    BUTTON = 1;
    P1 = (P1 & 0xF0) | LED_MASK; /* LEDs OFF if active-low */

    while (1)
    {
        if (BUTTON == 0)
        {
            delay_ms(20);             /* debounce press */
            if (BUTTON == 0)
            {
                mode++;
                if (mode > 4) mode = 1;
                wait_release();
                count = 0;
                running_led = 0;
                flash_on = 0;
            }
        }

        switch (mode)
        {
            case 0:
                set_leds(0x00);
                break;

            case 1: /* Running LED: LED1 -> LED2 -> LED3 -> LED4 */
                set_leds((unsigned char)(1U << running_led));
                delay_ms(250);
                running_led = (running_led + 1) & 0x03;
                break;

            case 2: /* Binary counter 0000 through 1111 then 0000 */
                set_leds(count);
                delay_ms(300);
                count = (count + 1) & 0x0F;
                break;

            case 3: /* Alternating 1010 / 0101 */
                set_leds((count & 1) ? 0x05 : 0x0A);
                delay_ms(300);
                count++;
                break;

            case 4: /* All LEDs flash together */
                flash_on = !flash_on;
                set_leds(flash_on ? 0x0F : 0x00);
                delay_ms(300);
                break;

            default:
                mode = 0;
                set_leds(0x00);
                break;
        }
    }
}

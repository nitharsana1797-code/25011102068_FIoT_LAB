/*
 * TASK2 - HMI using Matrix Keypad, LEDs and Seven-Segment Display
 * AT89S52 / Keil C51; direct port access only.
 *
 * ASSUMED wiring (verify/change for your board):
 *   4x4 keypad rows: P2.0-P2.3; columns: P2.4-P2.7
 *   LEDs: P1.0-P1.3, active-low
 *   Seven-segment: P0.0-P0.7 = a,b,c,d,e,f,g,dp (common-cathode)
 *   Common cathode digit enable tied appropriately (single digit).
 * P0 needs external pull-ups on 8051-family boards.
 */
#include <REGX52.H>

#define LED_MASK 0x0F
#define KEYPAD_PORT P2

/* Common-cathode patterns: bit0=a ... bit6=g, bit7=dp */
unsigned char code seg_code[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void delay_ms(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    for (i=0; i<ms; i++)
        for (j=0; j<115; j++)
            ;
}

void display_digit(unsigned char value)
{
    if (value <= 9)
        P0 = seg_code[value];
    else
        P0 = 0x00; /* blank invalid value */
}

void running_led(void);
void alternate_led(void);
void binary_counter(void);
void blink_all(void);

void leds_off(void)
{
    P1 = (P1 & 0xF0) | LED_MASK; /* assumed active-low */
}

void led_control(unsigned char key)
{
    unsigned char i;
    switch (key)
    {
        case 1: P1 = (P1 & 0xF0) | 0x0E; break; /* LED1 ON */
        case 2: P1 = (P1 & 0xF0) | 0x0D; break; /* LED2 ON */
        case 3: P1 = (P1 & 0xF0) | 0x0B; break; /* LED3 ON */
        case 4: P1 = (P1 & 0xF0) | 0x07; break; /* LED4 ON */
        case 5: running_led(); break;
        case 6: alternate_led(); break;
        case 7: binary_counter(); break;
        case 8: blink_all(); break;
        case 9: leds_off(); break;
        case 0: leds_off(); P0 = 0x00; break; /* reset display/system */
        default: break;
    }
}

/* Scan a 4x4 keypad. Returns 0..9 for keys; 0xFF means no supported key.
 * Layout assumed:
 * row 0: 1 2 3 A; row 1: 4 5 6 B; row 2: 7 8 9 C; row 3: * 0 # D
 * Only numeric keys are used by this task.
 */
unsigned char keypad_scan(void)
{
    unsigned char row, col;
    unsigned char keymap[4][4] = {
        {1,2,3,0xFF}, {4,5,6,0xFF}, {7,8,9,0xFF}, {0xFF,0,0xFF,0xFF}
    };

    for (row=0; row<4; row++)
    {
        KEYPAD_PORT = 0xFF;
        KEYPAD_PORT &= (unsigned char)~(1U << row); /* pull one row low */
        delay_ms(1);
        for (col=0; col<4; col++)
        {
            if ((KEYPAD_PORT & (1U << (col+4))) == 0)
            {
                delay_ms(20); /* debounce */
                if ((KEYPAD_PORT & (1U << (col+4))) == 0)
                {
                    while ((KEYPAD_PORT & (1U << (col+4))) == 0)
                        ;
                    delay_ms(20);
                    return keymap[row][col];
                }
            }
        }
    }
    return 0xFF;
}

/* Activity routines; blocking routines temporarily stop keypad scanning. */
void running_led(void)
{
    unsigned char i;
    for (i=0; i<4; i++) {
        P1 = (P1 & 0xF0) | (unsigned char)(LED_MASK ^ (1U<<i));
        delay_ms(250);
    }
    leds_off();
}
void alternate_led(void)
{
    unsigned char i;
    for (i=0; i<4; i++) {
        P1 = (P1 & 0xF0) | ((i&1) ? 0x0A : 0x05); /* alternates 1010/0101 */
        delay_ms(250);
    }
    leds_off();
}
void binary_counter(void)
{
    unsigned char n;
    for (n=0; n<16; n++) {
        P1 = (P1 & 0xF0) | ((~n)&LED_MASK);
        delay_ms(250);
    }
    leds_off();
}
void blink_all(void)
{
    unsigned char i;
    for (i=0; i<4; i++) {
        P1 = (P1 & 0xF0) | 0x00; delay_ms(250); /* all ON active-low */
        P1 = (P1 & 0xF0) | LED_MASK; delay_ms(250);
    }
    leds_off();
}

void main(void)
{
    unsigned char key;
    P1 = (P1 & 0xF0) | LED_MASK;
    P0 = 0x00;
    P2 = 0xFF; /* keypad pins released/high */

    while (1)
    {
        key = keypad_scan();
        if (key != 0xFF)
        {
            display_digit(key);
            led_control(key);
        }
    }
}

#include <reg51.h>

sbit RS = P1^3;
sbit E  = P1^2;

void delay()
{
    int i;
    for(i = 0; i < 1000; i++);
}

void pulse()
{
    E = 1;
    delay();
    E = 0;
}

void lcd_cmd(unsigned char cmd)
{
    RS = 0;

    // Send upper 4 bits: DB7-DB4
    P1 = (P1 & 0x0F) | (cmd & 0xF0);
    pulse();

    // Send lower 4 bits
    P1 = (P1 & 0x0F) | ((cmd << 4) & 0xF0);
    pulse();

    delay();
}

void lcd_data(unsigned char dat)
{
    RS = 1;

    // Send upper 4 bits
    P1 = (P1 & 0x0F) | (dat & 0xF0);
    pulse();

    // Send lower 4 bits
    P1 = (P1 & 0x0F) | ((dat << 4) & 0xF0);
    pulse();

    delay();
}

void lcd_init()
{
    RS = 0;

    // First high nibble: 0010
    P1 = (P1 & 0x0F) | 0x20;
    E = 1;
    E = 0;
    delay();

    // Second high nibble: 0010
   lcd_cmd(0x28);

    // Now normal 4-bit communication can be used
    lcd_cmd(0x0F);   // Display ON, Cursor ON, Blink ON
    lcd_cmd(0x06);   // Entry mode
    lcd_cmd(0x01);   // Clear
}

void main()
{
    lcd_init();

    lcd_data('H');
    lcd_data('I');
		lcd_cmd(0x10);

    while(1);
}
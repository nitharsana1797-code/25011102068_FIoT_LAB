
#include <reg51.h>

sbit RS = P1^3;
sbit E = P1^2;

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

    P1 = (P1 & 0x0F) | (cmd & 0xF0);
    pulse();

    P1 = (P1 & 0x0F) | ((cmd << 4) & 0xF0);
    pulse();

    delay();
}

void lcd_data(unsigned char dat)
{
    RS = 1;

    P1 = (P1 & 0x0F) | (dat & 0xF0);
    pulse();

    P1 = (P1 & 0x0F) | ((dat << 4) & 0xF0);
    pulse();

    delay();
}

void lcd_init()
{
    RS = 0;

    P1 = (P1 & 0x0F) | 0x20;
    E = 1;
    E = 0;
    delay();

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    if(row == 0)
        lcd_cmd(0x80 + col);
    else
        lcd_cmd(0xC0 + col);
}

void lcd_puts(char *str)
{
    while(*str != '\0')
    {
        lcd_data(*str);
        str++;
    }
}

void lcd_print_number(unsigned int value)
{
    unsigned char digit[5];
    int i = 0;

    if(value == 0)
    {
        lcd_data('0');
        return;
    }

    while(value > 0)
    {
        digit[i++] = (value % 10) + '0';
        value /= 10;
    }

    while(i > 0)
    {
        lcd_data(digit[--i]);
    }
}

void main()
{
    unsigned int count = 0;

    lcd_init();

    lcd_goto(0, 0);
    lcd_puts("COUNT:");

    while(1)
    {
        lcd_goto(1, 0);
        lcd_print_number(count);

        lcd_puts("              ");

        count++;

        if(count > 9999)
            count = 0;

        delay();
    }
}

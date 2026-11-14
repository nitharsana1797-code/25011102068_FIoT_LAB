#include <reg51.h>

/*==================================================
                    LCD PINS
==================================================*/

sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;

sbit RS  = P1^3;
sbit EN  = P1^2;


/*==================================================
                  KEYPAD PINS
==================================================*/

/* Rows */
sbit R0 = P0^0;
sbit R1 = P0^1;
sbit R2 = P0^2;
sbit R3 = P0^3;

/* Columns */
sbit C1 = P0^4;
sbit C2 = P0^5;
sbit C3 = P0^6;


/*==================================================
                    VARIABLES
==================================================*/

unsigned char row = 0;
unsigned char col = 0;


/*==================================================
                  DELAY FUNCTION
==================================================*/

void delay(void)
{
    unsigned int i;

    for(i = 0; i < 500; i++);
}


/*==================================================
                INITIALIZE ALL PINS
==================================================*/

void pin_init(void)
{
    /*------------- LCD pins -------------*/

    DB7 = 0;
    DB6 = 0;
    DB5 = 0;
    DB4 = 0;

    RS = 0;
    EN = 0;


    /*------------- Keypad rows -------------*/

    R0 = 1;
    R1 = 1;
    R2 = 1;
    R3 = 1;


    /*------------- Keypad columns -------------*/

    C1 = 1;
    C2 = 1;
    C3 = 1;
}


/*==================================================
                LCD ENABLE PULSE
==================================================*/

void lcd_pulse(void)
{
    EN = 1;

    delay();

    EN = 0;

    delay();
}


/*==================================================
              SEND HIGH NIBBLE
==================================================*/

void lcd_high_nibble(unsigned char value)
{
    DB7 = (value >> 7) & 1;
    DB6 = (value >> 6) & 1;
    DB5 = (value >> 5) & 1;
    DB4 = (value >> 4) & 1;

    lcd_pulse();
}


/*==================================================
               SEND LOW NIBBLE
==================================================*/

void lcd_low_nibble(unsigned char value)
{
    DB7 = (value >> 3) & 1;
    DB6 = (value >> 2) & 1;
    DB5 = (value >> 1) & 1;
    DB4 = value & 1;

    lcd_pulse();
}


/*==================================================
                 LCD COMMAND
==================================================*/

void lcd_cmd(unsigned char cmd)
{
    RS = 0;

    /* Send high 4 bits */
    lcd_high_nibble(cmd);

    /* Send low 4 bits */
    lcd_low_nibble(cmd);
}


/*==================================================
                  LCD DATA
==================================================*/

void lcd_data(unsigned char dat){
    RS = 1;

    /* Send high 4 bits */
    lcd_high_nibble(dat);

    /* Send low 4 bits */
    lcd_low_nibble(dat);

    RS = 0;
}


/*==================================================
                LCD INITIALIZATION
==================================================*/

void lcd_init(void)
{
    RS = 0;
    EN = 0;

    /*
       Send 0010

       This changes LCD from
       8-bit mode to 4-bit mode.
    */

    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 0;

    lcd_pulse();


    /*
       28H = 0010 1000

       DL = 0 -> 4-bit mode
       N  = 1 -> 2 lines
       F  = 0 -> 5x8 font
    */

    lcd_cmd(0x28);


    /*
       0CH

       Display ON
       Cursor OFF
       Blink OFF
    */

    lcd_cmd(0x0C);


    /*
       06H

       Cursor moves right
       after writing data
    */

    lcd_cmd(0x06);


    /*
       01H

       Clear display
    */

    lcd_cmd(0x01);

    delay();
}


/*==================================================
              SET LCD POSITION
==================================================*/

void lcd_position(unsigned char r, unsigned char c)
{
    if(r == 0)
    {
        /*
           First line:

           DDRAM address = 00H + column

           Command = 80H + column
        */

        lcd_cmd(0x80 + c);
    }

    else
    {
        /*
           Second line:

           DDRAM address = 40H + column

           Command = C0H + column
        */

        lcd_cmd(0xC0 + c);
    }
}


/*==================================================
                DISPLAY STAR
==================================================*/

void display_star(void)
{
    lcd_position(row, col);

    lcd_data('*');
}


/*==================================================
                  ERASE STAR
==================================================*/

void erase_star(void)
{
    lcd_position(row, col);

    lcd_data(' ');
}


/*==================================================
                  KEYPAD INIT
==================================================*/

void keypad_init(void)
{
    /*
       Rows are outputs.
       Initially all rows HIGH.
    */

    R0 = 1;
    R1 = 1;
    R2 = 1;
    R3 = 1;


    /*
       Columns are inputs.

       In 8051, writing 1 to a port pin
       allows it to be used as an input.
    */

    C1 = 1;
    C2 = 1;
    C3 = 1;
}


/*==================================================
                  GET KEY
==================================================*/

unsigned char get_key(void)
{
    /*==============================================
                      ROW 0
    ==============================================*/

    R0 = 0;
    R1 = 1;
    R2 = 1;
    R3 = 1;

    if(C1 == 0)
    {
        while(C1 == 0);
        return '*';
    }

    if(C2 == 0)
    {
        while(C2 == 0);
        return '0';
    }

    if(C3 == 0)
    {
        while(C3 == 0);
        return '#';
    }


    /*==============================================
                      ROW 1
    ==============================================*/

    R0 = 1;
    R1 = 0;
    R2 = 1;
    R3 = 1;

    if(C1 == 0)
    {
        while(C1 == 0);
        return '7';
    }

    if(C2 == 0)
    {
        while(C2 == 0);
        return '8';
    }

    if(C3 == 0)
    {
        while(C3 == 0);
        return '9';
    }


    /*==============================================
                      ROW 2
    ==============================================*/

    R0 = 1;
    R1 = 1;
    R2 = 0;
    R3 = 1;

    if(C1 == 0)
    {
        while(C1 == 0);
        return '4';
    }

    if(C2 == 0)
    {
        while(C2 == 0);
        return '5';
    }

    if(C3 == 0)
    {
        while(C3 == 0);
        return '6';
    }


    /*==============================================
                      ROW 3
    ==============================================*/

    R0 = 1;
    R1 = 1;
    R2 = 1;
    R3 = 0;

    if(C1 == 0)
    {
        while(C1 == 0);
        return '1';
    }

    if(C2 == 0)
    {
        while(C2 == 0);
        return '2';
    }

    if(C3 == 0)
    {
        while(C3 == 0);
        return '3';
    }


    /*
       No key pressed
    */

    return 0;
}


/*==================================================
                      MAIN
==================================================*/

void main(void)
{
    unsigned char key;


    /*----------------------------------------------
                INITIALIZE ALL PINS
    ----------------------------------------------*/

    pin_init();


    /*----------------------------------------------
                INITIALIZE KEYPAD
    ----------------------------------------------*/

    keypad_init();


    /*----------------------------------------------
                  INITIALIZE LCD
    ----------------------------------------------*/

    lcd_init();


    /*----------------------------------------------
                INITIAL POSITION
    ----------------------------------------------*/

    row = 0;
    col = 0;

    display_star();


    /*==============================================
                    MAIN LOOP
    ==============================================*/

    while(1)
    {
        key = get_key();


        /*==========================================
                    UP ? 8
        ==========================================*/

        if(key == '2')
        {
            if(row > 0)
            {
                erase_star();

                row--;

                display_star();
            }
        }


        /*==========================================
                    DOWN ? 2
        ==========================================*/

        else if(key == '8')
        {
            if(row < 1)
            {
                erase_star();

                row++;

                display_star();
            }
        }


        /*==========================================
                    LEFT ? 4
        ==========================================*/

        else if(key == '6')
        {
            if(col > 0)
            {
                erase_star();

                col--;

                display_star();
            }
        }


        /*==========================================
                    RIGHT ? 6
        ==========================================*/

        else if(key == '4')
        {
            if(col < 15)
            {
                erase_star();

                col++;

                display_star();
            }
        }
    }
}

#include <REG51.H>

sbit LED = P1^0;

void delay()
{
    int i, j;
    for(i = 0; i < 500; i++)
        for(j = 0; j < 100; j++);
}

void main()
{
    while(1)
    {
        LED = 0;   // LED ON 
        delay();

        LED = 1;   // LED OFF
        delay();
    }
}

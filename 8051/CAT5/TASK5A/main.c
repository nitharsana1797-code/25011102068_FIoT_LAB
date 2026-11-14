/* TASK1: Timer0 interrupt time base, AT89S52, 11.0592 MHz crystal.
 * 12-clock classic 8051: machine cycle = 1.085 us.
 * 1 ms requires ~922 counts; reload FC66 (approximate).
 */
#include <REGX52.H>
volatile unsigned int ms_ticks=0;
volatile bit one_second_flag=0;

void timer0_isr(void) interrupt 1
{
    TH0=0xFC; TL0=0x66;       /* reload for ~1 ms */
    ms_ticks++;
    if(ms_ticks>=1000) {
        ms_ticks=0;
        one_second_flag=1;
    }
}
void timer0_init(void)
{
    TMOD=(TMOD&0xF0)|0x01;    /* Timer0 mode 1, 16-bit */
    TH0=0xFC; TL0=0x66;
    ET0=1; EA=1; TR0=1;
}
void main(void)
{
    P1_0=1;                    /* assumed active-low LED OFF */
    timer0_init();
    while(1) {
        if(one_second_flag) {
            one_second_flag=0;
            P1_0=!P1_0;          /* toggle every second */
        }
    }
}

/* TASK3 (optional): Stepper motor sequence using Timer0 interrupt.
 * Assumed driver inputs connected to P1.0-P1.3.
 * Do NOT connect a motor directly to MCU pins; use a driver (ULN2003/H-bridge).
 * Example 4-step wave sequence; adapt for motor type and driver.
 */
#include <REGX52.H>
volatile unsigned char step_index=0;
volatile unsigned int step_ms=0;
volatile unsigned int step_interval=100; /* slow: 100 ms/step */
volatile bit step_event=0;
unsigned char code step_seq[4]={0x01,0x02,0x04,0x08};

void timer0_isr(void) interrupt 1
{
    TH0=0xFC; TL0=0x66; /* ~1 ms */
    if(++step_ms>=step_interval) { step_ms=0; step_event=1; }
}
void timer0_init(void)
{
    TMOD=(TMOD&0xF0)|0x01; TH0=0xFC; TL0=0x66;
    ET0=1; EA=1; TR0=1;
}
void main(void)
{
    unsigned char direction=1;
    timer0_init();
    while(1) {
        if(step_event) {
            step_event=0;
            P1=(P1&0xF0)|step_seq[step_index];
            if(direction) step_index=(step_index+1)&3;
            else step_index=(step_index+3)&3;
        }
        /* Change step_interval to 100 (slow), 50 (medium), or 10 (fast).
           Change direction to 0 for reverse rotation. */
    }
}

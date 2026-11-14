/* TASK2: Timer0 1-ms interrupt, software PWM + variable square wave.
 * Assumed LED P1.0 active-low; square-wave output P1.1.
 * 1-ms ISR supports 50-Hz PWM (20 phase slots); duty resolution ~5%.
 * For 500 Hz-1 kHz PWM use a faster timer tick and accept lower resolution,
 * or use hardware timer/output circuitry if available.
 */
#include <REGX52.H>
volatile unsigned char pwm_phase=0, duty=0;
volatile unsigned int fade_ms=0, wave_ms=0;
volatile unsigned char fade_up=1, wave_mode=1;
sbit PWM_LED=P1^0;
sbit WAVE_OUT=P1^1;

void timer0_isr(void) interrupt 1
{
    TH0=0xFC; TL0=0x66;
    /* 20 slots per 20-ms period = 50 Hz PWM */
    pwm_phase=(pwm_phase+1)%20;
    PWM_LED = (pwm_phase < (duty/5)) ? 0 : 1; /* active-low LED */
    if(++fade_ms>=100) {
        fade_ms=0;
        if(fade_up) { if(duty>=100) fade_up=0; else duty+=5; }
        else { if(duty==0) fade_up=1; else duty-=5; }
    }
    /* Select square wave: 100, 500, 1000, 2000, 5000 Hz.
       1-ms ISR cannot generate these frequencies; use output toggle
       periods only as a demonstration of lower-frequency selectable waves. */
    if(++wave_ms>=wave_mode) { wave_ms=0; WAVE_OUT=!WAVE_OUT; }
}
void timer0_init(void)
{
    TMOD=(TMOD&0xF0)|0x01; TH0=0xFC; TL0=0x66;
    ET0=1; EA=1; TR0=1;
}
void main(void)
{
    PWM_LED=1; WAVE_OUT=0;
    timer0_init();
    while(1) {
        /* Change wave_mode (in ms) to select output toggle interval.
           For frequency measurement, use an appropriate faster ISR. */
    }
}

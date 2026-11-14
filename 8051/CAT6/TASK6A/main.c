/* CAT5 TASK A: UART initialization and character transmission
 * AT89S52, 11.0592 MHz, 9600 baud, 8-N-1.
 * Keil C51; UART pins: P3.1/TXD (pin 11), P3.0/RXD (pin 10).
 * UART connection to USB-TTL: MCU TXD -> adapter RXD; MCU RXD -> adapter TXD;
 * GNDs common. Use logic-level compatible adapter (NOT RS-232).
 */
#include <REGX52.H>

void uart_init(void)
{
    SCON = 0x50;          /* Mode 1, 8-bit UART, REN=1 */
    TMOD = (TMOD & 0x0F) | 0x20; /* Timer1 mode 2, 8-bit auto-reload */
    PCON &= 0x7F;         /* SMOD=0 */
    TH1 = 0xFD;           /* 9600 baud at 11.0592 MHz */
    TL1 = 0xFD;
    TR1 = 1;              /* Start Timer1 */
    TI = 0;
    RI = 0;
}

void uart_transmit(unsigned char ch)
{
    SBUF = ch;             /* SBUF write starts transmission */
    while (TI == 0);       /* wait for transmit-complete flag */
    TI = 0;                /* clear flag for next character */
}

void uart_string(char *s)
{
    while (*s) uart_transmit(*s++);
}

void main(void)
{
    uart_init();
    while (1)
    {
        uart_string("Hello from 8051\\r\\n");
    /* Deliberately continuous as Task A specifies. */
    /* No software delay is used for UART baud timing. */
    /* A terminal may show repeated lines rapidly. */
    /* Optional pacing can be added only if your faculty asks. */
    /* Keep this task focused on UART TX. */
    ;
    }
}

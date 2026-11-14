/* CAT5 TASK B: Interrupt-driven UART RX ring buffer and echo
 * AT89S52, 11.0592 MHz, 9600 baud, 8-N-1.
 * RX ISR puts incoming bytes into a ring buffer; foreground echoes them.
 */
#include <REGX52.H>

#define RX_BUF_SIZE 32
volatile unsigned char rx_buf[RX_BUF_SIZE];
volatile unsigned char rx_head=0, rx_tail=0;
volatile bit rx_overflow=0;

void uart_init(void)
{
    SCON = 0x50;                 /* mode 1, REN=1 */
    TMOD = (TMOD & 0x0F) | 0x20;/* Timer1 mode 2 */
    PCON &= 0x7F;                /* SMOD=0 */
    TH1 = 0xFD; TL1 = 0xFD;      /* 9600 baud @ 11.0592 MHz */
    TR1 = 1;
    ES = 1; EA = 1;               /* UART interrupt + global interrupt */
    TI = 0; RI = 0;
}

/* Serial interrupt: RI and TI share this vector. */
void serial_isr(void) interrupt 4
{
    unsigned char ch, next;
    if (RI)
    {
        RI = 0;
        ch = SBUF;
        next = (rx_head + 1) % RX_BUF_SIZE;
        if (next != rx_tail) {
            rx_buf[rx_head] = ch;
            rx_head = next;
        } else {
            rx_overflow = 1;      /* buffer full; drop newest byte */
        }
    }
    if (TI) TI = 0;              /* clear TX-complete flag */
}

unsigned char uart_getchar_buffered(void)
{
    unsigned char ch;
    while (rx_head == rx_tail) ;  /* wait until foreground has data */
    ch = rx_buf[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
    return ch;
}

void uart_putchar(unsigned char ch)
{
    ES = 0;                       /* protect TX flag handling */
    SBUF = ch;
    while (TI == 0);
    TI = 0;
    ES = 1;
}

void main(void)
{
    unsigned char ch;
    uart_init();
    while (1)
    {
        ch = uart_getchar_buffered();
        uart_putchar(ch);          /* echo every received character */
    }
}

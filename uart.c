#include <xc.h>
#include "uart.h"

void init_uart(unsigned long baud)
{
    SPEN = 1;
    CREN = 1;
    BRGH = 1;
    SPBRG = (unsigned char)((FOSC / (16UL * baud)) - 1UL);
    RCIE = 0;
}

unsigned char getchar(void)
{
    if (OERR == 1)
    {
        CREN = 0;
        CREN = 1;
    }

    while (RCIF == 0)
    {
        ;
    }

    return RCREG;
}

void putchar(unsigned char data)
{
    while (TXIF == 0)
    {
        ;
    }

    TXREG = data;
}

void puts(const char *s)
{
    while (*s)
    {
        putchar(*s++);
    }
}
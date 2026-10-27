#include<xc.h>
#include "adc.h"

void init_adc()
{
    ADCS0 = 0;
    ADCS1 = 1;
    ADCS2 = 0;
    
    ADON = 1;
    
    ADFM = 1;
    
    PCFG0 = 0;
    PCFG1 = 0;
    PCFG2 = 1;
    PCFG3 = 0;
}

unsigned int read_adc(unsigned char channel)
{
    unsigned int adc = 0;

    ADCON0 = (ADCON0 & 0xC7) | (channel << 3);

    __delay_us(20);

    GO = 1;
    while(GO);

    adc = ((unsigned int)ADRESH << 8) | ADRESL;

    return adc;
}
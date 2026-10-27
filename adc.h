/* 
 * File:   adc.h
 * Author: HP
 *
 * Created on 19 September, 2026, 10:19 PM
 */

#ifndef ADC_H
#define	ADC_H

#define FOSC            20000000
#define _XTAL_FREQ      20000000
void init_adc();
unsigned int read_adc(unsigned char channel);

#endif	/* ADC_H */


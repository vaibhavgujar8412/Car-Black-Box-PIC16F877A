/* 
 * File:   digital_keypad.h
 * Author: HP
 *
 * Created on 16 September, 2026, 11:14 PM
 */

#ifndef DIGITAL_KEYPAD_H
#define	DIGITAL_KEYPAD_H

#define DIGITAL_KEYPAD_DDR          TRISB
#define DIGITAL_KEYPAD              PORTB

#define _XTAL_FREQ                  20000000

#define SW1     0x3E
#define SW2     0x3D
#define SW3     0x3B
#define SW4     0x37
#define SW5     0x2F
#define SW6     0x1F

#define ALL_RELEASED    0x3F

#define LEVEL       0
#define STATE       1

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad();

#endif	/* DIGITAL_KEYPAD_H */


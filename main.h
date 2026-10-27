/* 
 * File:   main.h
 * Author: HP
 *
 * Created on 22 September, 2026, 3:21 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "EEPROM.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "uart.h"
#include "ds1307.h"
#include "adc.h"
#include "dashbord.h"
#include <xc.h>
#define _XTAL_FREQ 20000000

#include "EEPROM.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "uart.h"
#include "ds1307.h"
#include "adc.h"
#include "dashbord.h"
#include <xc.h>
#include "display_main_menu.h"

// EEPROM flag variable for checking the event is present or not
#define LOG_FLAG_ADDR  100

#endif	/* MAIN_H */


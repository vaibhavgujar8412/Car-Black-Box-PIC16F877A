#include <xc.h>
#include "dashbord.h"
#include "EEPROM.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "uart.h"
#include "adc.h"
#include "ds1307.h"
#include "display_main_menu.h"

#pragma config WDTE = OFF
volatile unsigned char screen_flag = HOME_SCREEN;


void init_config(void)
{
    init_clcd();
    init_digital_keypad();
    init_i2c(10000);
    init_uart(9600);
    init_ds1307();
    init_adc();
}

void main(void)
{
    init_config();

    while(1)
    {
        switch(screen_flag)
        {
            case HOME_SCREEN:

                display_screen();

                break;

            case MENU_SCREEN:

                display_main_menu();
                clear_screen();
                screen_flag = HOME_SCREEN;

                break;
        }
    }
}
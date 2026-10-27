#include "digital_keypad.h"
#include <xc.h>

void init_digital_keypad()
{
    DIGITAL_KEYPAD_DDR |= 0x3F;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    unsigned char key;

    key = DIGITAL_KEYPAD & 0x3F;

    if(mode == LEVEL)
    {
        return key;
    }

    if(mode == STATE)
    {
        if(key == ALL_RELEASED)
        {
            __delay_ms(15);
            if((DIGITAL_KEYPAD & 0x3F) == ALL_RELEASED)
            {
                once = 1;
            }
            return ALL_RELEASED;
        }

        if(once)
        {
            __delay_ms(15);
            if((DIGITAL_KEYPAD & 0x3F) == key)
            {
                once = 0;
                return key;
            }
        }
    }

    return ALL_RELEASED;
}
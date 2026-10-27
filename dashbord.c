#include "main.h"

volatile unsigned char clock_reg[3];
const char *gear[] = {"GN", "G1", "G2", "G3", "G4", "G5", "GR", "_C"};
unsigned char i = 0;
unsigned char speed;
unsigned char event_index = 0;
unsigned char once = 1;
extern unsigned char screen_flag;


void display_time(const volatile unsigned char *clock_reg)
{
    char time[9];

    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';

    time[2] = ':';

    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';

    time[5] = ':';

    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';

    time[8] = '\0';

    clcd_print(time, LINE2(0));
}

void get_time(volatile unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);
}

static void display_speed(void)
{
    unsigned int adc;

    adc = read_adc(0);

    speed = (unsigned char)(((unsigned long)adc * 99) / 1023);

    clcd_putch(((speed / 10) % 10) + '0', LINE2(13));
    clcd_putch((speed % 10) + '0', LINE2(14));
}

typedef struct{
    unsigned char data[10];
}Event;

Event event;

void create_event(void)
{
    event.data[0] = ((clock_reg[0] >> 4) & 0x0F) + '0';
    event.data[1] = (clock_reg[0] & 0x0F) + '0';

    event.data[2] = ((clock_reg[1] >> 4) & 0x0F) + '0';
    event.data[3] = (clock_reg[1] & 0x0F) + '0';

    event.data[4] = ((clock_reg[2] >> 4) & 0x0F) + '0';
    event.data[5] = (clock_reg[2] & 0x0F) + '0';

    event.data[6] = gear[i][0];
    event.data[7] = gear[i][1];

    event.data[8] = (speed / 10) + '0';
    event.data[9] = (speed % 10) + '0';
}

static void save_event(void)
{
    unsigned char address;
    unsigned char j;

    address = event_index * 10;

    for(j = 0; j < 10; j++)
        write_external_eeprom(address + j, event.data[j]);

    write_external_eeprom(LOG_FLAG_ADDR, 1);
    
    event_index++;

    if(event_index >= 10)
        event_index = 0;
}

void display_screen(void)
{
    clcd_print("Time", LINE1(2));
    clcd_print("GR", LINE1(9));
    clcd_print("SPD", LINE1(12));

    get_time(clock_reg);
    display_time(clock_reg);
    display_speed();
    
    unsigned char key = read_digital_keypad(STATE);

    if(SW1 == key)
    {
        i = 7;
        create_event();
        save_event();
    }
    else if(SW2 == key)
    {
        if(i == 7)
            i = 0;
        if(i < 6)
            i++;

        create_event();
        save_event();
    }
    else if(SW3 == key)
    {
        if(i > 0)
            i--;

        create_event();
        save_event();
    }
    else if(SW4 == key)
    {
        screen_flag = MENU_SCREEN;
    }

    clcd_print(gear[i], LINE2(9));
}
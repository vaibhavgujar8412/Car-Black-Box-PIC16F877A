#include "main.h"
#define _XTAL_FREQ 20000000

unsigned char password[PASSWORD_LENGTH] = {1, 0, 1, 0};
unsigned char entered_password[PASSWORD_LENGTH];

unsigned char set_hour;
unsigned char set_minute;
unsigned char set_second;

unsigned char set_field;
extern unsigned char clock_reg[3];

///// password menu
static unsigned char check_password(void)
{
    unsigned char i;

    for(i = 0; i < PASSWORD_LENGTH; i++)
    {
        if(password[i] != entered_password[i])
        {
            return 0;
        }
    }

    return 1;
}

void blink_cursor(unsigned char index)
{
    static unsigned int cursor_count = 0;
    static unsigned char cursor_state = 1;

    cursor_count++;

    if (cursor_count >= 50)
    {
        cursor_count = 0;

        cursor_state = !cursor_state;

        if (cursor_state)
            clcd_putch('_', LINE2(index));
        else
            clcd_putch(' ', LINE2(index));
    }
}

static void display_password_entry(void)
{
    unsigned char index = 0;
    unsigned char key;

    clcd_print("ENTER PASSWORD", LINE1(0));
    clcd_print("                ", LINE2(0));

    while(index < PASSWORD_LENGTH)
    {
        key = read_digital_keypad(STATE);
        
        blink_cursor(index);
        
        if(key == SW1)
        {
            entered_password[index] = 1;
            clcd_putch('*', LINE2(index));
            index++;
        }
        else if(key == SW2)
        {
            entered_password[index] = 0;
            clcd_putch('*', LINE2(index));
            index++;
        }
        
    }
}

//////// log functions ///////
/* View logs */

typedef struct
{
    unsigned char read_data[10];
} Event;

Event event1;
Event event2;

/* Read all 10 logs from EEPROM into RAM */
static void read_event(Event *event, unsigned char event_no)
{
    unsigned char j;
    unsigned char address;

    address = event_no * 10;

    for(j = 0; j < 10; j++)
    {
        event->read_data[j] = read_external_eeprom(address + j);
    }
}

static void display_log(Event *event, unsigned char line)
{
    unsigned char display_log[15];

    display_log[0]  = event->read_data[0];
    display_log[1]  = event->read_data[1];
    display_log[2]  = ':';

    display_log[3]  = event->read_data[2];
    display_log[4]  = event->read_data[3];
    display_log[5]  = ':';

    display_log[6]  = event->read_data[4];
    display_log[7]  = event->read_data[5];

    display_log[8]  = ' ';

    display_log[9]  = event->read_data[6];
    display_log[10] = event->read_data[7];

    display_log[11] = ' ';

    display_log[12] = event->read_data[8];
    display_log[13] = event->read_data[9];

    display_log[14] = '\0';

    clcd_print((const char *)display_log, line);
}

static void view_log(void)
{
    unsigned char i = 0;
    unsigned char key;

    if(read_external_eeprom(LOG_FLAG_ADDR) == 0)
    {
        clear_screen();

        clcd_print("NO LOG PRESENT", LINE1(0));
        clcd_print("                ", LINE2(0));

        __delay_ms(1000);

        clear_screen();
        return;
    }
    
    read_event(&event1, i);
    read_event(&event2, i+1);
    
    clear_screen();
    
    display_log(&event1, LINE1(0));
    display_log(&event2, LINE2(0));

    while(1)
    {
        key = read_digital_keypad(STATE);

        /* SW1 -> Forward / Next log */
        if(key == SW1)
        {
            if(i < 8)
            {
                i++;
                
                read_event(&event1, i);
                read_event(&event2, i+1);

                display_log(&event1, LINE1(0));
                display_log(&event2, LINE2(0));
            }
        }

        /* SW2 -> Backward / Previous log */
        else if(key == SW2)
        {
            if(i > 0)
            {
                i--;

                read_event(&event1, i);
                read_event(&event2, i+1);
                
                display_log(&event1, LINE1(0));
                display_log(&event2, LINE2(0));
            }
        }
        else if(key == SW5)
        {
            clear_screen();
            return ;
        }
    }
}

/////// CLEARE LOG ///////

static void clear_log(void)
{
    unsigned char i;

    /* Clear all stored events */
    for(i = 0; i < 100; i++)
    {
        write_external_eeprom(i, 0x00);
    }

    /* No log is present */
    write_external_eeprom(LOG_FLAG_ADDR, 0);

    clear_screen();

    clcd_print("LOG CLEARED", LINE1(0));
    clcd_print("SUCCESSFULLY", LINE2(0));

    __delay_ms(1000);
}

//////// DOWNLOAD LOG //////
static void read_data(unsigned char *log_data, unsigned char i)
{
    unsigned char j;
    unsigned char address;

    address = i * 10;

    for(j = 0; j < 10; j++)
    {
        log_data[j] = read_external_eeprom(address + j);
    }
}

static void download_log_UART(unsigned char *log_data)
{
    unsigned char j;
    char display_log[15];

    display_log[0]  = log_data[0];
    display_log[1]  = log_data[1];
    display_log[2]  = ':';

    display_log[3]  = log_data[2];
    display_log[4]  = log_data[3];
    display_log[5]  = ':';

    display_log[6]  = log_data[4];
    display_log[7]  = log_data[5];

    display_log[8]  = ' ';

    display_log[9]  = log_data[6];
    display_log[10] = log_data[7];

    display_log[11] = ' ';

    display_log[12] = log_data[8];
    display_log[13] = log_data[9];

    display_log[14] = '\0';

    for(j = 0; j < 14; j++)
    {
        putchar(display_log[j]);
    }

    putchar('\r');
    putchar('\n');
}

static void download_log(void)
{
    unsigned char i;
    unsigned char log_data[10];

    /* Check whether any log is present */
    if(read_external_eeprom(LOG_FLAG_ADDR) == 0)
    {
        clear_screen();

        clcd_print("NO LOG PRESENT", LINE1(0));
        clcd_print("                ", LINE2(0));

        __delay_ms(1000);

        return;
    }

    /* Download all events */
    for(i = 0; i < 10; i++)
    {
        read_data(log_data, i);

        download_log_UART(log_data);
    }

    /* Display download status */
    clear_screen();

    clcd_print("LOG DOWNLOADED", LINE1(0));
    clcd_print("SUCCESSFULLY", LINE2(0));

    __delay_ms(1000);
}

////// SET TIME //////

static void display_set_time()
{
    char time[9];

    time[0] = (set_hour / 10) + '0';
    time[1] = (set_hour % 10) + '0';

    time[2] = ':';

    time[3] = (set_minute / 10) + '0';
    time[4] = (set_minute % 10) + '0';

    time[5] = ':';

    time[6] = (set_second / 10) + '0';
    time[7] = (set_second % 10) + '0';

    time[8] = '\0';

    clcd_print(time, LINE2(0));
}
static void increment_field(void)
{
    if(set_field == 0)
    {
        set_hour++;
        if(set_hour > 23)
            set_hour = 0;
    }
    else if(set_field == 1)
    {
        set_minute++;
        if(set_minute > 59)
            set_minute = 0;
    }
    else
    {
        set_second++;
        if(set_second > 59)
            set_second = 0;
    }
}
static void decrement_field(void)
{
    if(set_field == 0)
    {
        if(set_hour == 0)
            set_hour = 23;
        else
            set_hour--;
    }
    else if(set_field == 1)
    {
        if(set_minute == 0)
            set_minute = 59;
        else
            set_minute--;
    }
    else
    {
        if(set_second == 0)
            set_second = 59;
        else
            set_second--;
    }
}
static void change_field(void)
{
    if(set_field == 2)
        set_field = 1;
    else if(set_field == 1)
        set_field = 0;
    else
        set_field = 2;
}
static void update_time(void)
{
    unsigned char hour_bcd;
    unsigned char min_bcd;
    unsigned char sec_bcd;

    hour_bcd = ((set_hour / 10) << 4) | (set_hour % 10);
    min_bcd  = ((set_minute / 10) << 4) | (set_minute % 10);
    sec_bcd  = ((set_second / 10) << 4) | (set_second % 10);

    write_ds1307(HOUR_ADDR, hour_bcd);
    write_ds1307(MIN_ADDR, min_bcd);
    write_ds1307(SEC_ADDR, sec_bcd);
}

static void set_time(void)
{
    unsigned char key;

    get_time(clock_reg);

    //// BCD to Decimal conversion for the chage the time
    set_hour = ((clock_reg[0] >> 4) * 10) +
               (clock_reg[0] & 0x0F);

    set_minute = ((clock_reg[1] >> 4) * 10) +
                 (clock_reg[1] & 0x0F);

    set_second = ((clock_reg[2] >> 4) * 10) +
                 (clock_reg[2] & 0x0F);
    
    set_field = 2;
    clear_screen();
    clcd_print("SET TIME", LINE1(0));

    while(1)
    {
        display_set_time();
        key = read_digital_keypad(STATE);

        if(key == SW1)
        {
            increment_field();
        }
        else if(key == SW2)
        {
            decrement_field();
        }
        else if(key == SW6)
        {
            change_field();
        }
        else if(key == SW3)
        {
            update_time();
            clear_screen();

            clcd_print("TIME UPDATED", LINE1(0));
            clcd_print("SUCCESSFULLY", LINE2(0));

            __delay_ms(1000);
            return;
        }
        else if(key == SW4)
        {
            clear_screen();
            return;
        }
    }
}

/////// BLCK_BOX_MENU ///////
static void black_box_menu(void)
{
   clear_screen();
   unsigned char key;
   const char *menu[] = { "View Log", "Clear Log", "Download Log", "Set Time", "Change Pass" };
   unsigned char i=0;
    while(1)
    {
        //// Display menu /////
        if(i < 4)
        {
            clcd_putch('*', LINE1(0));
            clcd_putch(' ', LINE2(0));

            clcd_print(menu[i], LINE1(2));
            clcd_print(menu[i + 1], LINE2(2));
        }
        else
        {
            clcd_putch(' ', LINE1(0));
            clcd_putch('*', LINE2(0));

            clcd_print(menu[3], LINE1(2));
            clcd_print(menu[4], LINE2(2));
        }

        key = read_digital_keypad(STATE);
        //// SW1 -> move down ///
        if(key == SW1)
        {
            if(i < 4)
            {
                i++;
                clear_screen();
            }
        }

        /* SW2 -> move up */
        else if(key == SW2)
        {
            if(i > 0)
            {
                i--;
                clear_screen();
            }
        }
        
        if(key == SW3)
        {
            if(i == 0)
            {
                clear_screen();
                view_log();
            }
            if(i == 1)
            {
                
                clear_log();
                clear_screen();
            }
            if(i == 2)
            {
                download_log();
                clear_screen();
            }
            if(i == 3)
            {
                set_time();
                clear_screen();
            }
        }
        else if(key == SW5)
        {
            return;
        }
    }
}

void display_main_menu(void)
{
    unsigned char attempt = 0;
    unsigned char result;
    unsigned char sec;

    while(attempt < MAX_ATTEMPTS)
    {
        display_password_entry();

        result = check_password();

        if(result == 1)
        {
            clcd_print("ACCESS GRANTED", LINE1(0));
            clcd_print("WELCOME", LINE2(0));
            __delay_ms(1500);
            black_box_menu();
            clear_screen();

            return;
        }

        attempt++;

        if(attempt < MAX_ATTEMPTS)
        {
            clcd_print("WRONG PASSWORD", LINE1(0));

            if(attempt == 1)
                clcd_print("2 ATTEMPT LEFT", LINE2(0));
            else
                clcd_print("1 ATTEMPT LEFT", LINE2(0));

            __delay_ms(2000);
        }
    }

    clcd_print("YOU ARE BLOCKED", LINE1(0));

    for(sec = 60; sec > 0; sec--)
    {
        clcd_putch((sec / 10) + '0', LINE2(0));
        clcd_putch((sec % 10) + '0', LINE2(1));
        clcd_print(" SEC", LINE2(2));

        __delay_ms(1000);
    }

    clear_screen();
}
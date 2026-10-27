/* 
 * File:   dashbord.h
 * Author: HP
 *
 * Created on 17 September, 2026, 2:29 PM
 */

#ifndef DASHBORD_H
#define	DASHBORD_H

#define HOME_SCREEN       0
#define MENU_SCREEN       1
#define VIEW_LOG          2
#define CLEAR_LOG         3
#define DOWNLOAD_LOG      4
#define SET_TIME          5
#define CHANGE_PASSWORD   6
#define ENTER_PASSWORD    7

void get_time(volatile unsigned char *clock_reg);
void display_time(const volatile unsigned char *clock_reg);

static void display_speed(void);

void display_screen(void);

#endif	/* DASHBORD_H */


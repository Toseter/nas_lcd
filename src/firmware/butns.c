#include <string.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "butns.h"

/* convert int value to string */
inline int ati(const char * s) {
    uint8_t	a;
    unsigned char	sign;
    while(*s == ' ' || *s == '\t')
	s++;
    a = 0;
    sign = 0;
    if(*s == '-') {
	sign++;
	s++;
    } else if(*s == '+')
	s++;
    while((*s <= '9') && (*s >= '0'))
	a = a*10 + (*s++ - '0');
    if(sign)
	return -a;
    return a;
}


/* Function process pressed button. */
void com_bt_hundler(uint8_t* bts_buf, const char *bt_num) {
    char msg[LINE_LENGT] = "Command #";
    
    /* put pressed button info into buffer */
    bts_buf[ati(bt_num)] = 0xFF;

    /* print message */
    strcat(msg, bt_num);
    lcd_clear();
    lcd_goto(0x00);
    lcd_puts(msg);

}

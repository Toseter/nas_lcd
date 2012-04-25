/*
 *  Buttons defenations file/
 *  Describes button connection to 18F4550
 *
 */

#include <htc.h>
#include <stdint.h>

#ifndef _BUTNS_H_
#   define _BUTNS_H_

#define COM_BT_COUNT        13                                                  /* Number of command buttons */

/* Interrupt line 0 (INT0); Connected to LB0 */
/* Button number        PORTB Vaue */
/* Command buttons */
#define BT0_1_PIN       0x09
#define BT0_2_PIN       0x11
#define BT0_3_PIN       0x21
#define BT0_4_PIN       0x41
#define BT0_5_PIN       0x81

#define BT1_1_PIN       0x0A
#define BT1_2_PIN       0x12
#define BT1_3_PIN       0x22
#define BT1_4_PIN       0x42
#define BT1_5_PIN       0x82

#define BT2_1_PIN       0x0C
#define BT2_2_PIN       0x14
#define BT2_3_PIN       0x24
/* Message switch buttons */
#define BT2_4_PIN       0x44
#define BT2_5_PIN       0x84


/* Buttons real number */
#define BT0_1_NUM       "1"
#define BT0_2_NUM       "2"
#define BT0_3_NUM       "3"
#define BT0_4_NUM       "4"
#define BT0_5_NUM       "5"

#define BT1_1_NUM       "6"
#define BT1_2_NUM       "7"
#define BT1_3_NUM       "8"
#define BT1_4_NUM       "9"
#define BT1_5_NUM       "10"

#define BT2_1_NUM       "11"
#define BT2_2_NUM       "12"
#define BT2_3_NUM       "13"


extern void com_bt_hundler(uint8_t *bts_buf, const char *bt_num);

#endif /* _BUTNS_H_ */
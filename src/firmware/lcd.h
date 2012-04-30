#ifndef _LCD_H_
#define _LCD_H_

/*
 *	LCD interface header file
 */

/* 	Defining CHECKBUSY will check if the LCD is busy. The RW bit of the
 * 	LCD must connected to a port of the processor for the check busy
 * 	process to work.
 *
 * 	If CHECKBUSY is not defined it will instead use a delay loop.
 * 	The RW bit of the LCD does not need to connected in this case.
 */

// #define CHECKBUSY	1

#define LCD_WAIT DelayMs(5)


#define MESSAGE_LINE		0x0

#define LCD_RS	LA3
#define LCD_EN	LA1
#define LCD_RW	LA2
#define LCD_DATA	LATD
#define LCD_DATA_PORT	PORTD
#define LCD_RS_TRIS	TRISA3
#define LCD_EN_TRIS	TRISA1
#define LCD_RW_TRIS	TRISA2
#define LCD_DATA_TRIS	TRISD

#define FOURBIT_MODE	0x0
#define EIGHTBIT_MODE	0x1
#define OUTPUT_PIN      0x0
#define INPUT_PIN       0x1
#define OUTPUT_DATA     0x0
#define INPUT_DATA      0x0F

#define LCD_STROBE()	LCD_EN = 1; asm("nop"); asm("nop"); LCD_EN = 0

#define LCD_STROBE_READ(value)	LCD_EN = 1; \
				asm("nop"); asm("nop"); \
				value=LCD_DATA_PORT; \
				LCD_EN = 0;

#define	LCDSetCursor(x)			lcd_cmd(((x)&0x7F)|0x80)
#define LCDClear()			lcd_cmd(0x1)
#define LCDPrintChr(x)			lcd_data(x)
#define LCDGoto(x)			lcd_cmd(0x80+(x));
#define LCD_cursor_right()		lcd_cmd(0x14)
#define LCD_cursor_left()		lcd_cmd(0x10)
#define LCD_display_shift()		lcd_cmd(0x1C)
#define LCDHome()			lcd_cmd(0x2)

extern void LCDCmd(unsigned char);
extern void LCDData(unsigned char);
extern void LCDPrintStr(const char * s);
extern void LCDInit(unsigned char);

#endif

/*
 *	LCD interface example
 *	Uses routines from delay.c
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 or 8 bit mode
 *
 */

#include	<htc.h>
#include	"lcd.h"
#include	"delays.h"


static bit fourbit;		// four or eight bit mode?


/* send a command to the LCD */
void LCDCmd(unsigned char c)
{
	DelayMs(5); // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (fourbit) {
		LCD_DATA = ( ( c >> 4 ) & 0x0F );
		LCD_STROBE();
    		LCD_DATA = ( c & 0x0F );
		LCD_STROBE();
	} else {
		LCD_DATA = c;
		LCD_STROBE();
	}
}

/* send data to the LCD */
void LCDData(unsigned char c) {
	DelayMs(5); // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA = 0;
	LCD_RS = 1;
	if (fourbit) {
    	LCD_DATA |= ( ( c >> 4 ) & 0x0F );
		LCD_STROBE();
		LCD_DATA &= 0xF0;
		LCD_DATA |= ( c & 0x0F );
		LCD_STROBE();
	} else {
		LCD_DATA = c;
		LCD_STROBE();
	}
	LCD_RS = 0;
}

/* write a string of chars to the LCD */

void LCDPrintStr(const char * s) {
	while(*s)
		LCDData(*s++);
}

/* initialize the LCD */
void LCDInit(unsigned char mode) {
	char init_value;

	fourbit		= 0;
	if (mode == FOURBIT_MODE){
		fourbit = 1;
		init_value = 0x3;
	}else{
		init_value = 0x3F;
	}
	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;
	LCD_RS_TRIS	 = OUTPUT_PIN;
	LCD_EN_TRIS	 = OUTPUT_PIN;
	LCD_RW_TRIS	 = OUTPUT_PIN;
	LCD_DATA_TRIS	 = OUTPUT_DATA;
	DelayMs(1);
	LCD_DATA	 = init_value;
	LCD_STROBE();
	DelayMs(1);
	LCD_DATA	 = init_value;
	LCD_STROBE();
	DelayUs(200);
	LCD_DATA	 = init_value;
	LCD_STROBE();

	if (fourbit){
		LCD_WAIT; //may check LCD busy flag, or just delay a little, depending on lcd.h
		LCD_DATA = 0x2; // Set 4-bit mode
		LCD_STROBE();

		LCDCmd(0x28); // Function Set
	}else{
		LCDCmd(0x38);
	}
	LCDCmd(0xF); //Display On, Cursor On, Cursor Blink
	LCDCmd(0x1); //Display Clear
	LCDCmd(0x6); //Entry Mode
	LCDCmd(0x80); //Initialize DDRAM address to zero
}
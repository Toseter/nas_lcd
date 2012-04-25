
/*
 *	LCD interface 
 *	Uses routines from delay.c
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 or 8 bit mode
 *
 *      File token from Hi-Tech PIC18 C Compiler examples.
 */

#include	<htc.h>
#include	"lcd.h"

static bit fourbit;                   /* four or eight bit mode? */

#ifndef _XTAL_FREQ                                                              
#   define _XTAL_FREQ 20000000
#endif

#ifdef CHECKBUSY

unsigned char lcd_read_cmd_nowait(void) {
    unsigned char c, readc;

    LCD_DATA_TRIS	 = INPUT_DATA;

    LCD_RW = 1; // Read LCD
    asm("nop"); // short propagation delay
    asm("nop"); // short propagation delay

    if (fourbit) {
	LCD_STROBE_READ(readc); // Read high nibble
	// Move 4 bits to high nibble while zeroing low nibble
	c = ( ( readc << 4 ) & 0xF0 );
	LCD_STROBE_READ(readc); // Read low nibble
   	c |= ( readc & 0x0F ); // Or in 4 more bits to low nibble
    } else {
	LCD_STROBE_READ(readc);
	c = readc;
    }
    LCD_RW = 0; // Return to default mode of writing LCD
    LCD_DATA_TRIS	 = OUTPUT_DATA; // Return to default mode of writing LCD

    return(c);
}

// Return when the LCD is no longer busy, or we've waiting long enough!
void lcd_check_busy(void) {
    // To avoid hanging forever in event there's a bad or
    // missing LCD on hardware.  Will just run SLOW, but still run.
    unsigned int retry;
    unsigned char c;

    for (retry=1000; retry-- > 0; ) {
    	c = lcd_read_cmd_nowait();
	if (0==(c&0x80)) break; // Check busy bit.  If zero, no longer busy
    }
}
#endif

/* send a command to the LCD */
void lcd_cmd(unsigned char c) {
    LCD_WAIT;
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
void lcd_data(unsigned char c) {
    LCD_WAIT; 

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
void lcd_puts(const char * s) {
    while(*s)
        lcd_data(*s++);
}

/* initialize the LCD */
void lcd_init(unsigned char mode) {
    char init_value;

    fourbit = 0;
    if (mode == FOURBIT_MODE) {
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
    LCD_DATA_TRIS = OUTPUT_DATA;
    __delay_ms(15);
    LCD_DATA	 = init_value;
    LCD_STROBE();
    __delay_ms(25);
    LCD_DATA	 = init_value;
    LCD_STROBE();
    __delay_ms(20);
    LCD_DATA	 = init_value;
    LCD_STROBE();

    if (fourbit){
	LCD_WAIT; 
	LCD_DATA = 0x2;  /* Set 4-bit mode */
	LCD_STROBE();
	lcd_cmd(0x28);   /* Function Set */
    }else{
	lcd_cmd(0x38);
    }
    lcd_cmd(0xE);        /* Display ON, Cursor OFF, Cursor Blink OFF */
    lcd_cmd(0x1);        /* Display Clear */
    lcd_cmd(0x6);        /* Entry Mode */
    lcd_cmd(0x80);       /* Initialize DDRAM address to zero */
}
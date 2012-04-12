/*
 *  NAS_LCD Module Firmware file.
 *  Compiled: HI-TECH PICC18 Compiler
 *  IDE: PikLab IDE
 *  Chip: PIC18F4550
 *  Oscilator Frequency: 20 MHz
 *  Author: ArhiChief
 *  License: GPL
 *
 *  You can use and modify this source, in your own projects.
 */

#include <htc.h>

/*==================================== LCD Library ===========================*/

/*
 * This code will interface to a standard LCD controller
 * like the Hitachi HD44780. It uses it in 4 bit mode, with
 * the hardware connected as follows (the standard 14 pin
 * LCD connector is used):
 *
 * PORTA bits 0-3 are connected to the LCD data bits 4-7 (high nibble)
 * PORTA bit 4 is connected to the LCD RS input (register select)
 * PORTA bit 5 is connected to the LCD EN bit (enable)
 *
 * To use these routines, set up the port I/O (TRISA, TRISD) then
 * call lcd_init(), then other routines as required.
 *
 * Source token from HI-TECH PICC Compiler Examples
 */

#ifndef _XTAL_FREQ
#       define _XTAL_FREQ 20000000
#endif


#define LCD_RS PORTAbits.RA4

#define LCD_EN PORTAbits.RA5

#define LCD_DATA        PORTA

/* Strobe signal to LCD */
#define LCD_STROBE()    ((LCD_EN = 1),(LCD_EN=0));

#define lcd_cursor(x)   lcd_write(((x)&0x7F)|0x80)

/* write a byte to the LCD in 4 bit mode */

void lcd_write(unsigned char c) {
  __delay_us(40);
  LCD_DATA = ( ( c >> 4 ) & 0x0F );
  LCD_STROBE();
  LCD_DATA = ( c & 0x0F );
  LCD_STROBE();
}

/*
 *      Clear and home the LCD
 */

void lcd_clear() {
  LCD_RS = 0;
  lcd_write(0x1);
  __delay_ms(2);
}

/* write a string of chars to the LCD */
void lcd_puts(const char * s) {
  LCD_RS = 1;     // write characters
  while(*s)
    lcd_write(*s++);
}

/* write one character to the LCD */

void lcd_putch(char c) {
  LCD_RS = 1;     // write characters
  lcd_write( c );
}


/*
 * Go to the specified position
 */
void lcd_goto(unsigned char pos)
{
  LCD_RS = 0;
  lcd_write(0x80+pos);
}

/* initialise the LCD - put into 4 bit mode */
void lcd_init(void)
{
  char init_value;
  init_value = 0x3;
  TRISA=0;
  TRISD=0;
  LCD_RS = 0;
  LCD_EN = 0;

        
  __delay_ms(15); // wait 15mSec after power applied
  LCD_DATA         = init_value;
  LCD_STROBE();
  __delay_ms(5);
  LCD_STROBE();
  __delay_us(200);
  LCD_STROBE();
  __delay_us(200);
  LCD_DATA = 2;   // Four bit mode
  LCD_STROBE();

  lcd_write(0x28); // Set interface length
  lcd_write(0xF); // Display On, Cursor On, Cursor Blink
  lcd_clear();    // Clear screen
  lcd_write(0x6); // Set entry Mode
}
/*=========================== END LCD Library ================================*/



/*============================= Maintain Routine =============================*/
int main() {

  while (1) {
  }
}
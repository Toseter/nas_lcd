/*
 *  Main firmware file. Contains, descriptions of firmware workout.
 *
 *  File:   main.c
 *  Author: ArhiChief
 *
 *  Created on March 17, 2012, 11:44 PM
 */



#include <htc.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "butns.h"

#ifndef _XTAL_FREQ
#   define _XTAL_FREQ 20000000
#endif

#define MSG_NUM           5                             /* Number of messages */

/* Configuration bits */
#pragma config CPUDIV = OSC1_PLL2, PLLDIV = 5                     
#pragma config IESO = OFF, FOSC = HSPLL_HS, FCMEN = OFF           
#pragma config VREGEN = ON, PWRT = OFF, BOR = ON, BORV = 2        
#pragma config WDT = OFF, WDTPS = 32768                           
#pragma config PBADEN = ON, LPT1OSC = OFF, MCLRE = ON             
#pragma config STVREN = ON, DEBUG = OFF, ICPRT = OFF, XINST = OFF, LVP = OFF    
#pragma config EBTR0 = OFF, EBTR1 = OFF
#pragma config EBTRB = OFF


char msg_buf[MSG_NUM][LINE_LENGT];                   /* Message buffer */
uint8_t bts_buf[COM_BT_COUNT];                  /* Pressed buttons buf */
int8_t disp_msg = 0;                    /* Number of displayed message */

void next_msg(void) {
    (++disp_msg >= MSG_NUM) ? disp_msg = 0 : disp_msg +=0;
}

void prev_msg(void) {
    (--disp_msg < 0) ? disp_msg = MSG_NUM - 1 : disp_msg +=0;
}

void init(void) {
    /* Configurating ports */
    INTCON  = 0b11010000;
    INTCON2 = 0b01000000; // INTEDG1 - Interrupt on rising edge
    INTCON3 = 0b00000000;
    //          76543210

    ADCON1 = 0b00001110;  // AN0 Analog, everything else digital
    PORTA  = 0b00000000;
    LATA   = 0b00000000;
//    CMCON  = 0b00000111;  // Turn off Comparator
    ADCON0 = 0b00000001;  // A/D Converter module is enabled
    TRISA  = 0b00000001;
    //         76543210

//    SPPCON = 0b00000000;
 //   SPPCFG = 0b00000000;
    //         76543210

    PORTE  = 0b00000000;
    LATE   = 0b00000000;
    TRISE  = 0b00000000;  // Configure PORTE as output
    //         76543210

    PORTB  = 0b00000000;
    TRISB  = 0b00000000;  // PORTB
    //         76543210

    UCON  = 0b00000000;   // Dissable USB
    PORTC = 0b00000000;
    LATC  = 0b00000000;
    TRISC = 0b00000000;   // Configure PORTC as output
    //        76543210

    PORTD = 0b00000000;
    LATD  = 0b00000000;
    TRISD = 0b00000000;   // Configure PORTD as output

    lcd_init(FOURBIT_MODE); /* Initialize LCD module in fourbit mode */
}

/* Program starts here */
void main(void) {
    init();
    lcd_puts(" NAS_LCM MODULE");
    lcd_goto(0x40);                  
    lcd_puts("Im ready to work");
    ei();                            /* enable all interrupts */
        
    while (1) {
        CLRWDT();
    }
}

/* USB*/
void interrupt irq_h(void) {

}

/* Pressed buttons processor */
void interrupt low_priority irq_l(void) {
    __delay_ms (20);
    /* Buttons */
    if (INT0IF || INT1IF || INT2IF) {
        switch (PORTB) {
            /* Command buttons */
            case BT0_1_PIN:
                com_bt_hundler(bts_buf, BT0_1_NUM);
                break;
            case BT0_2_PIN:
                com_bt_hundler(bts_buf, BT0_2_NUM);
                break;
            case BT0_3_PIN:
                com_bt_hundler(bts_buf, BT0_3_NUM);
                break;
            case BT0_4_PIN:
                com_bt_hundler(bts_buf, BT0_4_NUM);
                break;
            case BT0_5_PIN:
                com_bt_hundler(bts_buf, BT0_5_NUM);
                break;

            case BT1_1_PIN:
                com_bt_hundler(bts_buf, BT1_1_NUM);
                break;
            case BT1_2_PIN:
                com_bt_hundler(bts_buf, BT1_2_NUM);
                break;
            case BT1_3_PIN:
                com_bt_hundler(bts_buf, BT1_3_NUM);
                break;
            case BT1_4_PIN:
                com_bt_hundler(bts_buf, BT1_4_NUM);
                break;
            case BT1_5_PIN:
                com_bt_hundler(bts_buf, BT1_5_NUM);
                break;

            case BT2_1_PIN:
                com_bt_hundler(bts_buf, BT2_1_NUM);
                break;
            case BT2_2_PIN:
                com_bt_hundler(bts_buf, BT2_2_NUM);
                break;
            case BT2_3_PIN:
                com_bt_hundler(bts_buf, BT2_3_NUM);
                break;

                /* Message switch buttons */
            case BT2_4_PIN: /* display next message */
                lcd_clear();
                lcd_goto(0x00);
                next_msg();
                lcd_puts(msg_buf[disp_msg]);
                lcd_goto(0x40);
                next_msg();
                lcd_puts(msg_buf[disp_msg]);
                break;
            case BT2_5_PIN: /* display previous message */
                lcd_clear();
                lcd_goto(0x00);
                prev_msg();
                lcd_puts(msg_buf[disp_msg]);
                lcd_goto(0x40);
                prev_msg();
                lcd_puts(msg_buf[disp_msg]);
                break;
        }
        INT0IF = 0;
        INT1IF = 0;
        INT2IF = 0;
    }
}
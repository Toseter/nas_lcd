/*
 * config.h - NAS_LCD configuration file
 *
 * Copyright (C)  2012 ArhiChief <arhichief@gmail.com>
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <htc.h>
#include "lcd.h"
#include "usb.h"

/* Configurating fuses */
#pragma config CONFIG1L = 0x24
#pragma config CONFIG1H = 0x0E
#pragma config CONFIG2L = 0x3F
#pragma config CONFIG2H = 0x1E
#pragma config CONFIG3H = 0x81
#pragma config CONFIG4L = 0x81
#pragma config CONFIG5L = 0x0F
#pragma config CONFIG5H = 0xC0
#pragma config CONFIG6L = 0x0F
#pragma config CONFIG6H = 0xA0
#pragma config CONFIG7L = 0x0F
#pragma config CONFIG7H = 0x40


/* Configurating ports */
void NASinit(void) {
    /* Configurating ports */
    INTCON  = 0xD8;
    INTCON2 = 0x71;
    INTCON3 = 0x18;
    
    ADCON1 = 0x0E;  // AN0 Analog, everything else digital
    PORTA  = 0x00;
    LATA   = 0x00;
    CMCON  = 0x07;  // Turn off Comparator
    ADCON0 = 0x00;  // A/D Converter module is disabled
    TRISA  = 0x001;

    PORTB  = 0x00;
    LATB =   0x00;
    TRISB  = 0x00;
    
    PORTD = 0x00;
    LATD  = 0x00;
    TRISD = 0x00;   // Configure PORTD as output

    LCDInit(FOURBIT_MODE);
    USBInit();
}

#endif _CONFIG_H_
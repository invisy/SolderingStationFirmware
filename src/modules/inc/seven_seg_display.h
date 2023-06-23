#ifndef _SEVEN_SEG_DISPLAY_H_
#define _SEVEN_SEG_DISPLAY_H_

#include "../../core/inc/gpio.h"

#define SYMBOLS_NUMBER 3
#define SEGMENTS_NUMBER 8

typedef struct
{
    GPIO_t* segments[SEGMENTS_NUMBER];
    GPIO_t* registers[SYMBOLS_NUMBER];
    unsigned char symbols[SYMBOLS_NUMBER];
    unsigned char currentRegister;
} SevenSegDisplay_t;

void seven_seg_display_init(SevenSegDisplay_t* display);
void seven_seg_display_update(SevenSegDisplay_t* display);
void seven_seg_display_clear(SevenSegDisplay_t* display);
void seven_seg_display_print_number(SevenSegDisplay_t* display, short number, unsigned char position);

#define seven_seg_display_get_symbol(display, index)  \
    display->symbols[index]

#define seven_seg_display_print_symbol(display, symbol, index)  \
    display->symbols[index] = symbol;

#define A_SYMBOL 0x77
#define C_SYMBOL 0x39
#define d_SYMBOL 0x5e
#define E_SYMBOL 0x79
#define H_SYMBOL 0x76
#define DOT_SYMBOL 0x80
#define DASH_SYMBOL 0x40


#endif
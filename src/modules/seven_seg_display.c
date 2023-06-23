#include "inc/seven_seg_display.h"
#include "../core/inc/millis.h"

static unsigned char digits_dictionary[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7F, 0x6F }; // 0,1,2,3,4,5,6,7,8,9

void seven_seg_display_print_number(SevenSegDisplay_t* display, short number, unsigned char position)
{
    while (number > 0)
    {
        unsigned char digit = number % 10;
        seven_seg_display_print_symbol(display, digits_dictionary[digit], position);
        if(position < 3)
            position++;
        number /= 10;
    }
}

void seven_seg_display_clear(SevenSegDisplay_t* display)
{
    for(int i=0;i < SYMBOLS_NUMBER;i++)
        display->symbols[i] = 0b00000000;
}

void seven_seg_display_update(SevenSegDisplay_t* display)
{
    static unsigned char current_register = 0;

    MILLIS_DELAY(DISPLAY_REGISTERS_UPDATE, 4)
    {
        for(int i=0;i < SYMBOLS_NUMBER;i++)
            gpio_set_low(display->registers[i]);

        for(int i=0;i < SEGMENTS_NUMBER;i++)
            if(display->symbols[current_register] & (1 << i))
                gpio_set_low(display->segments[i]);
            else
                gpio_set_high(display->segments[i]);

        gpio_set_high(display->registers[current_register]);

        if(current_register == (SYMBOLS_NUMBER-1))
            current_register = 0;
        else
            current_register++;
    }
}

void seven_seg_display_init(SevenSegDisplay_t* display)
{
    for(int i=0;i < SYMBOLS_NUMBER;i++)
    {
        gpio_configure_as_output(display->registers[i]);
        display->symbols[i] = 0;
    }

    for(int i=0;i < SEGMENTS_NUMBER;i++)
        gpio_configure_as_output(display->segments[i]);

    display->currentRegister = 0;
}
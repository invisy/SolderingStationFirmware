#include "core/inc/millis.h"
#include "inc/ui.h"

static SevenSegDisplay_t* segmentDisplay;

/*static void print_display_error()
{
    seven_seg_display_clear(segmentDisplay);
    seven_seg_display_print_symbol(segmentDisplay, d_SYMBOL, 1);
    seven_seg_display_print_symbol(segmentDisplay, E_SYMBOL, 0);
}*/

static void ui_print_temperature(unsigned short value)
{
    if(value > 999)
        seven_seg_display_print_number(segmentDisplay, 999, 0);
    else
        seven_seg_display_print_number(segmentDisplay, value, 0);
}

void ui_print_current_temperature(unsigned short value)
{
    seven_seg_display_clear(segmentDisplay);
    ui_print_temperature(value);
}

void ui_print_desired_temperature(unsigned short value)
{
    static char dotEnabled = 0;

    MILLIS_DELAY(PRINT_DOT, 300)
    {
        dotEnabled ^= 1;
    }

    ui_print_temperature(value);
    if(dotEnabled)
    {
        seven_seg_display_print_symbol(segmentDisplay, seven_seg_display_get_symbol(segmentDisplay, 0) | 0x80, 0);  //80 = 0x10000000
    }
    else
    {
        seven_seg_display_print_symbol(segmentDisplay, seven_seg_display_get_symbol(segmentDisplay, 0) & 0x7F, 0);  //7F = 0x01111111
    }
}

void ui_print_err(unsigned short value)
{
    seven_seg_display_clear(segmentDisplay);
    seven_seg_display_print_symbol(segmentDisplay, E_SYMBOL, 1);
    seven_seg_display_print_number(segmentDisplay, value, 2);
}


void ui_update()
{
    seven_seg_display_update(segmentDisplay);
}

void ui_init(SevenSegDisplay_t* display)
{
    segmentDisplay = display;
}
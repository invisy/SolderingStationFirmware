#ifndef _UI_H_
#define _UI_H_

#include "../modules/inc/seven_seg_display.h"

void ui_init(SevenSegDisplay_t* display);
void ui_update();
void ui_print_current_temperature(uint16_t value, uint8_t on_stand);
void ui_print_desired_temperature(uint16_t value);
void ui_print_err(uint16_t value);

#endif
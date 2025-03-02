#ifndef _UI_H_
#define _UI_H_

#include "../modules/inc/seven_seg_display.h"

void ui_init(SevenSegDisplay_t* display);
void ui_update();
void ui_print_current_temperature(unsigned short value, unsigned short on_stand);
void ui_print_desired_temperature(unsigned short value);
void ui_print_err(unsigned short value);

#endif
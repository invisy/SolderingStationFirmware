#ifndef _HEATER_TIMER_H_
#define _HEATER_TIMER_H_

#include <avr/interrupt.h>

typedef void (*heater_timer_callback_t)(); 

void heater_timer_init(heater_timer_callback_t callback_function);
#endif
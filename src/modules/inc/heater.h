#ifndef _HEATER_H_
#define _HEATER_H_

#include "../../core/inc/gpio.h"

#define MAX_HEATERS 8

typedef struct
{
    GPIO_t* heaterGpio;
    volatile bool isEnabled;
    volatile char powerPercentage;
    volatile char currentHalfWave;
} Heater_t;

void heater_init(volatile Heater_t* heaterPtr, GPIO_t* gpio);
void heater_enable(volatile Heater_t* heater);
void heater_disable(volatile Heater_t* heater);
int heater_get_current_power_percentage(volatile Heater_t* heater);
void heater_set_power_percentage(volatile Heater_t* heater, int powerPercentage);
void interrupt();

#endif
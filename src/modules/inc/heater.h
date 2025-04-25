#ifndef _HEATER_H_
#define _HEATER_H_

#include "../../core/inc/gpio.h"

#define MAX_HEATERS 8

typedef struct
{
    GPIO_t* heaterGpio;
    volatile bool isEnabled;
    volatile uint8_t powerPercentage;
    volatile uint8_t currentHalfWave;
} Heater_t;

void heater_init(Heater_t* heaterPtr, GPIO_t* gpio);
void heater_enable(Heater_t* heater);
void heater_disable(Heater_t* heater);
uint8_t heater_get_current_power_percentage(Heater_t* heater);
void heater_set_power_percentage(Heater_t* heater, uint8_t powerPercentage);
void interrupt();

#endif
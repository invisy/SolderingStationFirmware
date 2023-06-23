#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#define TEMP_ERROR 10000

#include "../../core/inc/gpio.h"

typedef struct
{
    GPIO_t* cs;
    GPIO_t* sclk;
    GPIO_t* miso;
} Thermocouple_t;

void thermoCouple_init(volatile Thermocouple_t* thermoCouplePtr, GPIO_t* cs, GPIO_t* sclk, GPIO_t* miso);
uint8_t spiread(volatile Thermocouple_t* thermoCouple);
unsigned short thermoCouple_get_temperature(volatile Thermocouple_t* thermoCouple);

#endif
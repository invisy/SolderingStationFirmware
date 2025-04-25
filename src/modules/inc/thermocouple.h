#ifndef _THERMOCOUPLE_H_
#define _THERMOCOUPLE_H_

#define TEMP_ERROR Q15_from_int(10000)

#include "../../core/inc/gpio.h"
#include "../../core/inc/q15.h"

typedef struct
{
    GPIO_t* cs;
    GPIO_t* sclk;
    GPIO_t* miso;
} Thermocouple_t;

void thermoCouple_init(Thermocouple_t* thermoCouplePtr, GPIO_t* cs, GPIO_t* sclk, GPIO_t* miso);
Q15_t thermoCouple_get_temperature(Thermocouple_t* thermoCouple);

#endif
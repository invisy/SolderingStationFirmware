#ifndef _PID_H_
#define _PID_H_

#include "heater.h"
#include "thermocouple.h"
#include "../../core/inc/gpio.h"

typedef struct 
{
    volatile Thermocouple_t* thermocouple;
    volatile Heater_t* heater;
    volatile float currentTemperature;
    volatile float expectedTemperature;
    volatile float kp;
    volatile float ki;
    volatile float kd;
    volatile float dt;
    volatile float previousError;
    volatile float integral;
} PID_t;

void pid_init(PID_t* pidPtr, float kp, float ki, float kd, float dt, Thermocouple_t* thermocouple, Heater_t* heater);
void pid_process(PID_t* pid);

#endif
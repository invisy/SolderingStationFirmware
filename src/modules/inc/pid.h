#ifndef _PID_H_
#define _PID_H_

#define PID_MAX_OUTPUT Q15_from_int(100)
#define PID_MIN_OUTPUT 0

#include "../../core/inc/gpio.h"
#include "../../core/inc/q15.h"
#include <util/atomic.h>

typedef struct 
{
    volatile Q15_t currentTemperature;
    volatile Q15_t expectedTemperature;
    volatile Q15_t kp;
    volatile Q15_t ki;
    volatile Q15_t kd;
    Q15_t dt;
    Q15_t previousError;
    Q15_t integral;
    Q15_t max_integral;
} PID_t;

void pid_init(PID_t* pidPtr, Q15_t kp, Q15_t ki, Q15_t kd, Q15_t dt);
void pid_update_coefs(PID_t* pidPtr, Q15_t kp, Q15_t ki, Q15_t kd);
uint8_t pid_process(PID_t* pid, Q15_t temp);

#endif
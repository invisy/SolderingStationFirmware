#include "inc/pid.h"


static inline Q15_t calculate_max_integral(Q15_t ki)
{
    return (ki != 0) ? Q15_div(PID_MAX_OUTPUT, ki) : 0;
}


uint8_t pid_process(PID_t* pid, Q15_t temp)
{
    if (pid->dt == 0)
        return 0;

    pid->currentTemperature = temp;

    Q15_t error = (pid->expectedTemperature)-(pid->currentTemperature);           //P

    Q15_t integral_candidate = pid->integral + Q15_mul(error, pid->dt);           //I

    Q15_t differential = Q15_div((error-pid->previousError), pid->dt);            //D
    pid->previousError = error;

    int64_t p = Q15_mul(error, pid->kp);
    int64_t i = Q15_mul(pid->integral, pid->ki);
    int64_t d = Q15_mul(differential, pid->kd);

    int64_t result = p + i + d;

    if (result >= PID_MIN_OUTPUT && result <= PID_MAX_OUTPUT) {
        pid->integral = integral_candidate;
        i = Q15_mul(pid->integral, pid->ki);
        result = p + i + d;                   // recalculate result only after integral accept
    }
    
    result = Q15_clamp(result, PID_MIN_OUTPUT, PID_MAX_OUTPUT);
    
    return (uint8_t)(Q15_to_int(result));
}

void pid_init(PID_t* pidPtr, Q15_t kp, Q15_t ki, Q15_t kd, Q15_t dt)
{
    *pidPtr = (PID_t){
        .currentTemperature = 0,
        .expectedTemperature = 0,
        .kp = kp,
        .ki = ki,
        .kd = kd,
        .dt = dt,
        .previousError = 0,
        .integral = 0,
        .max_integral = calculate_max_integral(ki)
    };
}

void pid_update_coefs(PID_t* pidPtr, Q15_t kp, Q15_t ki, Q15_t kd)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pidPtr->kp = kp;
        pidPtr->ki = ki;
        pidPtr->kd = kd;
        pidPtr->max_integral = calculate_max_integral(ki);
    }
}

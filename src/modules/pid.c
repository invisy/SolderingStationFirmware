#include "inc/pid.h"
#include <util/atomic.h>

void pid_process(PID_t* pid)
{
    unsigned short temp = thermoCouple_get_temperature(pid->thermocouple);
    if(temp == TEMP_ERROR)
    {
        //Overheating protection
        pid->currentTemperature = 0;
        pid->expectedTemperature = 0;
    }
    else
        pid->currentTemperature = temp;

    float error = (pid->expectedTemperature)-(pid->currentTemperature);     //P

    pid->integral += (error * pid->dt);                                     //I

    if(pid->integral < 0)
        pid->integral = 0;
    else if(pid->integral > 100)
        pid->integral= 100;

    float differential = (error-pid->previousError) / (pid->dt);            //D
    pid->previousError = error;

    int result = ((error * pid->kp)+(pid->integral * pid->ki)+(differential * pid->kd));
    
    if(result < 0)
        result = 0;
    else if(result > 100)
        result = 100;

    heater_set_power_percentage(pid->heater, result);
}

void pid_init(PID_t* pidPtr, float kp, float ki, float kd, float dt, Thermocouple_t* thermocouple, Heater_t* heater)
{
    PID_t pid = 
    {
        .thermocouple = thermocouple,
        .heater = heater,
        .currentTemperature = 0,
        .expectedTemperature = 0,
        .kp = kp,
        .ki = ki,
        .kd = kd,
        .dt = dt,
        .previousError = 0,
        .integral = 0
    };
        
    *pidPtr = pid;
    heater_enable(pid.heater);
}
#ifndef _SETTINGS_STORAGE_H_
#define _SETTINGS_STORAGE_H_

#include "../../inc/config.h"
#include "../../core/inc/eeprom.h"

typedef enum
{
    KP = PID_KP_ADDRESS,
    KI = PID_KI_ADDRESS,
    KD = PID_KD_ADDRESS
} PIDCoefficientType;

float get_pid_coefficient(PIDCoefficientType coefficientType, char pidId);
void set_pid_coefficient(PIDCoefficientType coefficientType, char pidId, float value);


#endif
#ifndef _SETTINGS_STORAGE_H_
#define _SETTINGS_STORAGE_H_

#include "../../inc/config.h"
#include "../../core/inc/eeprom.h"
#include "../../core/inc/q15.h"

typedef enum
{
    KP = PID_KP_ADDRESS,
    KI = PID_KI_ADDRESS,
    KD = PID_KD_ADDRESS
} PIDCoefficientType;

Q15_t get_pid_coefficient(PIDCoefficientType coefficientType, uint8_t pidId);
void set_pid_coefficient(PIDCoefficientType coefficientType, uint8_t pidId, Q15_t value);


#endif
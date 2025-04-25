#ifndef _COMMANDS_LIST_H_
#define _COMMANDS_LIST_H_

#include <stdint.h>
#include "../../core/inc/q15.h"

typedef enum
{
    GET_PID_DEVICES_NUMBER = 1,
    GET_PID_CURRENT_TEMPERATURE,
    GET_PID_EXPECTED_TEMPERATURE,
    GET_PID_COEFS,
    SET_PID_EXPECTED_TEMPERATURE,
    SET_PID_KP_COEF = 6,
    SET_PID_KI_COEF = 7,
    SET_PID_KD_COEF = 8
} commands_t;


typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t number;
} get_pid_devices_number_response;


typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
} get_pid_current_temperature_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    Q15_t temperature;
} get_pid_current_temperature_response;


typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
} get_pid_expected_temperature_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    Q15_t temperature;
} get_pid_expected_temperature_response;


typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
} get_pid_coefs_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    Q15_t kp;
    Q15_t ki;
    Q15_t kd;
} get_pid_coefs_response;


typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
    Q15_t temperature;
} set_expected_temperature_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
    Q15_t kp;
} set_pid_kp_coef_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
    Q15_t ki;
} set_pid_ki_coef_request;

typedef struct __attribute__ ((__packed__, aligned(1)))
{
    uint8_t pid_id;
    Q15_t kd;
} set_pid_kd_coef_request;

#endif

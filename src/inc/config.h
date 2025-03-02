#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "../core/inc/gpio.h"

//Parameters

//#define USART_BAUDRATE 9600
#define MAX_HEATER_TEMPERATURE 500
#define PID_PERIOD_MS 500
#define DISPLAY_TEMP_UPDATE_TIME 200
#define PIDS_NUMBER 1
#define BUTTONS_PAUSE 150
#define STAND_PAUSE 1000

//EEPROM

#define PID_KP_ADDRESS 0x0
#define PID_KI_ADDRESS 0x4
#define PID_KD_ADDRESS 0x8
#define PID_SECTION_SIZE 0x12


#endif
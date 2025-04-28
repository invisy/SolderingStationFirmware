#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "serial_communication.h"
#include "../../modules/inc/pid.h"

void command_handler_init(PID_t* pidArray, uint8_t length);
command_response_t handle_command(uint16_t cmd, volatile uint8_t* data, uint8_t data_length);

#endif
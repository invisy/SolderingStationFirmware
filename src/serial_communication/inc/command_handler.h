#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include "serial_communication.h"
#include "../../modules/inc/pid.h"

void command_handler_init(PID_t* pidArray, unsigned char length);
command_response_t handle_command(unsigned short cmd, unsigned char* data, unsigned char data_length);

#endif
#ifndef _SERIAL_COMMUNICATION_H_
#define _SERIAL_COMMUNICATION_H_

#include <stdint.h>

#define BUFFER_LENGTH 255
#define MAX_DATA_LENGTH 250

typedef enum {
    OK = 1,
    WRONG_COMMAND = 2,
    CORRUPTED_PACKET = 3,
    WRONG_ARGUMENTS_LENGTH = 4,
    WRONG_ARGUMENT_VALUE = 5
} RESPONSE_CODE;

typedef struct
{
    RESPONSE_CODE response_code;
    uint8_t data[BUFFER_LENGTH];
    uint8_t data_length;
} command_response_t;

typedef command_response_t(*command_hander_t)(uint16_t,volatile uint8_t*, uint8_t);

typedef enum {
    UNINITIALIZED,
    RECEIVING_HEADER,
    RECEIVING_DATA_AND_CRC,
    TRANSMITTING_RESPONSE
} SERIAL_PROCESS_STATE;

void serial_communication_init(uint32_t usartBaudrate, command_hander_t command_handler);
void serial_write_response(uint8_t response, uint8_t* data, uint8_t data_length);
void serial_process();

#endif
#ifndef _SERIAL_COMMUNICATION_H_
#define _SERIAL_COMMUNICATION_H_

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
    unsigned char data[BUFFER_LENGTH];
    unsigned char data_length;
} command_response_t;

typedef command_response_t(*command_hander_t)(unsigned short, unsigned char*, unsigned char);

typedef enum {
    UNINITIALIZED,
    RECEIVING_HEADER,
    RECEIVING_DATA_AND_CRC,
    TRANSMITTING_RESPONSE
} SERIAL_PROCESS_STATE;

void serial_communication_init(unsigned int usartBaudrate, command_hander_t command_handler);
void serial_write_response(unsigned char response, unsigned char* data, unsigned char data_length);
void serial_process();

#endif
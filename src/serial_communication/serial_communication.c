#include "../core/inc/millis.h"
#include "../core/inc/usart.h"
#include "inc/serial_communication.h"
#include "inc/serial_crc.h"
#include <util/atomic.h>

static uint32_t timeout = 2;
static uint8_t buffer[BUFFER_LENGTH];
static uint8_t buffer_counter = 0;
static uint8_t transmitting_counter = 0;
static uint32_t last_buffer_counter_update_time = 0;

static SERIAL_PROCESS_STATE state = UNINITIALIZED;
static command_hander_t Command_handler;

void serial_transmit_callback()
{
    if(transmitting_counter < buffer_counter)
    {
        usart_send_byte(buffer[transmitting_counter]);
        transmitting_counter++;
    }
    else
    {
        usart_transmit_byte_interrupt_disable();
        buffer_counter = 0;
        transmitting_counter = 0;
        last_buffer_counter_update_time = 0;
        state = RECEIVING_HEADER;
        usart_receive_byte_interrupt_enable();
    }

}

void serial_receive_callback(uint8_t symbol)
{
    last_buffer_counter_update_time = millis();
    buffer[buffer_counter] = symbol;
    buffer_counter++;

    if(buffer_counter == 255)
        usart_receive_byte_interrupt_disable();
}

void serial_process()
{
    static uint8_t packet_length = 0;
    static uint8_t data_length = 0;

    switch(state)
    {
        case RECEIVING_HEADER:
        {
            if(buffer_counter >= 3 && buffer[2] <= MAX_DATA_LENGTH)
            {
                data_length = buffer[2];
                packet_length = data_length + 5;
                state = RECEIVING_DATA_AND_CRC;
            }
            else if(buffer_counter != 0 && millis_timeout_check(&last_buffer_counter_update_time, timeout))
            {
                usart_receive_byte_interrupt_disable();
                serial_write_response(CORRUPTED_PACKET, 0, 0);
                state = TRANSMITTING_RESPONSE;
                usart_transmit_byte_interrupt_enable();
            }

            break; 
        }
        case RECEIVING_DATA_AND_CRC:
        {
            if(buffer_counter >= packet_length)
            {
                usart_receive_byte_interrupt_disable();
                uint16_t caluclated_crc = CRC16(buffer, packet_length-2);
                uint16_t received_crc = buffer[buffer_counter-2] | (buffer[buffer_counter-1] << 8);
                if(caluclated_crc == received_crc)
                {
                    uint16_t cmd = buffer[0] | (buffer[1] << 8);
                    command_response_t result = Command_handler(cmd, &buffer[3], data_length);
                    serial_write_response(result.response_code, result.data, result.data_length);
                }
                else
                {
                    serial_write_response(CORRUPTED_PACKET, 0, 0);
                }
                state = TRANSMITTING_RESPONSE;
                usart_transmit_byte_interrupt_enable();
            }
            else if(millis_timeout_check(&last_buffer_counter_update_time, timeout))
            {
                usart_receive_byte_interrupt_disable();
                serial_write_response(CORRUPTED_PACKET, 0, 0);
                state = TRANSMITTING_RESPONSE;
                usart_transmit_byte_interrupt_enable();
            }
            break;
        }
        case TRANSMITTING_RESPONSE:
        case UNINITIALIZED:
            break;
    }
}

void serial_write_response(uint8_t response, uint8_t* data, uint8_t data_length)
{
    uint8_t packet_length = data_length + 4;
    buffer[0] = response;
    buffer[1] = data_length;

    for(uint8_t i = 0; i < (data_length); i++)
        buffer[i+2] = data[i];

    uint16_t caluclated_crc = CRC16(buffer, (packet_length-2));
    buffer[packet_length-2] = caluclated_crc & 0xff;
    buffer[packet_length-1] = (caluclated_crc >> 8) & 0xff;

    buffer_counter = packet_length;
}

void serial_communication_init(uint32_t usartBaudrate, command_hander_t command_handler)
{
    transmit_callback_t transmit_callback = (transmit_callback_t)serial_transmit_callback;
    receive_callback_t receive_callback = (receive_callback_t)serial_receive_callback;
    Command_handler = command_handler;

    // Calculate UART receive timeout in milliseconds based on the time required to receive ~4 bytes.
    // Ensures the timeout is never less than 2 ms. All operations are integer-based for performance.
    uint32_t bytesPerSec = usartBaudrate / 8;
    timeout = (4000 + bytesPerSec - 1) / bytesPerSec;
    if(timeout < 2)
        timeout = 2;

    usart_init(usartBaudrate, transmit_callback, receive_callback);

    state = RECEIVING_HEADER;
    buffer_counter = 0;
    usart_receive_byte_interrupt_enable();
}

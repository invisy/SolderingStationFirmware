#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>

typedef void*(*transmit_callback_t)();
typedef void*(*receive_callback_t)(uint8_t symbol);

void usart_init(uint16_t usartBaudrate, transmit_callback_t transmit_callback, receive_callback_t receive_callback);
void usart_receive_byte_interrupt_enable();
void usart_receive_byte_interrupt_disable();
void usart_transmit_byte_interrupt_enable();
void usart_transmit_byte_interrupt_disable();
void usart_send_byte(uint8_t byte);

#endif
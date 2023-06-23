#ifndef _USART_H_
#define _USART_H_

typedef void*(*transmit_callback_t)();
typedef void*(*receive_callback_t)(char symbol);

void usart_init(unsigned int usartBaudrate, transmit_callback_t transmit_callback, receive_callback_t receive_callback);
void usart_receive_byte_interrupt_enable();
void usart_receive_byte_interrupt_disable();
void usart_transmit_byte_interrupt_enable();
void usart_transmit_byte_interrupt_disable();
void usart_send_byte(unsigned char byte);

#endif
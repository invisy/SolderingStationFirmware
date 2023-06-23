#include "inc/usart.h"
#include <avr/interrupt.h>

static transmit_callback_t Transmit_callback;
static receive_callback_t Receive_callback;

void usart_init(unsigned int usartBaudrate, transmit_callback_t transmit_callback, receive_callback_t receive_callback)
{
    cli();                       
    UCSRA = (1 << U2X);                                                             /* U2X0 (double speed) and iterrupts are enabled */
    UCSRC = (1 << UCSZ1) | (1 << UCSZ0);                                            /* 8 data bits, no parity, 1 stop bit */

    char u2x0BitMultiplier = (char)((UCSRA & (1<<U2X)) >> U2X) + 1;
    unsigned int baudPrescale = ((F_CPU/usartBaudrate/16)*u2x0BitMultiplier)-1;     /* F_CPU/(16*usartBaudrate) U2X=0;  F_CPU/(8*usartBaudrate) U2X=1*/
    UBRRH = (unsigned char)(baudPrescale >> 8);	                                    /* Load upper 8-bits of the baud rate */
    UBRRL = (unsigned char)baudPrescale;	                                        /* Load lower 8-bits */

    UCSRB = (1 << RXEN) | (1 << TXEN);	                                            /* Turn on transmission and reception*/

    Transmit_callback = transmit_callback;
    Receive_callback = receive_callback;
    sei();
}

void usart_receive_byte_interrupt_enable()
{
    UCSRB |= (1<<RXCIE);
}

void usart_receive_byte_interrupt_disable()
{
    UCSRB &= ~(1<<RXCIE);
}

void usart_transmit_byte_interrupt_enable()
{
    UCSRB |= (1<<UDRIE);
}

void usart_transmit_byte_interrupt_disable()
{
    UCSRB &= ~(1<<UDRIE);
}

void usart_send_byte(unsigned char byte)
{
    UDR = byte;
}

ISR(USART_UDRE_vect)
{
    Transmit_callback();
}

ISR(USART_RXC_vect)
{
    char receivedByte = UDR;
	Receive_callback(receivedByte);
}
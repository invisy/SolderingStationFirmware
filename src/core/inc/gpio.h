#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdbool.h>
#include <avr/io.h>

typedef struct 
{
	volatile uint8_t* ddr;
	volatile uint8_t* port;
	volatile uint8_t* pin;
	uint8_t number;
} GPIO_t;

#define SETUP_PIN(Name, Port, Pin) 	\
	GPIO_t Name = 					\
	{								\
		.ddr = &DDR##Port,			\
		.port = &PORT##Port,		\
		.pin = &PIN##Port,			\
		.number = P##Port##Pin  	\
	}						 		\

#endif

/***** Configure IO *****/
void gpio_configure_as_input(GPIO_t* gpio);
void gpio_configure_as_output(GPIO_t* gpio);
void gpio_pullup_on(GPIO_t* gpio);
void gpio_pullup_off(GPIO_t* gpio);
//void gpio_enable_digital_input(GPIO_t* gpio);
//void gpio_isable_digital_input(GPIO_t* gpio);

/***** Manipulate Outputs *****/
void gpio_set_high(GPIO_t* gpio);
void gpio_set_low(GPIO_t* gpio);
void gpio_toggle(GPIO_t* gpio);

/***** Test Inputs *****/
bool gpio_is_high(GPIO_t* gpio);
bool gpio_is_low(GPIO_t* gpio);

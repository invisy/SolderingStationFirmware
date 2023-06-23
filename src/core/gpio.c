#include "inc/gpio.h"

/***** Configure IO *****/
void gpio_configure_as_input(GPIO_t* gpio)
{
    *gpio->ddr &= ~(1 << gpio->number);
}

void gpio_configure_as_output(GPIO_t* gpio)
{
    *gpio->ddr |= (1 << gpio->number);
}

void gpio_pullup_on(GPIO_t* gpio)
{
    *gpio->port |= (1 << gpio->number);
}

void gpio_pullup_off(GPIO_t* gpio)
{
    *gpio->port &= ~(1 << gpio->number);
}

/***** Manipulate Outputs *****/

void gpio_set_high(GPIO_t* gpio)
{
    *gpio->port |= (1 << gpio->number);
}

void gpio_set_low(GPIO_t* gpio)
{
    *gpio->port &= ~(1 << gpio->number);
}

void gpio_toggle(GPIO_t* gpio)
{
    *gpio->pin |= (1 << gpio->number);
}

/***** Test Inputs *****/

bool gpio_is_high(GPIO_t* gpio)
{
    return (*gpio->pin & (1 << gpio->number));
}

bool gpio_is_low(GPIO_t* gpio)
{
    return !gpio_is_high(gpio);
}
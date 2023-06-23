#include "inc/thermocouple.h"
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned short thermoCouple_get_temperature(volatile Thermocouple_t* thermoCouple)
{
    uint16_t v;

    gpio_set_low(thermoCouple->cs);
    _delay_us(10);

    v = spiread(thermoCouple);
    v <<= 8;
    v |= spiread(thermoCouple);

    gpio_set_high(thermoCouple->cs);

    if (v & 0x4) {
        // uh oh, no thermocouple attached!
        return TEMP_ERROR;
    }

    v >>= 3;

    return v/4;
}

uint8_t spiread(volatile Thermocouple_t* thermoCouple) {
    int i;
    uint8_t d = 0;

    for (i = 7; i >= 0; i--) {
        gpio_set_low(thermoCouple->sclk);
        _delay_us(10);
        if (gpio_is_high(thermoCouple->miso)) {
            // set the bit to 0 no matter what
            d |= (1 << i);
        }

        gpio_set_high(thermoCouple->sclk);
        _delay_us(10);
    }

    return d;
}

void thermoCouple_init(volatile Thermocouple_t* thermoCouplePtr, GPIO_t* cs, GPIO_t* sclk, GPIO_t* miso)
{
    Thermocouple_t thermoCouple = {
        .cs = cs,
        .sclk = sclk,
        .miso = miso
    };

    *thermoCouplePtr = thermoCouple;

    gpio_configure_as_output(thermoCouplePtr->cs);
    gpio_configure_as_output(thermoCouplePtr->sclk);
    gpio_configure_as_input(thermoCouplePtr->miso);

    gpio_set_high(thermoCouplePtr->cs);
    _delay_ms(500);
}
#include "inc/thermocouple.h"
#include <avr/interrupt.h>
#include <util/delay.h>


uint8_t spiread(Thermocouple_t* thermoCouple) {
    int8_t i;
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

Q15_t thermoCouple_get_temperature(Thermocouple_t* thermoCouple)
{
    uint32_t v;

    gpio_set_low(thermoCouple->cs);
    _delay_us(10);

    v = spiread(thermoCouple);
    v <<= 8;
    v |= spiread(thermoCouple);

    gpio_set_high(thermoCouple->cs);
    _delay_us(10);

    if (v & 0x4) {
        // uh oh, no thermocouple attached!
        return TEMP_ERROR;
    }

    v >>= 3;

    // v*0.25 -> Q15_t
    return (Q15_t)(Q15_div(Q15_from_int(v), Q15_from_int(4)));
}

void thermoCouple_init(Thermocouple_t* thermoCouplePtr, GPIO_t* cs, GPIO_t* sclk, GPIO_t* miso)
{
    thermoCouplePtr->cs = cs;
    thermoCouplePtr->sclk = sclk;
    thermoCouplePtr->miso = miso;

    gpio_configure_as_output(thermoCouplePtr->cs);
    gpio_configure_as_output(thermoCouplePtr->sclk);
    gpio_configure_as_input(thermoCouplePtr->miso);

    gpio_set_high(thermoCouplePtr->cs);
    _delay_ms(500);
}

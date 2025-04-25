#include <avr/interrupt.h>
#include "inc/millis.h"
#include <util/atomic.h>

static volatile uint32_t counter = 0;

static bool configure_timer()
{
    cli();

    #ifdef __AVR_ATmega328P__

    TCNT0 = 0;                                  // Clear TCNT0
    OCR0A = F_CPU/1000/64-1;                    // 1000 Hz (F_CPU/((OCR0A+1)*64))
    TCCR0A = (1 << WGM01);                      // CTC
    TCCR0B = (1 << CS01) | (1 << CS00);         // Prescaler 64
    TIMSK0 = (1 << OCIE0A);                     // Output Compare Match A Interrupt Enable

    #elif defined __AVR_ATmega8__

    TCNT2 = 0;                                  // Clear TCNT2
    OCR2 = F_CPU/1000/64-1;                     // 1000 Hz (F_CPU/((OCR2+1)*64))
    TCCR2 = (1 << WGM21) | (1 << CS22);         // CTC & Prescaler 64
    TIMSK |= (1 << OCIE2);                       // Output Compare Match A Interrupt Enable
    
    #endif

    sei();

    return 1;
}

uint32_t millis()
{
    uint32_t result = 0;

    static bool firstInit = 1;
    if(firstInit)
    {
        configure_timer();
        firstInit = 0;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        result = counter;
    }

    return result;
}

bool millis_timeout_check(uint32_t* previousMillis, uint32_t intervalMs)
{
    uint32_t currentMillis = millis();
    if ((uint32_t)(currentMillis - *previousMillis) >= intervalMs)
    {
        *previousMillis = currentMillis;
        return 1;
    }

    return 0;
}

#ifdef __AVR_ATmega328P__
ISR(TIMER0_COMPA_vect) {
#elif defined __AVR_ATmega8__
ISR(TIMER2_COMP_vect) {
#endif
    counter++;
}
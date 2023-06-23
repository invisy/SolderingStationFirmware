#include "inc/heater_timer.h"

static heater_timer_callback_t callback;

void heater_timer_init(heater_timer_callback_t callback_function)
{
    cli();
#ifdef __AVR_ATmega328P__
    TCNT2 = 0;                                  // Clear TCNT0
    OCR2A = F_CPU/100/1024-1;                   // 100 Hz (F_CPU/((OCR2A+1)*1024))
    TCCR2A = (1 << WGM21);                      // CTC
    TCCR2B = (1 << CS22) | (1 << CS20);         // Prescaler 1024
    TIMSK2 = (1 << OCIE2A);                     // Output Compare Match A Interrupt Enable
#elif defined __AVR_ATmega8__
    TCNT1 = 0;                                          // Clear TCNT0
    OCR1A = F_CPU/100/64-1;                             // 100 Hz (F_CPU/((OCR0A+1)*64))
    TCCR1A = 0;                                         // CTC
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC & Prescaler 64
    TIMSK |= (1 << OCIE1A);                             // Output Compare Match A Interrupt Enable
#endif

    callback = callback_function;
    sei();
}

#ifdef __AVR_ATmega328P__
ISR(TIMER2_COMPA_vect) {
#elif defined __AVR_ATmega8__
ISR(TIMER1_COMPA_vect) {
#endif
    callback();
}
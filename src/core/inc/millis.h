#ifndef _MILLIS_H_
#define _MILLIS_H_
#include <stdbool.h>
#include <stdint.h>

uint32_t millis();
bool millis_timeout_check(uint32_t* previousMillis, uint32_t intervalMs);

// Example: MILLIS_DELAY(Button1, 1000) { read_button(); }
#define MILLIS_DELAY(uniqueName, intervalMs)                                                             \
    static uint32_t __millis_prev_##uniqueName = 0;                                                \
    if(millis_timeout_check(&__millis_prev_##uniqueName, intervalMs))

#endif
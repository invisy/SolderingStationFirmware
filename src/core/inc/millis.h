#ifndef _MILLIS_H_
#define _MILLIS_H_
#include <stdbool.h>

unsigned long millis();
bool millis_timeout_check(unsigned long* previousMillis, int intervalMs);

// Example: MILLIS_DELAY(Button1, 1000) { read_button(); }
#define MILLIS_DELAY(uniqueName, intervalMs)                                                             \
    static unsigned long previousMillis_##uniqueName = 0;                                                \
    if(millis_timeout_check(&previousMillis_##uniqueName, intervalMs))

#endif
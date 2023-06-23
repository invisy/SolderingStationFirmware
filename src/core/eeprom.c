#include "inc/eeprom.h"
#include <avr/eeprom.h>

float storage_read_float(float* address)
{
    return eeprom_read_float(address);
}

void storage_write_float(float* address, float value)
{
    eeprom_write_float(address, value);
}

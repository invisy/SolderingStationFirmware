#include "inc/eeprom.h"
#include <avr/eeprom.h>

int32_t storage_read_dword(uint32_t* address)
{
    return eeprom_read_dword(address);
}

void storage_write_dword(uint32_t* address, int32_t value)
{
    eeprom_write_dword(address, value);
}

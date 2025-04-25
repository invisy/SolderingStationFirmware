#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <stdint.h>

int32_t storage_read_dword(uint32_t* address);
void storage_write_dword(uint32_t* address, int32_t value);

#endif
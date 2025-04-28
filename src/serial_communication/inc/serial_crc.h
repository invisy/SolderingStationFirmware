#ifndef _MB_CRC_H
#define _MB_CRC_H

#include <stdint.h>
#include <avr/pgmspace.h>

uint16_t CRC16(volatile uint8_t *pucFrame, uint16_t usLen);

#endif
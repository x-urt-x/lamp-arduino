#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <Arduino.h>
#include <avr/pgmspace.h>

extern uint8_t inoise8(uint16_t x, uint16_t y, uint16_t z);
extern int8_t inoise8_raw(uint16_t x, uint16_t y, uint16_t z);
#endif
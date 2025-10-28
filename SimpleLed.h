#ifndef SIMPLE_LED
#define SIMPLE_LED

#include "Structures.h"

class SimpleLed
{
public:
	SimpleLed(uint8_t pin, uint32_t freq, uint16_t range);
	void parse(const char* input_str);
	void begin();

	int get_br();
	void set_br(int br);
private:
	uint16_t _br;
	uint8_t _pin;
	uint32_t _freq;
	uint16_t _range;
};

#endif
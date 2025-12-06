#include "SimpleLed.h"

SimpleLed::SimpleLed(uint8_t pin, uint32_t freq, uint16_t range)
	:_pin(pin), _freq(freq), _range(range)
{}

void SimpleLed::parse(const char* input_str)
{
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 'b':
	{
		int br;
		parseIn_int(br);
		set_br(br);
	}
	break;
	default:
		break;
	}
}

void SimpleLed::begin()
{
	pinMode(_pin, OUTPUT);
	analogWriteFreq(_freq);
	analogWriteRange(_range);
}

int SimpleLed::get_br()
{
	return _br;
}

void SimpleLed::set_br(int br)
{
	if (br < 0) br = 0;
	if (br > PWM_RANGE) br = PWM_RANGE;
	analogWrite(_pin, br);
	_br = br;
}

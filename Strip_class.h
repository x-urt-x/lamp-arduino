#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H

#include <Adafruit_NeoPixel.h>
#include "log.h"
#include "Structures.h"

static const uint8_t cutoff_order[] = { 0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,40,50,49,59,41,51,48,58,42,52,47,57,43,53,46,56};
static const uint8_t cutoff_imm[] = { 44,45,54,55 };

class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6); //n=100
	void apply_br();
	void eff_singleColor();
  void eff_halfSingleColor();
	void set_br(int br);
	void set_main_color(Color_str color);
	float _br_max_gain;
	int _br_cutoff_bound;

private:
	uint8* _pixels;
	byte _led_amount;
	byte _led_active_count; //count of active leds on each frame
	Color_str _main_color, _second_color;
	Color_str* _leds_arr;
	int _br_vir; //virtual brightness up to 255*97=24735
};
#endif



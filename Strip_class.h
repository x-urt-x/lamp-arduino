#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H


//effects numbers
#define EFF_SINGLE 0
#define EFF_FIRE 1

#include <Adafruit_NeoPixel.h>
#include "log.h"
#include "IEffect.h"
#include "Structures.h"

class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6); //n=100
	void apply_br();
	void set_br(int br);
	void set_main_color(Color_str color);
	float _br_max_gain;
	int _br_cutoff_bound;
  void set_effect(byte num);
  void frame();
  
private:
  IEffect* effect;

  const uint8_t* _cur_cutoff_order;
  const uint8_t*  _cur_cutoff_imm;
  byte _cur_cutoff_order_len, _cur_cutoff_imm_len, _cur_cutoff_units;

	uint8* _pixels; 
	byte _led_amount;
	Color_str _main_color, _second_color;
	Color_str* _leds_arr; //array of leds color
	int _br_vir; //virtual brightness up to 255*97=24735
};

#endif



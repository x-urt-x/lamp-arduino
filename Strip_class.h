#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H

#include <Adafruit_NeoPixel.h>
#include "log.h"
#include "Structures.h"
#include "Effect_bases.h"
#include "IEffect.h"

class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6); //n=100

	void begin();

	int get_preset_count();
	const String* get_preset_names();
	String get_status();
	void set_br(int br);
	void set_effect(byte num);
	void tick(bool now = false);

	//Effectable
	void set_effect_strip_update_delay_time(uint delay);
	void set_effect_br_cutoff_bound(int br_cutoff_bound);
	void set_effect_step(uint step);
	void apply_default_option();
	//Colorable
	void set_color(Color_str color, int num);
	//Preseteble
	void set_preset(int num);
	//Rainbowble
	void set_rainbow_state(bool state, int num);
	void set_rainbow_step(int step, int num);
private:
	IEffect* effect;
	void apply_br();

	unsigned long _strip_update_prev_time = 0;
	unsigned long _strip_update_cur_time = 0;
	float _br_max_gain;
	
	Cutoff_str* _cutoff_option;
	byte _cur_cutoff_units;

	int _strip_update_delay_time = 0;

	uint8* _pixels;
	byte _led_amount;
	Color_str* _leds_arr; //array of leds color
	int _br_vir;
};
#endif
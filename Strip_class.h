#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H

//effects numbers
#define EFF_SINGLE 0
#define EFF_FIRE 1

#include <Adafruit_NeoPixel.h>
#include "log.h"
#include "Structures.h"
#include "IEffect.h"

class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6); //n=100

	int get_preset_count();
	const String* get_preset_names();
	String get_status();
	void set_br(int br);
	void set_main_color(Color_str color);
	void set_second_color(Color_str color);
	void set_effect(byte num);
	void set_delay(uint delay);
	void set_br_cutoff_bound(int br_cutoff_bound);
	void set_preset(int num);
	void set_step(uint step);

	void tick(bool now = false);
private:
	IEffect* effect;
	Options* options;

	void apply_br();
	//void 

	unsigned long _strip_update_prev_time = 0;
	unsigned long _strip_update_cur_time = 0;
	float _br_max_gain;
	
	const uint8_t* _cur_cutoff_order;
	const uint8_t* _cur_cutoff_imm;
	byte _cur_cutoff_order_len, _cur_cutoff_imm_len, _cur_cutoff_units;

	uint8* _pixels;
	byte _led_amount;
	Color_str* _leds_arr; //array of leds color
	int _br_vir;
};
#endif
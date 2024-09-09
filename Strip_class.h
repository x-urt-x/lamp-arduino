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
	void set_br(int br);
	void set_main_color(Color_str color);
	void set_second_color(Color_str color);
	void set_effect(byte num);
	void set_delay(uint delay);
	void set_br_cutoff_bound(int br_cutoff_bound);
	int get_preset_count();
	const String* get_preset_names();
	void set_preset(int num);
	void tick(bool now = false);
private:
	IEffect* effect;


	void apply_br();
	void frame();

	unsigned long _strip_update_prev_time = 0;
	unsigned long _strip_update_cur_time = 0;
#if defined(LOG_USB_BR_ENABLE) || defined(LOG_USB_COLOR_MAP_ENABLE) || defined(LOG_USB_FIRE_ENABLE)
	uint _strip_update_delay_time = 5000;
#else
	uint _strip_update_delay_time = 200;
#endif

	const uint8_t* _cur_cutoff_order;
	const uint8_t* _cur_cutoff_imm;
	byte _cur_cutoff_order_len, _cur_cutoff_imm_len, _cur_cutoff_units;

	float _br_max_gain;
	int _br_cutoff_bound;

	uint8* _pixels;
	byte _led_amount;
	Color_str _main_color, _second_color;
	Color_str* _leds_arr; //array of leds color
	int _br_vir; //virtual brightness up to 255*97=24735
};

#endif



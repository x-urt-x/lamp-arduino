#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "log.h"
#include "Structures.h"

#include "IEffectBlock.h"
#include "BaseEffect.h"
#include "PresetBlock.h"
#include "ColorBlock.h"
#include "RainbowBlock.h"

//#include "ParseBlock.h"

#include "Effect_Noise.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"
#include "Effect_rainbowStrip.h"


class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6);

	void begin();
	void udp_set_color(Color_str color);
	void parse(const char* input_str);
	JsonDocument getJSON(bool udp, bool state);

	int get_br();
	void set_br(int br);
	void set_effect(byte num);
	//Effectable
	void set_effect_strip_update_delay_time(uint delay);
	void set_effect_br_cutoff_bound(int br_cutoff_bound);
	void set_effect_step(uint step);
	void apply_default_option();
	int get_strip_update_delay_time();
	int get_br_cutoff_bound();
	int get_effect_step();
	String get_effect_name();
	//Colorable
	void set_color(Color_str color, int num);
	Color_str* get_colors();
	int get_color_len();
	//Preseteble
	void set_preset(int num);
	int get_preset_count();
	const String* get_preset_names();
	//Rainbowble
	void set_rainbow_state(bool state, int num);
	void set_rainbow_step(int step, int num);
	bool* get_rainbow_states();
	int* get_rainbow_steps();
	int get_rainbow_len();

	void tick();

private:
	BaseEffect* effect;
	void apply_br();
	float _br_max_gain;
	
	byte _cur;

	uint8* _pixels;
	byte _led_amount;
	Color_str* _leds_arr; //array of leds color
	int _br_vir;
};
#endif
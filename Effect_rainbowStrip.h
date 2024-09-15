#ifndef EFFECT_RAINBOWSTRIP_H
#define EFFECT_RAINBOWSTRIP_H

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"
#include <Adafruit_NeoPixel.h>

class Effect_rainbowStrip : public IEffect
{
public:
	Effect_rainbowStrip(Color_str* leds_arr);

	void setup() override;
	void make_frame() override;
	Options* get_options_ptr() override;
	const byte* get_cutoff_order() override;
	const byte* get_cutoff_imm() override;
	byte get_cutoff_order_len() override;
	byte get_cutoff_imm_len() override;
	int get_preset_count() override;
	const String* get_preset_names() override;
	void set_preset(int num=0) override;
	String get_effect_name() override;
private:
	Color_str* _leds_arr;

	static Options _options;
	static bool _options_ini;

	static const byte _cutoff_order[96];
	static const byte _cutoff_imm[4];
	static const int _cutoff_order_len = 96;
	static const int _cutoff_imm_len = 4;

	static const String _preset_names[1];
	static const int _preset_len = 1;

	static const String _name;

	uint16_t _hue[100];
};

#endif
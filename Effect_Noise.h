#ifndef EFFECT_NOISE
#define EFFECT_NOISE

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"
#include "PerlinNoise.h"
#include <Adafruit_NeoPixel.h>

class Effect_Noise : public IEffect
{
public:
	Effect_Noise(Color_str* leds_arr);
	void setup() override;
	void make_frame() override;

	Options* get_options_ptr() override;

	const byte* get_cutoff_order() override;
	const byte* get_cutoff_imm() override;
	byte get_cutoff_order_len() override;
	byte get_cutoff_imm_len() override;

	int get_preset_count() override;
	const String* get_preset_names() override;
	void set_preset(int num = 0) override;

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

	uint16_t _x, _y, _z;
	static const int scale = 35;
};
#endif 
#ifndef EFFECT_NOISE
#define EFFECT_NOISE

#include <Arduino.h>
#include "Structures.h"
#include "Effect_bases.h"
#include "PerlinNoise.h"
#include <Adafruit_NeoPixel.h>

class Effect_Noise : public Effectable, public Colorable, public Rainbowble
{
public:
	Effect_Noise(Color_str* leds_arr);

	//Effectable
	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	void apply_default_option() override;

private:
	Color_str* _leds_arr;

	static IEffect::ParentBaseIDs _parent_base_IDs;
	IEffect::ParentBaseIDs* get_parent_base_ids() override;

	static Option_color _option_color;
	inline Option_color* get_option_color() override;

	static Option_rainbow _option_rainbow;
	inline Option_rainbow* get_option_rainbow() override;

	static Option_effect _option_effect;
	inline Option_effect* get_option_effect() override;

	uint16_t _x, _y, _z;
	static const int scale = 35;
	int hue_shift;
};
#endif 
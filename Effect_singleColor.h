#ifndef EFFECT_SINGLECOLOR_H
#define EFFECT_SINGLECOLOR_H

#include <Arduino.h>
#include "Structures.h"
#include "Effect_bases.h"

class Effect_singleColor : public Effectable, public Colorable, public Rainbowble
{
public:
	Effect_singleColor(Color_str* leds_arr);

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

	int _hue;
};
#endif
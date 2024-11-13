#include "Effect_rainbowStrip.h"

//class Effect_rainbowStrip : public Effectable, public Rainbowble
IEffect::ParentBaseIDs Effect_rainbowStrip::_parent_base_IDs =
{
	new BaseIDEnum[3] {
	IEffect::BaseIDEnum::EffectableID,
	IEffect::BaseIDEnum::RainbowbleID,
	},
	2
};
IEffect::ParentBaseIDs* Effect_rainbowStrip::get_parent_base_ids() { return &_parent_base_IDs; }

Rainbowble::Option_rainbow Effect_rainbowStrip::_option_rainbow{ new bool[1], 1, new int[1] };
inline Rainbowble::Option_rainbow* Effect_rainbowStrip::get_option_rainbow() { return &_option_rainbow; }

Effectable::Option_effect Effect_rainbowStrip::_option_effect{};
inline Effectable::Option_effect* Effect_rainbowStrip::get_option_effect() { return &_option_effect; }


Effect_rainbowStrip::Effect_rainbowStrip(Color_str* leds_arr) : _leds_arr(leds_arr) { apply_default_option(); }

String Effect_rainbowStrip::get_effect_name() { return "Rainbow Wave"; }

void Effect_rainbowStrip::apply_default_option()
{
	_option_effect.br_cutoff_bound = 0;
	_option_effect.strip_update_delay_time = 15;
	_option_rainbow.states[0] = true;
	_option_rainbow.steps[0] = 512;
}

void Effect_rainbowStrip::setup()
{
	for (int i = 0; i < MATR_LEN; i++)
		_hue[i] = i * 65535 / MATR_LEN;
}

void Effect_rainbowStrip::make_frame()
{
	for (int i = 0; i < MATR_LEN; i++)
	{
		_leds_arr[i].set(Color_str::ColorHSV(_hue[i]));
		_hue[i] += _option_rainbow.steps[0];
	}
}
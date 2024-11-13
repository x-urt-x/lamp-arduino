#include "Effect_singleColor.h"

//class Effect_singleColor : public Effectable, public Colorable, public Rainbowble
IEffect::ParentBaseIDs Effect_singleColor::_parent_base_IDs =
{
	new BaseIDEnum[3] {
	IEffect::BaseIDEnum::EffectableID,
	IEffect::BaseIDEnum::ColorableID,
	IEffect::BaseIDEnum::RainbowbleID
	},
	3
};
IEffect::ParentBaseIDs* Effect_singleColor::get_parent_base_ids() { return &_parent_base_IDs; }

Colorable::Option_color Effect_singleColor::_option_color{ new Color_str[1]{},1 };
inline Colorable::Option_color* Effect_singleColor::get_option_color() { return &_option_color; }

Rainbowble::Option_rainbow Effect_singleColor::_option_rainbow{ new bool[1], 1, new int[1] };
inline Rainbowble::Option_rainbow* Effect_singleColor::get_option_rainbow() { return &_option_rainbow; }

Effectable::Option_effect Effect_singleColor::_option_effect{};
inline Effectable::Option_effect* Effect_singleColor::get_option_effect() { return &_option_effect; }


Effect_singleColor::Effect_singleColor(Color_str* leds_arr) : _leds_arr(leds_arr) { apply_default_option(); }

String Effect_singleColor::get_effect_name() { return "Single Color"; }

void Effect_singleColor::apply_default_option()
{
	_option_color.colors[0] = Color_str(255, 255, 255);
	_option_effect.strip_update_delay_time = 100;
	_option_effect.br_cutoff_bound = 0;
	_option_effect.effect_step = 0;
	_option_rainbow.states[0] = false;
	_option_rainbow.steps[0] = 80;
}

void Effect_singleColor::setup() {}

void Effect_singleColor::make_frame()
{
	if (_option_rainbow.states[0])
	{
		Color_str color = Color_str::ColorHSV(_hue);
		for (int i = 0; i < MATR_LEN; i++)
			_leds_arr[i].set(color);
		_hue += _option_rainbow.steps[0];
	}
	else
	{
		for (int i = 0; i < MATR_LEN; i++)
			_leds_arr[i].set(_option_color.colors[0]);
	}
}

#include "Effect_Noise.h"

#define led_m(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]

//class Effect_Noise : public Effectable, public Colorable, public Rainbowble
IEffect::ParentBaseIDs Effect_Noise::_parent_base_IDs =
{
	new BaseIDEnum[3] {
	IEffect::BaseIDEnum::EffectableID,
	IEffect::BaseIDEnum::ColorableID,
	IEffect::BaseIDEnum::RainbowbleID,
	},
	3
};
IEffect::ParentBaseIDs* Effect_Noise::get_parent_base_ids() { return &_parent_base_IDs; }

Colorable::Option_color Effect_Noise::_option_color{ new Color_str[2]{},2 };
inline Colorable::Option_color* Effect_Noise::get_option_color() { return &_option_color; }

Rainbowble::Option_rainbow Effect_Noise::_option_rainbow{ new bool[1],1,new int[1] {} };
inline Rainbowble::Option_rainbow* Effect_Noise::get_option_rainbow() { return &_option_rainbow; }

Effectable::Option_effect Effect_Noise::_option_effect{};
inline Effectable::Option_effect* Effect_Noise::get_option_effect() { return &_option_effect; }


Effect_Noise::Effect_Noise(Color_str* leds_arr) : _leds_arr(leds_arr) { apply_default_option(); }

String Effect_Noise::get_effect_name() { return "Noise"; }

void Effect_Noise::apply_default_option()
{
	_option_color.colors[0] = Color_str(0, 0, 255);
	_option_color.colors[1] = Color_str(255, 0, 0);
	_option_effect.strip_update_delay_time = 10;
	_option_effect.br_cutoff_bound = 0;
	_option_effect.effect_step = 1;
	_option_rainbow.states[0] = true;
	_option_rainbow.steps[0] = 400;
}

void Effect_Noise::setup()
{
	hue_shift = 0;
	_x = random(0x10000);
	_y = random(0x10000);
	_z = random(0x10000);
}

void Effect_Noise::make_frame()
{
	for (int i = 0; i < 10; i++) {
		int ioffset = scale * i;
		for (int j = 0; j < 10; j++) {
			int joffset = scale * j;
			int noise = inoise8(_x + ioffset, _y + joffset, _z);
			if (_option_rainbow.states[0])
			{
				noise *= 255;
				noise += hue_shift;
				noise %= 256 * 256;
				led_m(i, j).set(Adafruit_NeoPixel::ColorHSV(noise));
			}
			else
			{
				led_m(i, j).set(Color_str::lerp(_option_color.colors[0], _option_color.colors[1], (uint8_t)noise));
			}
		}
	}
	hue_shift += _option_rainbow.steps[0];
	_z += _option_effect.effect_step;
}
#include "Effect_rainbowStrip.h"

BaseBlock Effect_rainbowStrip::baseBlock = BaseBlock();
RainbowBlock Effect_rainbowStrip::rainbowBlock = RainbowBlock(new bool[1], 1, new int[1]);

bool Effect_rainbowStrip::_has_init = false;

Effect_rainbowStrip::Effect_rainbowStrip(Color_str* leds_arr) : BaseEffect(leds_arr)
{
	BLOCK(baseBlock);
	BLOCK(rainbowBlock);
	if (!_has_init)
	{
		apply_default_option();
		_has_init = true;
	}
}


void Effect_rainbowStrip::apply_default_option()
{
	baseBlock.set_strip_update_delay_time(15);
	baseBlock.set_br_cutoff_bound(0);
	baseBlock.set_effect_step(0);
	rainbowBlock.get_rainbow_states()[0] = true;
	rainbowBlock.get_rainbow_steps()[0] = 512;
}

String Effect_rainbowStrip::get_effect_name() 
{ 
	return "Rainbow Wave"; 
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
		_hue[i] += rainbowBlock.get_rainbow_steps()[0];
	}
}
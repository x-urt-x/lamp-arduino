#include "Effect_singleColor.h"

BaseBlock Effect_singleColor::baseBlock = BaseBlock();
ColorBlock Effect_singleColor::colorBlock = ColorBlock(new Color_str[1]{}, 1);
RainbowBlock Effect_singleColor::rainbowBlock = RainbowBlock(new bool[1], 1, new int[1]);

bool Effect_singleColor::_has_init = false;

Effect_singleColor::Effect_singleColor(Color_str* leds_arr) : BaseEffect(leds_arr)
{
	BLOCK(baseBlock);
	BLOCK(colorBlock);
	BLOCK(rainbowBlock);
	if (!_has_init)
	{
		apply_default_option();
		_has_init = true;
	}
}

void Effect_singleColor::apply_default_option()
{
	baseBlock.set_strip_update_delay_time(100);
	baseBlock.set_br_cutoff_bound(0);
	baseBlock.set_effect_step(0);
	colorBlock.set_color(Color_str(255, 255, 255), 0);
	rainbowBlock.set_rainbow_state(false, 0);
	rainbowBlock.set_rainbow_step(80, 0);
}

String Effect_singleColor::get_effect_name() 
{
	return "Single Color"; 
}

void Effect_singleColor::setup() {}

void Effect_singleColor::make_frame()
{
	if (rainbowBlock.get_rainbow_states()[0])
	{
		Color_str color = Color_str::ColorHSV(_hue);
		for (int i = 0; i < MATR_LEN; i++)
			_leds_arr[i].set(color);
		_hue += rainbowBlock.get_rainbow_steps()[0];
	}
	else
	{
		for (int i = 0; i < MATR_LEN; i++)
			_leds_arr[i].set(colorBlock.get_colors()[0]);
	}
}

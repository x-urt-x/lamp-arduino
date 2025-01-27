#include "Effect_Noise.h"

#define led_m(x,y) _leds_arr[y*MATR_SIZE+(y%2==0?x:MATR_SIZE-x-1)]

BaseBlock Effect_Noise::baseBlock = BaseBlock();
ColorBlock Effect_Noise::colorBlock = ColorBlock(new Color_str[2]{}, 2);
RainbowBlock Effect_Noise::rainbowBlock = RainbowBlock(new bool[1], 1, new int[1]);

bool Effect_Noise::_has_init = false;

Effect_Noise::Effect_Noise(Color_str* leds_arr) : BaseEffect(leds_arr)
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

void Effect_Noise::apply_default_option()
{
	baseBlock.set_strip_update_delay_time(10);
	baseBlock.set_br_cutoff_bound(0);
	baseBlock.set_effect_step(1);
	colorBlock.set_color(Color_str(0, 0, 0), 0);
	colorBlock.set_color(Color_str(255, 255, 255), 1);
	rainbowBlock.set_rainbow_state(true, 0);
	rainbowBlock.set_rainbow_step(400, 0);
}

String Effect_Noise::get_effect_name() 
{
	return "Noise"; 
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
	for (int i = 0; i < MATR_SIZE; i++) {
		int ioffset = scale * i;
		for (int j = 0; j < MATR_SIZE; j++) {
			int joffset = scale * j;
			int noise = inoise8(_x + ioffset, _y + joffset, _z);
			if (rainbowBlock.get_rainbow_states()[0])
			{
				noise *= 255;
				noise += hue_shift;
				noise %= 256 * 256;
				led_m(i, j).set(Adafruit_NeoPixel::ColorHSV(noise));
			}
			else
			{
				led_m(i, j).set(Color_str::lerp(colorBlock.get_colors()[0], colorBlock.get_colors()[1], (uint8_t)noise));
			}
		}
	}
	hue_shift += rainbowBlock.get_rainbow_steps()[0];
	_z += baseBlock.get_effect_step();
}
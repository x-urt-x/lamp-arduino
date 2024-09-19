#include "Effect_Noise.h"

#define led_m(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]

Options Effect_Noise::_options = Options();
bool Effect_Noise::_options_ini = false;

const byte Effect_Noise::_cutoff_order[96] = { 0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,40,50,49,59,41,51,48,58,42,52,47,57,43,53,46,56 };
const byte Effect_Noise::_cutoff_imm[4] = { 44,45,54,55 };

const String Effect_Noise::_preset_names[1] = { "default" };

const String Effect_Noise::_name = "Noise";


Effect_Noise::Effect_Noise(Color_str* leds_arr) : _leds_arr(leds_arr)
{
	_x = random(0x10000);
	_y = random(0x10000);
	_z = random(0x10000);
}


const unsigned char* Effect_Noise::get_cutoff_order() { return _cutoff_order; }

const unsigned char* Effect_Noise::get_cutoff_imm() { return _cutoff_imm; }

byte Effect_Noise::get_cutoff_order_len() { return _cutoff_order_len; }

byte Effect_Noise::get_cutoff_imm_len() { return _cutoff_imm_len; }

int Effect_Noise::get_preset_count() { return _preset_len; }

const String* Effect_Noise::get_preset_names() { return _preset_names; }

String Effect_Noise::get_effect_name() { return _name; }

Options* Effect_Noise::get_options_ptr() { return &_options; }

void Effect_Noise::set_preset(int num)
{
	switch (num)
	{
	case 0:
	{
		_options.main_color = Color_str(0, 0, 0);
		_options.second_color = Color_str(0, 0, 0);
		_options.strip_update_delay_time = 10;
		_options.step = 1;
		_options.br_cutoff_bound = 0;
		break;
	}
	default:
		set_preset(0);
		break;
	}
}

void Effect_Noise::setup()
{
	if (!_options_ini)
	{
		set_preset();
		_options_ini = true;
	}
}

void Effect_Noise::make_frame()
{
	for (int i = 0; i < 10; i++) {
		int ioffset = scale * i;
		for (int j = 0; j < 10; j++) {
			int joffset = scale * j;
			uint8_t hue = inoise8(_x + ioffset, _y + joffset, _z);
			hue += _z;
			led_m(i,j).set(Adafruit_NeoPixel::ColorHSV(hue * 256));
		}
	}
	_z += _options.step;
}
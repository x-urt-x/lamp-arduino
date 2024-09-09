#include "Strip_class.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"

Strip::Strip(uint16_t n, int16_t p)
	: Adafruit_NeoPixel(n, p),
	_main_color(50, 50, 50),
	_second_color(50, 50, 50),
	_leds_arr(reinterpret_cast<Color_str*>(getPixels())),
	_pixels(getPixels()),
	_led_amount(n),
	_br_cutoff_bound(10)
{
	set_effect(EFF_FIRE);
}


void Strip::set_main_color(Color_str color) {
	_main_color = color;
}

void Strip::set_second_color(Color_str color)
{
	_second_color = color;
}

void Strip::frame()
{
	//LOG_USB_STARTUP("%d %d %d\n", _main_color.r, _main_color.g, _main_color.b);
	effect->make_frame();
	apply_br();
}

int Strip::get_preset_count()
{
	return effect->get_preset_count();
}

const String* Strip::get_preset_names()
{
	return effect->get_preset_names();
}

void Strip::set_preset(int num)
{
	effect->set_preset(num);
}

void Strip::tick(bool now)
{
	_strip_update_cur_time = millis();
	if ((_strip_update_cur_time - _strip_update_prev_time > _strip_update_delay_time) || now)
	{
		_strip_update_prev_time = _strip_update_cur_time;
		//Serial.printf("\n");

		frame();
		show();
	}
}

void Strip::set_effect(byte num)
{
	if (effect)
	{
		delete effect;
	}

	switch (num)
	{
	case EFF_SINGLE:
		effect = new Effect_singleColor(_leds_arr, &_main_color, &_strip_update_delay_time);
		break;
	case EFF_FIRE:
		effect = new Effect_fire(_leds_arr, &_main_color, &_second_color, &_strip_update_delay_time);
		break;
	}

	_cur_cutoff_order = effect->get_cutoff_order();
	_cur_cutoff_imm = effect->get_cutoff_imm();
	_cur_cutoff_order_len = effect->get_cutoff_order_len();
	_cur_cutoff_imm_len = effect->get_cutoff_order_len();
	_cur_cutoff_units = _cur_cutoff_order_len + (_cur_cutoff_imm_len == 0 ? 0 : 1);

	effect->setup();
}

void Strip::set_delay(uint delay)
{
	_strip_update_delay_time = delay;
}

void Strip::set_br_cutoff_bound(int br_cutoff_bound)
{
	_br_cutoff_bound = br_cutoff_bound;
}

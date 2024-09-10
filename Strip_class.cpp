#include "Strip_class.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"

Strip::Strip(uint16_t n, int16_t p)
	: Adafruit_NeoPixel(n, p),
	_leds_arr(reinterpret_cast<Color_str*>(getPixels())),
	_pixels(getPixels()),
	_led_amount(n)
{}


int Strip::get_preset_count() { return effect->get_preset_count(); }

const String* Strip::get_preset_names() { return effect->get_preset_names(); }

void Strip::set_preset(int num) { effect->set_preset(num); }

void Strip::set_main_color(Color_str color) { options->main_color = color; }

void Strip::set_second_color(Color_str color) { options->second_color = color; }

void Strip::set_delay(uint delay) {	options->strip_update_delay_time = delay; }

void Strip::set_br_cutoff_bound(int br_cutoff_bound) { options->br_cutoff_bound = br_cutoff_bound; }

void Strip::tick(bool now)
{
	_strip_update_cur_time = millis();
	if ((_strip_update_cur_time - _strip_update_prev_time > options->strip_update_delay_time) || now)
	{
		_strip_update_prev_time = _strip_update_cur_time;
		//Serial.printf("\n");

		effect->make_frame();
		apply_br();
		show();
	}
}

String Strip::get_status()
{
	char hex1[8], hex2[8];
	snprintf(hex1, sizeof(hex1), "#%02X%02X%02X", options->main_color.r, options->main_color.g, options->main_color.b);
	snprintf(hex2, sizeof(hex2), "#%02X%02X%02X", options->second_color.r, options->second_color.g, options->second_color.b);

	String result = "";
	result += "effect name: " + String(effect->get_effect_name()) + "\n";
	result += "brightness: " + String(_br_vir) + "\n";
	result += "strip_update_delay_time: " + String(options->strip_update_delay_time) + "\n";
	result += "main_color: " + String(hex1) + "\n";
	result += "second_color: " + String(hex2) + "\n";
	result += "br_cutoff_bound: " + String(options->br_cutoff_bound) + "\n";
	result += "step: " + String(options->step) + "\n";
	return result;
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
		effect = new Effect_singleColor(_leds_arr);
		break;
	case EFF_FIRE:
		effect = new Effect_fire(_leds_arr);
		break;
	}

	effect->setup();
	options = effect->get_options_ptr();

	_cur_cutoff_order = effect->get_cutoff_order();
	_cur_cutoff_imm = effect->get_cutoff_imm();
	_cur_cutoff_order_len = effect->get_cutoff_order_len();
	_cur_cutoff_imm_len = effect->get_cutoff_order_len();
	_cur_cutoff_units = _cur_cutoff_order_len + (_cur_cutoff_imm_len == 0 ? 0 : 1);
}
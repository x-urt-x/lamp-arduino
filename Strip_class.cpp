#include "Strip_class.h"
#include "Effect_Noise.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"
#include "Effect_rainbowStrip.h"

Strip::Strip(uint16_t n, int16_t p)
	: Adafruit_NeoPixel(n, p),
	_leds_arr(reinterpret_cast<Color_str*>(getPixels())),
	_pixels(getPixels()),
	_led_amount(n)
{}

void Strip::begin()
{
	IEffect::BaseSize_arr[IEffect::BaseIDEnum::EffectableID] = Effectable::BaseSize;
	IEffect::BaseSize_arr[IEffect::BaseIDEnum::ColorableID] = Colorable::BaseSize;
	IEffect::BaseSize_arr[IEffect::BaseIDEnum::PresetebleID] = Preseteble::BaseSize;
	IEffect::BaseSize_arr[IEffect::BaseIDEnum::RainbowbleID] = Rainbowble::BaseSize;
	Adafruit_NeoPixel::begin();
}

//Effectable
void Strip::set_effect_strip_update_delay_time(uint delay)
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		tmp->set_strip_update_delay_time(delay);
}
void Strip::set_effect_br_cutoff_bound(int br_cutoff_bound)
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		tmp->set_br_cutoff_bound(br_cutoff_bound);
}
void Strip::set_effect_step(uint step)
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		tmp->set_effect_step(step);
}
void Strip::apply_default_option()
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		tmp->apply_default_option();
}

//Colorable
void Strip::set_color(Color_str color, int num)
{
	if (Colorable* tmp = effect->castToBase<Colorable>())
		tmp->set_color(color, num);
}

//Preseteble
int Strip::get_preset_count()
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
		return tmp->get_preset_len();
	else
		return -1;
}
const String* Strip::get_preset_names()
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
	{
		return tmp->get_preset_names();
	}
	else
		return nullptr;
}
void Strip::set_preset(int num)
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
		tmp->set_preset(num);
}
//Rainbowble
void Strip::set_rainbow_state(bool state, int num)
{
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
		tmp->set_rainbow_state(state, num);
}
void Strip::set_rainbow_step(int step, int num)
{
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
		tmp->set_rainbow_step(step, num);
}


void Strip::tick(bool now)
{
	_strip_update_cur_time = millis();
	if ((_strip_update_cur_time - _strip_update_prev_time > _strip_update_delay_time) || now)
	{
		_strip_update_prev_time = _strip_update_cur_time;
		_strip_update_delay_time = reinterpret_cast<Effectable*>(effect)->get_strip_update_delay_time();
		//Serial.printf("\n");

		effect->make_frame();
		apply_br();
		show();
	}
}

void Strip::parse(const char* input_str)
{
	char key = input_str[0];
	input_str++
	LOG_USB_SWITCH("key0 - %c\n", key);
	switch (key)
	{
	case 'e':
	{
		key = input_str[0];
		input_str++
		LOG_USB_SWITCH("key1 - %c\n", key);
		switch (key)
		{
		case 'd':
			set_effect_strip_update_delay_time(atoi(input_str));
			break;
		case 'b':
			set_br(atoi(input_str));
			break;
		case 'c':
			set_effect_br_cutoff_bound(atoi(input_str));
			break;
		case 's':
			set_effect_step(atoi(input_str));
			break;
		default:
			break;
		}
		break;
	}
	case 'c':
	{
		byte pos = atoi(input_str);
		while (input_str[0] != ' ') input_str++;
		Color_str col;
		col.set(strtoul(input_str, nullptr, 16));
		set_color(col, pos);
	}
	case 'p':
	{
		key = input_str[0];
		input_str++
		LOG_USB_SWITCH("key1 - %c\n", key);
		switch (key)
		{
		case 'n':
		{
			int count = get_preset_count();
			if (count > 0)
			{
				const String* names = get_preset_names();
				for (int i = 0; i < count; i++)
				{
					Serial.printf("%d - %s\n", i, names[i]);
				}
			}
			else
			{
				Serial.printf("no preset\n");
			}
			break;
		}
		case 's':
		{
			set_preset(atoi(input_str));
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'r':
	{
		key = input_str[0];
		input_str++
		LOG_USB_SWITCH("key1 - %c\n", key);
		switch (key)
		{
		case 'm':
		{
			byte pos = atoi(input_str);
			while (input_str[0] != ' ') input_str++;
			set_rainbow_state(atoi(input_str), pos);
			break;
		}
		case 's':
		{
			byte pos = atoi(input_str);
			while (input_str[0] != ' ') input_str++;
			set_rainbow_step(atoi(input_str), pos);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 's':
	{
		Serial.print(get_status().c_str());
		break;
	}
	case 'm':
	{
		set_effect(atoi(input_str));
		break;
	}
	default:
		break;
	}
}

String Strip::get_status()
{
	String Effectable_str = "";
	if (Effectable* tmp = effect->castToBase<Effectable>())
	{
		Effectable_str += "name = ";
		Effectable_str += tmp->get_effect_name();
		Effectable_str += "\n";

		Effectable_str += "strip_update_delay_time = ";
		Effectable_str += tmp->get_strip_update_delay_time();
		Effectable_str += "\n";

		Effectable_str += "br_cutoff_bound = ";
		Effectable_str += tmp->get_br_cutoff_bound();
		Effectable_str += "\n";

		Effectable_str += "effect_step = ";
		Effectable_str += tmp->get_effect_step();
		Effectable_str += "\n";
	}

	String Colorable_str = "";
	if (Colorable* tmp = effect->castToBase<Colorable>())
	{
		int len = tmp->get_color_len();
		Color_str* colors = tmp->get_colors();
		for (int i = 0; i < len; i++)
		{
			Colorable_str += "Color ";
			Colorable_str += String(i);
			Colorable_str += " = ";
			char hex[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };
			snprintf(hex, sizeof(hex), "#%02X%02X%02X", colors[i].r, colors[i].g, colors[i].b);
			Colorable_str += String(hex);
			Colorable_str += "\n";
		}
	}

	String Preseteble_str = "";
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
	{
		int len = tmp->get_preset_len();
		const String* presets = tmp->get_preset_names();

		for (int i = 0; i < len; i++)
		{
			Colorable_str += "Preset name ";
			Colorable_str += String(i);
			Colorable_str += " = ";
			Colorable_str += presets[i];
			Colorable_str += "\n";
		}
	}

	String Rainbowble_str = "";
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
	{
		int len = tmp->get_rainbow_len();
		bool* states = tmp->get_rainbow_states();
		int* steps = tmp->get_rainbow_steps();

		for (int i = 0; i < len; i++)
		{
			Colorable_str += "State ";
			Colorable_str += String(i);
			Colorable_str += " = ";
			Colorable_str += String(states[i]);
			Colorable_str += "\t";
			Colorable_str += "Step ";
			Colorable_str += String(i);
			Colorable_str += " = ";
			Colorable_str += String(steps[i]);
			Colorable_str += "\n";
		}
	}

	return Effectable_str + Colorable_str + Preseteble_str + Rainbowble_str;
}

void Strip::set_effect(byte num)
{
	if (effect)
	{
		delete effect;
	}
	switch (num)
	{
	case 0:
		effect = new Effect_singleColor(_leds_arr);
		break;
	case 1:
		effect = new Effect_fire(_leds_arr);
		break;
	case 2:
		effect = new Effect_rainbowStrip(_leds_arr);
		break;
	case 3:
		effect = new Effect_Noise(_leds_arr);
		break;
	default:
		effect = new Effect_singleColor(_leds_arr);
		break;
	}

	effect->setup();

	_cutoff_option = effect->get_cutoff_str();
	_cur_cutoff_units = _cutoff_option->_cutoff_order_len + (_cutoff_option->_cutoff_imm_len == 0 ? 0 : 1);
}
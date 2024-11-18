#include "Strip_class.h"
#include "Effect_Noise.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"
#include "Effect_rainbowStrip.h"

Strip::Strip(uint16_t n, int16_t p)
	: Adafruit_NeoPixel(n, p),
	_leds_arr(reinterpret_cast<Color_str*>(getPixels())),
	_pixels(getPixels()),
	_led_amount(n),
	_state(true)
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
int Strip::get_strip_update_delay_time()
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		return tmp->get_strip_update_delay_time();
	return -1;
}
int Strip::get_br_cutoff_bound()
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		return tmp->get_br_cutoff_bound();
	return -1;
}
int Strip::get_effect_step()
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		return tmp->get_effect_step();
	return -1;
}
String Strip::get_effect_name()
{
	if (Effectable* tmp = effect->castToBase<Effectable>())
		return tmp->get_effect_name();
	return String();
}

//Colorable
void Strip::set_color(Color_str color, int num)
{
	if (Colorable* tmp = effect->castToBase<Colorable>())
		tmp->set_color(color, num);
}
Color_str* Strip::get_colors()
{
	if (Colorable* tmp = effect->castToBase<Colorable>())
		return tmp->get_colors();
	return nullptr;
}
int Strip::get_color_len()
{
	if (Colorable* tmp = effect->castToBase<Colorable>())
		return tmp->get_color_len();
	return 0;
}

//Preseteble
void Strip::set_preset(int num)
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
		tmp->set_preset(num);
}
int Strip::get_preset_count()
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
		return tmp->get_preset_len();
	return 0;
}
const String* Strip::get_preset_names()
{
	if (Preseteble* tmp = effect->castToBase<Preseteble>())
		return tmp->get_preset_names();
	return nullptr;
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
bool* Strip::get_rainbow_states()
{
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
		return tmp->get_rainbow_states();
	return nullptr;
}
int* Strip::get_rainbow_steps()
{
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
		return tmp->get_rainbow_steps();
	return nullptr;
}
int Strip::get_rainbow_len()
{
	if (Rainbowble* tmp = effect->castToBase<Rainbowble>())
		return tmp->get_rainbow_len();
	return 0;
}


void Strip::tick(bool now)
{
	if (_state)
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
}

void Strip::udp_set_color(Color_str color)
{
	set_color(color, 0);
}

void Strip::parse(const char* input_str)
{
	LOG_USB_SWITCH("%s\n", input_str);
	char key = input_str[0];
	input_str++;
	LOG_USB_SWITCH("key0 - %c\n", key);
	switch (key)
	{
	case 'e':
	{
		key = input_str[0];
		input_str++;
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
		case 'r':
			apply_default_option();
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
		input_str++;
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
		input_str++;
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
		set_state(atoi(input_str));
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

JsonDocument Strip::getJSON()
{
	JsonDocument doc;
	doc["state"] = String(_state);
	doc["name"] = get_effect_name();
	JsonArray blocks = doc.createNestedArray("blocks");

	JsonObject effectable = blocks.createNestedObject();
	effectable["block_name"] = "effectable";
	effectable["strip_update_delay_time"] = get_strip_update_delay_time();
	effectable["br"] = get_br();
	effectable["br_cutoff_bound"] = get_br_cutoff_bound();

	if (int len = get_color_len())
	{
		JsonObject colorable = blocks.createNestedObject();
		colorable["block_name"] = "colorable";
		//colorable["color_len"] = String(len);
		Color_str* colors = get_colors();
		char hex[8] = {};
		for (int i = 0; i < len; i++)
		{
			JsonObject field = colorable.createNestedObject(String(i));
			snprintf(hex, sizeof(hex), "#%02X%02X%02X", colors[i].r, colors[i].g, colors[i].b);
			field["Color"] = String(hex);
		}
	}
	if (int len = get_preset_count())
	{
		JsonObject preseteble = blocks.createNestedObject();
		preseteble["block_name"] = "preseteble";
		//preseteble["preseteble_len"] = String(len);
		const String* presets = get_preset_names();
		for (int i = 0; i < len; i++)
		{
			JsonObject field = preseteble.createNestedObject(String(i));
			field["Name"] = String(presets[i]);
		}
	}
	if (int len = get_rainbow_len())
	{
		JsonObject rainbowble = blocks.createNestedObject();
		rainbowble["block_name"] = "rainbowble";
		//rainbowble["rainbow_len"] = String(len);
		bool* states = get_rainbow_states();
		int* steps = get_rainbow_steps();
		for (int i = 0; i < len; i++)
		{
			JsonObject field = rainbowble.createNestedObject(String(i));
			field["State"] = String(states[i]);
			field["Step"] = String(steps[i]);
		}
	}
	return doc;
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

void Strip::set_state(int state) { set_state(state == 0 ? false : true); }
void Strip::set_state(bool state)
{
	_state = state;
	if (!_state)
	{
		fill(Color(0, 0, 0));
		show();
	}
}

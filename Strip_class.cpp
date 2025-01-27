#include "Strip_class.h"

Strip::Strip(uint16_t n, int16_t p)
	: Adafruit_NeoPixel(n, p),
	_leds_arr(reinterpret_cast<Color_str*>(getPixels())),
	_pixels(getPixels()),
	_led_amount(n)
{}

void Strip::begin()
{
	//ParseBlock::setStrip(this);
	Adafruit_NeoPixel::begin();
}

//IEffect
String Strip::get_effect_name()
{
	return effect->get_effect_name();
}
void Strip::apply_default_option()
{
	effect->apply_default_option();
}

//BaseBlock
void Strip::set_effect_strip_update_delay_time(uint delay)
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		tmp->set_strip_update_delay_time(delay);
	return;
}
void Strip::set_effect_br_cutoff_bound(int br_cutoff_bound)
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		tmp->set_br_cutoff_bound(br_cutoff_bound);
	return;
}
void Strip::set_effect_step(uint step)
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		tmp->set_effect_step(step);
	return;
}
int Strip::get_strip_update_delay_time()
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		return tmp->get_strip_update_delay_time();
	return -1;
}
int Strip::get_br_cutoff_bound()
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		return tmp->get_br_cutoff_bound();
	return -1;
}
int Strip::get_effect_step()
{
	if (BaseBlock* tmp = effect->get_block<BaseBlock>())
		return tmp->get_effect_step();
	return -1;
}

//ColorBlock
void Strip::set_color(Color_str color, int num)
{
	if (ColorBlock* tmp = effect->get_block<ColorBlock>())
		tmp->set_color(color, num);
	return;
}
Color_str* Strip::get_colors()
{
	if (ColorBlock* tmp = effect->get_block<ColorBlock>())
		return tmp->get_colors();
	return nullptr;
}
int Strip::get_color_len()
{
	if (ColorBlock* tmp = effect->get_block<ColorBlock>())
		return tmp->get_color_len();
	return 0;
}

//PresetBlock
void Strip::set_preset(int num)
{
	if (PresetBlock* tmp = effect->get_block<PresetBlock>())
		tmp->set_preset(num);
}
int Strip::get_preset_count()
{
	if (PresetBlock* tmp = effect->get_block<PresetBlock>())
		return tmp->get_preset_len();
	return 0;
}
const String* Strip::get_preset_names()
{
	if (PresetBlock* tmp = effect->get_block<PresetBlock>())
		return tmp->get_preset_names();
	return nullptr;
}

//RainbowBlock
void Strip::set_rainbow_state(bool state, int num)
{
	if (RainbowBlock* tmp = effect->get_block<RainbowBlock>())
		tmp->set_rainbow_state(state, num);
}
void Strip::set_rainbow_step(int step, int num)
{
	if (RainbowBlock* tmp = effect->get_block<RainbowBlock>())
		tmp->set_rainbow_step(step, num);
}
bool* Strip::get_rainbow_states()
{
	if (RainbowBlock* tmp = effect->get_block<RainbowBlock>())
		return tmp->get_rainbow_states();
	return nullptr;
}
int* Strip::get_rainbow_steps()
{
	if (RainbowBlock* tmp = effect->get_block<RainbowBlock>())
		return tmp->get_rainbow_steps();
	return nullptr;
}
int Strip::get_rainbow_len()
{
	if (RainbowBlock* tmp = effect->get_block<RainbowBlock>())
		return tmp->get_rainbow_len();
	return 0;
}

void Strip::tick()
{
	effect->make_frame();
	apply_br();
	show();
}

void Strip::udp_set_color(Color_str color)
{
	set_color(color, 0);
}

void Strip::parse(char* data)
{
	char* pch = NULL;
	pch = strtok(data, "\n");
	while (pch != NULL)
	{
		parseSingle(pch);
		pch = strtok(NULL, "\n");
	}
}


void Strip::parseSingle(const char* input_str)
{
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 'b':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 'd':
		{
			int delay;
			parseIn_int(delay);
			set_effect_strip_update_delay_time(delay);
			break;
		}
		case 'h':
		{
			int step;
			parseIn_int(step);
			set_effect_step(step);
			break;
		}
		case 'c':
		{
			int cutoff;
			parseIn_int(cutoff);
			set_effect_br_cutoff_bound(cutoff);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'c':
	{
		byte pos;
		parseIn_int(pos);
		Color_str col;
		col.set(strtoul(input_str, nullptr, 16));
		set_color(col, pos);
		break;
	}
	case 'r':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 's':
		{
			byte pos;
			parseIn_int(pos);
			bool state;
			parseIn_int(state);
			set_rainbow_state(state, pos);
			break;
		}
		case 'h':
		{
			byte pos;
			parseIn_int(pos);
			int step;
			parseIn_int(step);
			set_rainbow_step(step, pos);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'p':
	{
		byte pos;
		parseIn_int(pos);
		set_preset(pos);
		break;
	}
	default:
		break;
	}
}

void Strip::getEffectJSON(JsonArray& blocks)
{

	

	JsonObject base = blocks.createNestedObject();
	base["block_name"] = "base";
	base["strip_update_delay_time"] = get_strip_update_delay_time();
	base["br_cutoff_bound"] = get_br_cutoff_bound();
	base["effect_step"] = get_effect_step();

	if (int len = get_color_len())
	{
		JsonObject colorable = blocks.createNestedObject();
		colorable["block_name"] = "colorable";
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
		bool* states = get_rainbow_states();
		int* steps = get_rainbow_steps();
		for (int i = 0; i < len; i++)
		{
			JsonObject field = rainbowble.createNestedObject(String(i));
			field["State"] = String(states[i]);
			field["Step"] = String(steps[i]);
		}
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
	case EffectIDEnum::singleColor:
		effect = new Effect_singleColor(_leds_arr);
		break;
	case EffectIDEnum::fire:
		effect = new Effect_fire(_leds_arr);
		break;
	case EffectIDEnum::rainbowStrip:
		effect = new Effect_rainbowStrip(_leds_arr);
		break;
	case EffectIDEnum::Noise:
		effect = new Effect_Noise(_leds_arr);
		break;
	default:
		effect = new Effect_singleColor(_leds_arr);
		break;
	}

	effect->setup();
	//_cur_cutoff_units = _cutoff_option->_cutoff_order_len + (_cutoff_option->_cutoff_imm_len == 0 ? 0 : 1);
}
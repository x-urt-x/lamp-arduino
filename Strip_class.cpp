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

//Effectable
void Strip::set_effect_strip_update_delay_time(uint delay)
{
	effect->set_strip_update_delay_time(delay);
}
void Strip::set_effect_br_cutoff_bound(int br_cutoff_bound)
{
	effect->set_br_cutoff_bound(br_cutoff_bound);
}
void Strip::set_effect_step(uint step)
{
	effect->set_effect_step(step);
}
void Strip::apply_default_option()
{
	effect->apply_default_option();
}
int Strip::get_strip_update_delay_time()
{
	return effect->get_strip_update_delay_time();
}
int Strip::get_br_cutoff_bound()
{
	return effect->get_br_cutoff_bound();
}
int Strip::get_effect_step()
{
	return effect->get_effect_step();
}
String Strip::get_effect_name()
{
	return effect->get_effect_name();
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
	case 'm':
	{
		set_effect(atoi(input_str));
		break;
	}
	case 'x':
	{
		key = input_str[0];
		input_str++;
		LOG_USB_SWITCH("key1 - %t\n", key);
		switch (key)
		{
		case 'r':
		{
			LOG_USB_STARTUP("reset\n");
			ESP.restart();
			break;
		}
		case 'f':
		{
			LOG_USB_STARTUP("full reset\n");
			EEPROM.write(0, 0x80);
			EEPROM.commit();
			ESP.restart();
			break;
		}
		case 't':
		{
			unsigned long now_millis_time = millis();
			byte now_weekday = StartTimeInfo::start_weekday + (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) / 86400UL;
			unsigned long now_daytime = (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) % 86400UL;


			Serial.printf("start_day_time: %lu\n ", StartTimeInfo::start_day_time);
			Serial.printf("start_epoch_time: %lu\n ", StartTimeInfo::start_epoch_time);
			Serial.printf("start_millis_time: %lu\n ", StartTimeInfo::start_millis_time);
			Serial.printf("start_weekday: %d\n ", StartTimeInfo::start_weekday);
			Serial.printf("now_millis_time: %lu\n ", now_millis_time);
			Serial.printf("now_weekday: %d\n ", now_weekday);
			Serial.printf("now_daytime: %lu\n ", now_daytime);

			break;
		}
		case 'p':
		{
			Serial.println("print mem");
			Serial.printf("init bit: %d\n", EEPROM.read(0) >> 7);
			Serial.printf("obj data count: %d\n", EEPROM.read(0) & 0b0111'1111);
			for (int i = 0; i < OBJ_DATA_CAP; i++)
			{
				uint16_t addr;
				EEPROM.get(1 + i * 2, addr);
				Serial.printf("%d-%d|",addr >> 12, addr & 0x0FFF);
			}
			Serial.print('\n');

			for (int i = (OBJ_DATA_CAP+1) * 2 + 1; i < 64 * 64; i++)
			{
				if((i - (OBJ_DATA_CAP + 1) * 2 + 1 )%64==0) Serial.print('\n');
				byte value = EEPROM.read(i);
				if (value < 0x10)
					Serial.print("0");
				Serial.print(value, HEX);
				Serial.print(" ");
			}
			Serial.print('\n');
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

JsonDocument Strip::getJSON(bool udp, bool state, int timerCount)
{
	JsonDocument doc;
	doc["state"] = String(state);
	doc["UDP"] = String(udp);
	doc["name"] = get_effect_name();
	doc["timerCount"] = timerCount;
	doc["br"] = get_br();
	JsonArray blocks = doc.createNestedArray("blocks");

	JsonObject effectable = blocks.createNestedObject();
	effectable["block_name"] = "effectable";
	effectable["strip_update_delay_time"] = get_strip_update_delay_time();
	effectable["br_cutoff_bound"] = get_br_cutoff_bound();
	effectable["effect_step"] = get_effect_step();

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
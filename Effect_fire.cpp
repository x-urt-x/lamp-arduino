#include "Effect_fire.h"

#define FIRE_COEF 38

BaseBlock Effect_fire::baseBlock = BaseBlock();
ColorBlock Effect_fire::colorBlock = ColorBlock(new Color_str[4]{}, 4);
PresetBlock Effect_fire::presetBlock = PresetBlock(new String[3]{ "red fire","blue fire","green fire" }, 3);

bool Effect_fire::_has_init = false;

Effect_fire::Effect_fire(Color_str* leds_arr) : BaseEffect(leds_arr), _pos(0), _key_temp(0), _cur_temp(0)
{
	BLOCK(baseBlock);
	BLOCK(colorBlock);
	BLOCK(presetBlock);
	if (!_has_init)
	{
		apply_default_option();
		_has_init = true;
	}
}


String Effect_fire::get_effect_name() { return "Fire"; }

void Effect_fire::apply_default_option() { preset(0); }

void Effect_fire::preset(int num)
{
	switch (num)
	{
	case 0:
	{
		colorBlock.get_colors()[0] = Color_str(0, 0, 0);
		colorBlock.get_colors()[1] = Color_str(255, 0, 0);
		colorBlock.get_colors()[2] = Color_str(255, 165, 0);
		colorBlock.get_colors()[3] = Color_str(255, 240, 200);
		baseBlock.set_strip_update_delay_time(20);
		baseBlock.set_br_cutoff_bound(30);
		baseBlock.set_effect_step(30);
		break;
	}
	case 1:
	{
		colorBlock.get_colors()[0] = Color_str(0, 0, 0);
		colorBlock.get_colors()[1] = Color_str(0, 0, 255);
		colorBlock.get_colors()[2] = Color_str(49, 207, 216);
		colorBlock.get_colors()[3] = Color_str(255, 255, 255);
		baseBlock.set_strip_update_delay_time(20);
		baseBlock.set_br_cutoff_bound(30);
		baseBlock.set_effect_step(60);
		break;
	}
	case 2:
	{
		colorBlock.get_colors()[0] = Color_str(0, 0, 0);
		colorBlock.get_colors()[1] = Color_str(0, 255, 0);
		colorBlock.get_colors()[2] = Color_str(27, 239, 15);
		colorBlock.get_colors()[3] = Color_str(255, 255, 255);
		baseBlock.set_strip_update_delay_time(20);
		baseBlock.set_br_cutoff_bound(30);
		baseBlock.set_effect_step(30);
		break;
	}
	default:
		break;
	}
}

void Effect_fire::setup()
{
	_pos = baseBlock.get_effect_step();
	_key_temp = 0;
	_cur_temp = 0;
}

void Effect_fire::make_frame()
{
	if (_pos >= baseBlock.get_effect_step())
	{
		_key_temp = rand() % 255;
		if (_key_temp < 40) _key_temp *= 2;
		if (_key_temp > 215) _key_temp -= (_key_temp/215)*2;
		_pos = 0;
		LOG_USB_FIRE("new _key_temp =  %d\n", _key_temp);
	}
	_cur_temp += (_key_temp - _cur_temp) * FIRE_COEF / 255;
	uint32_t col = temp_to_color(_cur_temp);
	LOG_USB_FIRE("_cur_temp = %d\t_key_temp = %d\n", _cur_temp,_key_temp);
	for ( int i = 0; i < MATR_LEN; i++)
	{
		_leds_arr[i].set(col);
	}
	_pos++;
}

uint32_t Effect_fire::temp_to_color(byte temp) {
	Color_str color(0, 0, 0);

	// Нормализуем температуру от 0 до 1
	float t = temp / 255.0f;

	// Если температура низкая, интерполируем от чёрного к baseColor
	if (t < 0.33f) {
		float localT = t / 0.33f;  // Нормализуем для этого сегмента (0.0 до 0.33 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(colorBlock.get_colors()[0].r * (1 - localT) + colorBlock.get_colors()[1].r * localT);
		color.g = static_cast<uint8_t>(colorBlock.get_colors()[0].g * (1 - localT) + colorBlock.get_colors()[1].g * localT);
		color.b = static_cast<uint8_t>(colorBlock.get_colors()[0].b * (1 - localT) + colorBlock.get_colors()[1].b * localT);
	}
	// Если температура средняя, интерполируем от baseColor к colorMid
	else if (t < 0.66f) {
		float localT = (t - 0.33f) / 0.33f;  // Нормализуем для этого сегмента (0.33 до 0.66 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(colorBlock.get_colors()[1].r * (1 - localT) + colorBlock.get_colors()[2].r * localT);
		color.g = static_cast<uint8_t>(colorBlock.get_colors()[1].g * (1 - localT) + colorBlock.get_colors()[2].g * localT);
		color.b = static_cast<uint8_t>(colorBlock.get_colors()[1].b * (1 - localT) + colorBlock.get_colors()[2].b * localT);
	}
	// Если температура высокая, интерполируем от colorMid к colorHigh
	else {
		float localT = (t - 0.66f) / 0.34f;  // Нормализуем для этого сегмента (0.66 до 1.0 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(colorBlock.get_colors()[2].r * (1 - localT) + colorBlock.get_colors()[3].r * localT);
		color.g = static_cast<uint8_t>(colorBlock.get_colors()[2].g * (1 - localT) + colorBlock.get_colors()[3].g * localT);
		color.b = static_cast<uint8_t>(colorBlock.get_colors()[2].b * (1 - localT) + colorBlock.get_colors()[3].b * localT);
	}
	return color.get();
}
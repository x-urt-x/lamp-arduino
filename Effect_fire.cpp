#include "Effect_fire.h"

#define led_m(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]

#define FRAMES 40
#define SIDE_CENTER 10
#define SIDE_RANGE 2
#define DEC_CENTER 14
#define DEC_RANGE 10
#define RANDOM_CENTER 0
#define RANDOM_RANGE 3

byte Effect_fire::_cutoff_order[98] = { 0,9,1,8,2,7,3,6,4,5,10,19,11,18,12,17,13,16,14,15,20,29,21,28,22,27,23,26,24,25,30,39,31,38,32,37,33,36,34,35,40,49,41,48,42,47,43,46,44,45,50,59,51,58,52,57,53,56,54,55,60,69,61,68,62,67,63,66,64,65,70,79,71,78,72,77,73,76,74,75,80,89,81,88,82,87,83,86,84,85,90,99,91,98,92,97,93,96 };
byte Effect_fire::_cutoff_imm[2] = { 94,95 };

//class Effect_fire : public Effectable, public Colorable, public Preseteble
IEffect::ParentBaseIDs Effect_fire::_parent_base_IDs =
{
	new BaseIDEnum[3] {
	IEffect::BaseIDEnum::EffectableID,
	IEffect::BaseIDEnum::ColorableID,
	IEffect::BaseIDEnum::PresetebleID
	},
	3
};
IEffect::ParentBaseIDs* Effect_fire::get_parent_base_ids() { return &_parent_base_IDs; }

Effectable::Option_effect Effect_fire::_option_effect{};
inline Effectable::Option_effect* Effect_fire::get_option_effect() { return &_option_effect; }

Colorable::Option_color Effect_fire::_option_color{ new Color_str[4]{}, 4 };
inline Colorable::Option_color* Effect_fire::get_option_color() { return &_option_color; }

Preseteble::Option_preset Effect_fire::_option_preset{ (new String[3]{"red fire","blue fire","green fire"}),4 };
inline Preseteble::Option_preset* Effect_fire::get_option_preset() { return &_option_preset; }

Effect_fire::Effect_fire(Color_str* leds_arr) : _leds_arr(leds_arr) { apply_default_option(); }


byte* Effect_fire::get_cutoff_order() { return _cutoff_order; }
byte* Effect_fire::get_cutoff_imm() { return _cutoff_imm; }
byte Effect_fire::get_cutoff_order_len() { return _cutoff_order_len; }
byte Effect_fire::get_cutoff_imm_len() { return _cutoff_imm_len; }

String Effect_fire::get_effect_name() { return "Fire"; }

void Effect_fire::apply_default_option() { set_preset(0); }

void Effect_fire::set_preset(int num)
{
	switch (num)
	{
	case 0:
	{
		_option_color.colors[0] = Color_str(0, 0, 0);
		_option_color.colors[1] = Color_str(255, 0, 0);
		_option_color.colors[2] = Color_str(255, 165, 0);
		_option_color.colors[3] = Color_str(255, 240, 200);
		_option_effect.strip_update_delay_time = 50;
		_option_effect.effect_step = 30;
		_option_effect.br_cutoff_bound = 30;
		break;
	}
	case 1:
	{
		_option_color.colors[0] = Color_str(0, 0, 0);
		_option_color.colors[1] = Color_str(0, 0, 255);
		_option_color.colors[2] = Color_str(49, 207, 216);
		_option_color.colors[3] = Color_str(255, 255, 255);
		_option_effect.strip_update_delay_time = 60;
		_option_effect.effect_step = 30;
		_option_effect.br_cutoff_bound = 60;
		break;
	}
	case 2:
	{
		_option_color.colors[0] = Color_str(0, 0, 0);
		_option_color.colors[1] = Color_str(0, 255, 0);
		_option_color.colors[2] = Color_str(27, 239, 15);
		_option_color.colors[3] = Color_str(255, 255, 255);
		_option_effect.strip_update_delay_time = 50;
		_option_effect.effect_step = 30;
		_option_effect.br_cutoff_bound = 30;
		break;
	}
	default:
		set_preset(0);
		break;
	}
}

void Effect_fire::setup()
{
	_frame_count = FRAMES;
	//центр пламени
	_center_temp_change = 20;
	_center_pos = random(2, 8);
	_center_temp = random(180, 240);

	dic_map_key_gen();
	for (int i = 0; i < 10; i++)
	{
		_side_coef_cur[i] = _side_coef_key[i];
		for (int j = 0; j < 10; j++)
		{
			_dic_map_cur[i][j] = _dic_map_key[i][j];
		}
	}
}

void Effect_fire::dic_map_key_gen()
{
	LOG_USB_FIRE("center = %d\tcenter_temp = %d\n", _center_pos, _center_temp);

	_dic_map_key[_center_pos][9] = 255 - _center_temp;
	for (int x = 0; x < 10; x++)
	{
		_dic_map_key[x][9] = random(10, 20);
	}

	_side_coef_key[0] = random(SIDE_CENTER - SIDE_RANGE, SIDE_CENTER + SIDE_RANGE + 1);
	for (int y = 1; y < 10; y++)
		_side_coef_key[y] = _side_coef_key[y - 1] + int(_side_coef_key[y - 1] < SIDE_CENTER ? random(-11, 4) : random(-3, 12)) / 4;

	for (int y = 0; y < 9; y++)
		for (int x = 0; x < 10; x++)
			_dic_map_key[x][y] = random(DEC_CENTER - DEC_RANGE, DEC_CENTER + DEC_RANGE + 1);

	LOG_USB_FIRE_SIDE_MAP(_side_coef_key, _dic_map_key);
}

int Effect_fire::dic_map_cur_step()
{
	LOG_USB_FIRE("++++dic_map_cur_step++++\n");

	LOG_USB_FIRE("dic_map_cur_step calculation\n");

	for (int y = 0; y < 10; y++)
	{
		if (_side_coef_cur[y] > _side_coef_key[y])
		{
			_side_coef_cur[y]--;
		}
		else if (_side_coef_cur[y] < _side_coef_key[y])
		{
			_side_coef_cur[y]++;
		}
	}
	int difference = 0;
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			float dif = 0;
			if (_dic_map_cur[x][y] > _dic_map_key[x][y])
			{
				dif = _dic_map_cur[x][y] - _dic_map_key[x][y];
				_dic_map_cur[x][y] -= byte_round_up(dif / (FRAMES - _frame_count));
			}
			else if (_dic_map_cur[x][y] < _dic_map_key[x][y])
			{
				dif = _dic_map_key[x][y] - _dic_map_cur[x][y];
				_dic_map_cur[x][y] += byte_round_up(dif / (FRAMES - _frame_count));
			}
			difference += dif;
		}
	}

	_frame_count--;
	LOG_USB_FIRE("difference = %d _fire_frame_count = %d\n", difference, _frame_count);
	LOG_USB_FIRE("++++dic_map_cur_step++++\n");

	return difference;
}

void Effect_fire::temp_map_gen()
{
	_temp_map[_center_pos][9] = _center_temp;
	for (int x = _center_pos + 1; x < 10; x++)
		_temp_map[x][9] = constrain(_temp_map[x - 1][9] - _dic_map_cur[x][9] * (1 + sqrt(x - _center_pos) / 2), 1, 254) - random(-1, 2);
	for (int x = _center_pos - 1; x >= 0; x--)
		_temp_map[x][9] = constrain(_temp_map[x + 1][9] - _dic_map_cur[x][9] * (1 + sqrt(_center_pos - x) / 2), 1, 254) - random(-1, 2);

	for (int y = 0; y < 9; y++)
	{
		byte left = 2 * SIDE_CENTER - _side_coef_cur[y];
		LOG_USB_FIRE("row: %d\t", y);
		for (int x = 0; x < 10; x++)
		{
			_temp_map[x][y] = (
				_temp_map[x][y + 1] +
				_temp_map[x][y > 7 ? 9 : y + 2] * 1.05 +
				float(_temp_map[constrain(x - 1, 0, 9)][y + 1]) * ((left + random(RANDOM_CENTER - RANDOM_RANGE, RANDOM_CENTER + RANDOM_RANGE + 1) / 10.0) / 10.0) +
				float(_temp_map[constrain(x + 1, 0, 9)][y + 1]) * ((_side_coef_cur[y] + random(RANDOM_CENTER - RANDOM_RANGE, RANDOM_CENTER + RANDOM_RANGE + 1) / 10.0) / 10.0)
				) / 4;
			LOG_USB_FIRE("%d - ", _temp_map[x][y]);
			int dic = _dic_map_cur[x][y] + int(random(-4, 6) / 3);
			LOG_USB_FIRE("%d = ", dic);
			_temp_map[x][y] = _temp_map[x][y] < dic ? 0 : _temp_map[x][y] - dic;
			LOG_USB_FIRE("%d\t", _temp_map[x][y]);
		}
		LOG_USB_FIRE("\n");
	}
}

void Effect_fire::make_frame()
{
	LOG_USB_FIRE("\n====Frame start====\n");
	if (dic_map_cur_step() < 5) {
		LOG_USB_FIRE("----new frame----\n");
		//температура центра
		_center_temp = random(
			byte_d(_center_temp - _center_temp_change),
			byte_u(_center_temp + 21)
		);;
		if (_center_temp < 100) _center_temp_change = 0;
		if (_center_temp > 180) _center_temp_change = 20;
		if (_center_temp > 220) _center_temp_change = 50;

		//положение центра
		_center_pos += byte(random((_center_pos > 6 ? 1 : 5), (_center_pos < 3 ? 17 : 13))) / 6;
		_center_pos = constrain(_center_pos, 1, 10);
		_center_pos--;

		LOG_USB_FIRE("_fire_center %d\n", _center_pos);
		LOG_USB_FIRE("_fire_center_temp %d\n", _center_temp);
		dic_map_key_gen();
		LOG_USB_FIRE("----new frame----\n");
		_frame_count = FRAMES;
	}

	LOG_USB_FIRE_SIDE_MAP(_side_coef_key, _dic_map_key);
	LOG_USB_FIRE_SIDE_MAP(_side_coef_cur, _dic_map_cur);

	LOG_USB_FIRE("old ");
	LOG_USB_FIRE_TEMP();

	temp_map_gen();

	LOG_USB_FIRE("new ");
	LOG_USB_FIRE_TEMP();

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			led_m(i, j).set(temp_to_color(_temp_map[i][j]));
}

uint32_t Effect_fire::temp_to_color(byte temp) {
	Color_str color(0, 0, 0);

	// Нормализуем температуру от 0 до 1
	float t = temp / 255.0f;

	// Если температура низкая, интерполируем от чёрного к baseColor
	if (t < 0.33f) {
		float localT = t / 0.33f;  // Нормализуем для этого сегмента (0.0 до 0.33 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(_option_color.colors[0].r * (1 - localT) + _option_color.colors[1].r * localT);
		color.g = static_cast<uint8_t>(_option_color.colors[0].g * (1 - localT) + _option_color.colors[1].g * localT);
		color.b = static_cast<uint8_t>(_option_color.colors[0].b * (1 - localT) + _option_color.colors[1].b * localT);
	}
	// Если температура средняя, интерполируем от baseColor к colorMid
	else if (t < 0.66f) {
		float localT = (t - 0.33f) / 0.33f;  // Нормализуем для этого сегмента (0.33 до 0.66 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(_option_color.colors[1].r * (1 - localT) + _option_color.colors[2].r * localT);
		color.g = static_cast<uint8_t>(_option_color.colors[1].g * (1 - localT) + _option_color.colors[2].g * localT);
		color.b = static_cast<uint8_t>(_option_color.colors[1].b * (1 - localT) + _option_color.colors[2].b * localT);
	}
	// Если температура высокая, интерполируем от colorMid к colorHigh
	else {
		float localT = (t - 0.66f) / 0.34f;  // Нормализуем для этого сегмента (0.66 до 1.0 -> 0.0 до 1.0)
		color.r = static_cast<uint8_t>(_option_color.colors[2].r * (1 - localT) + _option_color.colors[3].r * localT);
		color.g = static_cast<uint8_t>(_option_color.colors[2].g * (1 - localT) + _option_color.colors[3].g * localT);
		color.b = static_cast<uint8_t>(_option_color.colors[2].b * (1 - localT) + _option_color.colors[3].b * localT);
	}
	return color.get();
}
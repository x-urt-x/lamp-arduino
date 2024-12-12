#include "Effect_fire.h"

#define led_m(x,y) _leds_arr[255-(y*MATR_SIZE+(y%2==0?x:MATR_SIZE-x-1))]

#define FRAMES 40
#define SIDE_CENTER 10
#define SIDE_RANGE 2
#define DEC_CENTER 14
#define DEC_RANGE 10
#define RANDOM_CENTER 0
#define RANDOM_RANGE 3

Cutoff_str Effect_fire::_cutoff_option
{
	MATR_LEN - 2,
	2,
	new byte[MATR_LEN - 2]
#ifdef MATR10x10
{
	0,9,1,8,2,7,3,6,4,5,
	10,19,11,18,12,17,13,16,14,15,
	20,29,21,28,22,27,23,26,24,25,
	30,39,31,38,32,37,33,36,34,35,
	40,49,41,48,42,47,43,46,44,45,
	50,59,51,58,52,57,53,56,54,55,
	60,69,61,68,62,67,63,66,64,65,
	70,79,71,78,72,77,73,76,74,75,
	80,89,81,88,82,87,83,86,84,85,
	90,99,
	91,98,
	92,97,
	93,96
}
#else
#ifdef MATR16x16
{
	0, 15, 1, 14, 2, 13, 3, 12, 4, 11, 5, 10, 6, 9, 7, 8,
		16, 31, 17, 30, 18, 29, 19, 28, 20, 27, 21, 26, 22, 25, 23, 24,
		32, 47, 33, 46, 34, 45, 35, 44, 36, 43, 37, 42, 38, 41, 39, 40,
		48, 63, 49, 62, 50, 61, 51, 60, 52, 59, 53, 58, 54, 57, 55, 56,
		64, 79, 65, 78, 66, 77, 67, 76, 68, 75, 69, 74, 70, 73, 71, 72,
		80, 95, 81, 94, 82, 93, 83, 92, 84, 91, 85, 90, 86, 89, 87, 88,
		96, 111, 97, 110, 98, 109, 99, 108, 100, 107, 101, 106, 102, 105, 103, 104,
		112, 127, 113, 126, 114, 125, 115, 124, 116, 123, 117, 122, 118, 121, 119, 120,
		128, 143, 129, 142, 130, 141, 131, 140, 132, 139, 133, 138, 134, 137, 135, 136,
		144, 159, 145, 158, 146, 157, 147, 156, 148, 155, 149, 154, 150, 153, 151, 152,
		160, 175, 161, 174, 162, 173, 163, 172, 164, 171, 165, 170, 166, 169, 167, 168,
		176, 191, 177, 190, 178, 189, 179, 188, 180, 187, 181, 186, 182, 185, 183, 184,
		192, 207, 193, 206, 194, 205, 195, 204, 196, 203, 197, 202, 198, 201, 199, 200,
		208, 223, 209, 222, 210, 221, 211, 220, 212, 219, 213, 218, 214, 217, 215, 216,
		224, 239, 225, 238, 226, 237, 227, 236, 228, 235, 229, 234, 230, 233, 231, 232,
		240, 255,
		241, 254,
		242, 253,
		243, 252,
		244, 251,
		245, 250,
		246, 249,
}
#endif // MATR16x16
#endif // MATR10x10
,
new byte[2]
#ifdef MATR10x10
{ 94, 95 }
#else
#ifdef MATR16x16
{ 247, 248 }
#endif // MATR16x16
#endif // MATR10x10
};

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

Preseteble::Option_preset Effect_fire::_option_preset{ (new String[3]{"red fire","blue fire","green fire"}),3 };
inline Preseteble::Option_preset* Effect_fire::get_option_preset() { return &_option_preset; }

bool Effect_fire::_has_init = false;
Effect_fire::Effect_fire(Color_str* leds_arr) : _leds_arr(leds_arr)
{
	if (!_has_init)
	{
		apply_default_option();
		_has_init = true;
	}
}


String Effect_fire::get_effect_name() { return "Fire"; }

void Effect_fire::apply_default_option() { set_preset(0); }

Cutoff_str* Effect_fire::get_cutoff_str() { return &_cutoff_option; }

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
	_center_pos = random(2, MATR_SIZE - 2);
	_center_temp = random(180, 240);

	dic_map_key_gen();
	for (int i = 0; i < MATR_SIZE; i++)
	{
		_side_coef_cur[i] = _side_coef_key[i];
		for (int j = 0; j < MATR_SIZE; j++)
		{
			_dic_map_cur[i][j] = _dic_map_key[i][j];
		}
	}
}

void Effect_fire::dic_map_key_gen()
{
	LOG_USB_FIRE("center = %d\tcenter_temp = %d\n", _center_pos, _center_temp);

	_dic_map_key[_center_pos][MATR_SIZE - 1] = 255 - _center_temp;
	for (int x = 0; x < MATR_SIZE; x++)
	{
		_dic_map_key[x][MATR_SIZE - 1] = random(10, 20);
	}

	_side_coef_key[0] = random(SIDE_CENTER - SIDE_RANGE, SIDE_CENTER + SIDE_RANGE + 1);
	for (int y = 1; y < MATR_SIZE; y++)
	{
		int delta = int(_side_coef_key[y - 1] > SIDE_CENTER ? random(-11, 4) : random(-3, 12)) / 4;
		_side_coef_key[y] = constrain(int(_side_coef_key[y - 1]) + delta, 0, 255);
	}

	for (int y = 0; y < MATR_SIZE - 1; y++)
		for (int x = 0; x < MATR_SIZE; x++)
			_dic_map_key[x][y] = random(DEC_CENTER - DEC_RANGE, DEC_CENTER + DEC_RANGE + 1);

	LOG_USB_FIRE_SIDE_MAP(_side_coef_key, _dic_map_key);
}

int Effect_fire::dic_map_cur_step()
{
	LOG_USB_FIRE("++++dic_map_cur_step++++\n");

	LOG_USB_FIRE("dic_map_cur_step calculation\n");

	for (int y = 0; y < MATR_SIZE; y++)
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
	for (int y = 0; y < MATR_SIZE; y++)
	{
		for (int x = 0; x < MATR_SIZE; x++)
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
	_temp_map[_center_pos][MATR_SIZE - 1] = _center_temp;
	for (int x = _center_pos + 1; x < MATR_SIZE; x++)
		_temp_map[x][MATR_SIZE - 1] = constrain(_temp_map[x - 1][MATR_SIZE - 1] - _dic_map_cur[x][MATR_SIZE - 1] * (1 + sqrt(x - _center_pos) / 2), 1, 254) - random(-1, 2);
	for (int x = _center_pos - 1; x >= 0; x--)
		_temp_map[x][MATR_SIZE - 1] = constrain(_temp_map[x + 1][MATR_SIZE - 1] - _dic_map_cur[x][MATR_SIZE - 1] * (1 + sqrt(_center_pos - x) / 2), 1, 254) - random(-1, 2);

	for (int y = 0; y < MATR_SIZE - 1; y++)
	{
		byte left = 2 * SIDE_CENTER - _side_coef_cur[y];
		LOG_USB_FIRE("row: %d\t", y);
		for (int x = 0; x < MATR_SIZE; x++)
		{
			_temp_map[x][y] = (
				_temp_map[x][y + 1] +
				_temp_map[x][y > MATR_SIZE - 3 ? MATR_SIZE - 1 : y + 2] * 1.05 +
				float(_temp_map[constrain(x - 1, 0, MATR_SIZE - 1)][y + 1]) * ((left + random(RANDOM_CENTER - RANDOM_RANGE, RANDOM_CENTER + RANDOM_RANGE + 1) / 10.0) / 10.0) +
				float(_temp_map[constrain(x + 1, 0, MATR_SIZE - 1)][y + 1]) * ((_side_coef_cur[y] + random(RANDOM_CENTER - RANDOM_RANGE, RANDOM_CENTER + RANDOM_RANGE + 1) / 10.0) / 10.0)
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
		_center_pos += byte(random((_center_pos > MATR_SIZE * 2 / 3 ? 1 : 5), (_center_pos < MATR_SIZE / 3 ? 17 : 13))) / 6;
		_center_pos = constrain(_center_pos, 1, MATR_SIZE);
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

	for (int i = 0; i < MATR_SIZE; i++)
		for (int j = 0; j < MATR_SIZE; j++)
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
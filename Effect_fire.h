#ifndef EFFECT_FIRE_H
#define EFFECT_FIRE_H

#include <Arduino.h>
#include "Config.h"
#include "Structures.h"
#include "BaseEffect.h"
#include "ColorBlock.h"
#include "PresetBlock.h"

class Effect_fire : public BaseEffect
{
public:
	Effect_fire(Color_str* leds_arr);

	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	
	void apply_default_option() override;

	virtual Cutoff_str* get_cutoff_str() override;

	void preset(int num);

private:
	static bool _has_init;
	static ColorBlock colorBlock;
	static PresetBlock presetBlock;

	uint32_t temp_to_color(byte temp);
	void dic_map_key_gen();	
	int dic_map_cur_step();	
	void temp_map_gen();

	byte _dic_map_key[MATR_SIZE][MATR_SIZE];
	byte _side_coef_key[MATR_SIZE];	
	byte _dic_map_cur[MATR_SIZE][MATR_SIZE];
	byte _side_coef_cur[MATR_SIZE];
	byte _temp_map[MATR_SIZE][MATR_SIZE];

	byte _center_pos;
	int _center_temp_change;
	byte _center_temp;
	byte _frame_count;

	static Cutoff_str _cutoff_option;
};

#endif
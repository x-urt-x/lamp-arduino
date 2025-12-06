#ifndef EFFECT_FIRE_H
#define EFFECT_FIRE_H

#include <Arduino.h>
#include "Config.h"
#include "Structures.h"
#include "BaseEffect.h"
#include "BaseBlock.h"
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

	void preset(int num);

private:
	static bool _has_init;

	static BaseBlock baseBlock;
	static ColorBlock colorBlock;
	static PresetBlock presetBlock;

	uint32_t temp_to_color(byte temp);

	uint8_t _pos;
	uint8_t _key_temp;
	uint8_t _cur_temp;
};

#endif
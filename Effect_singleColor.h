#ifndef EFFECT_SINGLECOLOR_H
#define EFFECT_SINGLECOLOR_H

#include <Arduino.h>
#include "Structures.h"
#include "Config.h"
#include "BaseEffect.h"
#include "BaseBlock.h"
#include "ColorBlock.h"
#include "RainbowBlock.h"

class Effect_singleColor : public BaseEffect
{
public:
	Effect_singleColor(Color_str* leds_arr);

	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	void apply_default_option() override;
private:
	static bool _has_init;

	static BaseBlock baseBlock;
	static ColorBlock colorBlock;
	static RainbowBlock rainbowBlock;

	int _hue;
};
#endif
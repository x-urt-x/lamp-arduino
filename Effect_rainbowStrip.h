#ifndef EFFECT_RAINBOWSTRIP_H
#define EFFECT_RAINBOWSTRIP_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Structures.h"
#include "Config.h"
#include "BaseEffect.h"
#include "BaseBlock.h"
#include "RainbowBlock.h"

class Effect_rainbowStrip : public BaseEffect
{
public:
	Effect_rainbowStrip(Color_str* leds_arr);

	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	void apply_default_option() override;

private:
	static bool _has_init;

	static BaseBlock baseBlock;
	static RainbowBlock rainbowBlock;

	uint16_t _hue[MATR_LEN];
};

#endif
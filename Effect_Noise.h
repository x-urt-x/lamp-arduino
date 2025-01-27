#ifndef EFFECT_NOISE
#define EFFECT_NOISE

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PerlinNoise.h"
#include "Structures.h"
#include "Config.h"
#include "BaseEffect.h"
#include "BaseBlock.h"
#include "ColorBlock.h"
#include "RainbowBlock.h"

class Effect_Noise : public BaseEffect
{
public:
	Effect_Noise(Color_str* leds_arr);

	//Effectable
	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	void apply_default_option() override;

private:
	static bool _has_init;

	static BaseBlock baseBlock;
	static ColorBlock colorBlock;
	static RainbowBlock rainbowBlock;

	uint16_t _x, _y, _z;
	static const int scale = 35;
	int hue_shift;
};
#endif 
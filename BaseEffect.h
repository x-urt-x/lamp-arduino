#ifndef BASEEFFECT_H
#define BASEEFFECT_H
#include "Structures.h"
#include "Config.h"
#include "IEffectBlock.h"

#define BLOCK(blockName) _blocks[blockName._id] = &blockName

enum EffectIDEnum : byte
{
	singleColor = 0,
	fire,
	rainbowStrip,
	Noise
};

class BaseEffect
{
public:
	BaseEffect(Color_str* leds_arr);

	virtual void setup() = 0;
	virtual void make_frame() = 0;
	virtual String get_effect_name() = 0;
	virtual void apply_default_option() = 0;

	virtual Cutoff_str* get_cutoff_str();

	template<typename BlockClass>
	BlockClass* get_block()
	{
		return static_cast<BlockClass*>(_blocks[BlockClass::_id]);
	}
private:
	static Cutoff_str _cutoff_option;
protected:
	IEffectBlock* _blocks[IEffectBlock::BlockIDEnum::BlocksCount];
	Color_str* _leds_arr;
};
#endif
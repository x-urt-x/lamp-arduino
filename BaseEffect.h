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
	BaseEffect(Color_str* leds_arr, int _strip_update_delay_time = 100, int _br_cutoff_bound = 0, int _effect_step = 1);

	virtual void setup() = 0;
	virtual void make_frame() = 0;
	virtual String get_effect_name() = 0;
	virtual void apply_default_option() = 0;

	virtual Cutoff_str* get_cutoff_str();

	void set_strip_update_delay_time(int strip_update_delay_time);
	void set_br_cutoff_bound(int br_cutoff_bound);
	void set_effect_step(int step);
	int get_strip_update_delay_time();
	int get_br_cutoff_bound();
	int get_effect_step();

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

	int _strip_update_delay_time;
	int _br_cutoff_bound;
	int _effect_step;
};
#endif
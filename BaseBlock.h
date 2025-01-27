#ifndef BASEBLOCK_H
#define BASEBLOCK_H
#include "IEffectBlock.h"

class BaseBlock : public IEffectBlock
{
public:
	BaseBlock(int _strip_update_delay_time = 100, int _br_cutoff_bound = 0, int _effect_step = 1);

	void set_strip_update_delay_time(int strip_update_delay_time);
	void set_br_cutoff_bound(int br_cutoff_bound);
	void set_effect_step(int step);
	int get_strip_update_delay_time();
	int get_br_cutoff_bound();
	int get_effect_step();

	static const IEffectBlock::BlockIDEnum _id;
private:
	int _strip_update_delay_time;
	int _br_cutoff_bound;
	int _effect_step;
};
#endif
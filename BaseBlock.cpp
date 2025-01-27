#include "BaseBlock.h"
   
const IEffectBlock::BlockIDEnum BaseBlock::_id = IEffectBlock::BlockIDEnum::BaseBlockID;

void BaseBlock::set_strip_update_delay_time(int strip_update_delay_time) { _strip_update_delay_time = strip_update_delay_time; }
void BaseBlock::set_br_cutoff_bound(int br_cutoff_bound)
{
    if (br_cutoff_bound < 0) return;
    _br_cutoff_bound = br_cutoff_bound;
}
void BaseBlock::set_effect_step(int step) { _effect_step = step; }
int BaseBlock::get_strip_update_delay_time() { return _strip_update_delay_time; }
int BaseBlock::get_br_cutoff_bound() { return _br_cutoff_bound; }
int BaseBlock::get_effect_step() { return _effect_step; }

BaseBlock::BaseBlock(int _strip_update_delay_time, int _br_cutoff_bound, int _effect_step)
	: _strip_update_delay_time(_strip_update_delay_time), _br_cutoff_bound(_br_cutoff_bound), _effect_step(_effect_step)
{}
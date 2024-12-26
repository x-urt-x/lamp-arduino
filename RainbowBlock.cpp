#include "RainbowBlock.h"

const IEffectBlock::BlockIDEnum RainbowBlock::_id = IEffectBlock::BlockIDEnum::RainbowBlockID;

void RainbowBlock::set_rainbow_step(int step, int num)
{
	if (num >= _states_len) return;
	_steps[num] = step;
}
void RainbowBlock::set_rainbow_state(bool state, int num)
{
	if (num >= _states_len) return;
	_states[num] = state;
}

bool* RainbowBlock::get_rainbow_states() { return _states; }
int* RainbowBlock::get_rainbow_steps() { return _steps; }
int RainbowBlock::get_rainbow_len() { return _states_len; }

RainbowBlock::RainbowBlock(bool* states, int states_len, int* steps)
	:_states(states), _steps(steps), _states_len(states_len)
{
	if (_states_len <= 0) _states_len = 1;
	if (!_states)
	{
		_states = new bool[_states_len];
		for (int i = 0; i < _states_len; i++) _states[i] = true;
	}
	if (!_steps)
	{
		_steps = new int[_states_len];
		for (int i = 0; i < _states_len; i++) _steps[i] = 512;
	}
}

RainbowBlock::~RainbowBlock()
{
	if (_states) delete[] _states;
	if (_steps) delete[] _steps;
}

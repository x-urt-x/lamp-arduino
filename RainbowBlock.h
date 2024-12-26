#ifndef RAINBOWBLOCK_H
#define RAINBOWBLOCK_H
#include "RainbowBlock.h"
#include "IEffectBlock.h"

class RainbowBlock : public IEffectBlock
{
public:
	RainbowBlock(bool* states = nullptr, int states_len = 1, int* steps = nullptr);
	~RainbowBlock();

	void set_rainbow_step(int step, int num = 0);
	void set_rainbow_state(bool state, int num = 0);

	bool* get_rainbow_states();
	int* get_rainbow_steps();
	int get_rainbow_len();

	static const IEffectBlock::BlockIDEnum _id;
private:
	bool* _states;
	int _states_len;
	int* _steps;
};

#endif
#ifndef PRESETBLOCK_H
#define PRESETBLOCK_H

#include "IEffectBlock.h"

class PresetBlock :public IEffectBlock
{
public:
	PresetBlock(String* names = nullptr, int len = 1);
	~PresetBlock();

	int get_preset_len();
	String* get_preset_names();
	void set_preset(int num = 0);
	int get_preset();

	static const IEffectBlock::BlockIDEnum _id;
private:
	String* _names;
	int _len;
	int _to_set;
};
#endif
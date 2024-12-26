#include "PresetBlock.h"

const IEffectBlock::BlockIDEnum PresetBlock::_id = IEffectBlock::BlockIDEnum::PresetBlockID;

int PresetBlock::get_preset_len()
{
	return _len;
}

String* PresetBlock::get_preset_names()
{
	return _names;
}

void PresetBlock::set_preset(int num)
{
	if (num >= _len) return;
	_to_set = num;
}

int PresetBlock::get_preset()
{
	return _to_set;
}



PresetBlock::PresetBlock(String* names, int len)
	:_names(names), _len(len), _to_set(-1)
{
	if (_len <= 0) _len = 1;
	if (!_names)
	{
		_names = new String[_len];
		for (int i = 0; i < _len; i++) _names[i] = "";
	}
}

PresetBlock::~PresetBlock()
{
	if (_names) delete[] _names;
}
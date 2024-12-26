#include "ColorBlock.h"

const IEffectBlock::BlockIDEnum ColorBlock::_id = IEffectBlock::BlockIDEnum::ColorBlockID;

void ColorBlock::set_color(Color_str color, int num)
{
	if (num >= _color_len) return;
	_colors[num].set(color);
}
void ColorBlock::set_color(uint32_t color, int num)
{
	if (num >= _color_len) return;
	_colors[num].set(color);
}
Color_str* ColorBlock::get_colors() { return _colors; }
int ColorBlock::get_color_len() { return _color_len; }

ColorBlock::ColorBlock(Color_str* colors, int color_len)
	:_colors(colors), _color_len(color_len)
{
	if (_color_len <= 0) _color_len = 1;
	if (!_colors)
	{
		_colors = new Color_str[_color_len];
		for (int i = 0; i < _color_len; i++) _colors[i] = {50,50,50};
	}
}

ColorBlock::~ColorBlock()
{
	if (_colors) delete[] _colors;
}

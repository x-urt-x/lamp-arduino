#ifndef COLORBLOCK_H
#define COLORBLOCK_H
#include "IEffectBlock.h"
#include "Structures.h"

class ColorBlock : public IEffectBlock
{
public:
	ColorBlock(Color_str* colors = nullptr, int color_len = 1);
	~ColorBlock();

	void set_color(Color_str color, int num = 0);
	void set_color(uint32_t color, int num = 0);
	Color_str* get_colors();
	int get_color_len();

	static const IEffectBlock::BlockIDEnum _id;
private:
	Color_str* _colors;
	int _color_len;
};
#endif

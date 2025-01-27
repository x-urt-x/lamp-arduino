#ifndef IEFFECTBLOCK_H
#define IEFFECTBLOCK_H
#include <Arduino.h>

class IEffectBlock
{
public:
	enum BlockIDEnum : byte
	{
		BaseBlockID,
		ColorBlockID,
		PresetBlockID,
		RainbowBlockID,
		ParseBlockID,
		BlocksCount
	};
};

#endif
#ifndef IEFFECTBLOCK_H
#define IEFFECTBLOCK_H
#include <Arduino.h>

class IEffectBlock
{
public:
	enum BlockIDEnum : byte
	{
		ColorBlockID,
		PresetBlockID,
		RainbowBlockID,
		BlocksCount
	};
};

#endif
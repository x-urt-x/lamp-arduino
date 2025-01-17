#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H
#include <EEPROM.h>
#include "Config.h"
#include "Structures.h"
#include "log.h"

enum DataObjectIDEnum : byte
{
	Effect = 0,
	Timer
};

struct DataObjAddrArr
{
	DataObjAddrArr(byte len);
	DataObjAddrArr(const DataObjAddrArr& obj);
	~DataObjAddrArr();
	byte len;
	uint16_t* addrArr;
	byte* posArr;
private:
	static unsigned int count;
};

struct MemManager
{
	static uint16_t reservAddr(uint16_t size, DataObjectIDEnum id);
	static void deleteAddr(byte pos);
	static DataObjAddrArr getAllById(DataObjectIDEnum id);
private:
	static void shiftLeft(byte posTo, byte shift);
	static bool shiftRight(byte posFrom, byte shift);
};

#endif
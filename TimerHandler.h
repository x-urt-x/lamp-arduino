#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H
#include "MemManager.h"
#include "IEventTimer.h"

#include "BrEventTimer.h"
#include "EffectEventTimer.h"
#include "OnOffTimer.h"
#include "CommandTimer.h"

class TimerHandler
{
public:
	TimerHandler();
	
	void parseTimer(char* input_str);

	void tickAll();
	void addActiveTimer(IEventTimer* timerDataHolder);

	void addActiveAllFromMem();

	void deleteActiveTimer(byte num);
	void deleteMemTimer(byte num);

	IDataHolderArr getMemDataAll();
	IDataHolder* getMemData(uint16_t adrr);
	
	JsonDocument getMemJsonAll();
	JsonDocument getActiveJsonAll();

	void setActiveState(bool state, int num);
	bool getActiveState(int num);
	int getTimerCount();
private:
	IEventTimer** timers;
	byte _timers_count = 0;
	byte _timers_cap = 3;
};
#endif
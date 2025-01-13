#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H
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
	void addTimer(IEventTimer* timerDataHolder);
	void deleteTimer(byte num);

	void createAll();

	IDataHolderArr loadAll(IDataHolderArr &dataArr);
	IDataHolder* loadTimer(uint16_t adrr);
	
	JsonDocument getMemJsonAll();
	JsonDocument getActiveJsonAll();

	void setState(bool state, int num);
	bool getState(int num);
	int getTimerCount();
private:
	IEventTimer** timers;
	byte _timers_count = 0;
	byte _timers_cap = 3;
};
#endif
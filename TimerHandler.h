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
	
	void parse(const char* input_str);

	void tickAll();
	//unsafe num
	void tickSingle(byte num, unsigned long cur_time);
	void addActiveTimer(IEventTimer* timerDataHolder);

	void addActiveAllFromMem();

	void deleteActiveTimer(byte num);
	void deleteMemTimer(byte num);

	IDataHolderArr getMemDataAll();
	IDataHolder* getMemData(uint16_t adrr);
	
	void getMemJsonAll(JsonArray& memTimers);
	void getActiveJsonAll(JsonArray& actimeTimers);

	void setActiveState(bool state, int num);
	bool getActiveState(int num);
	int getTimerCount();
private:
	IEventTimer** timers;
	byte _timers_count = 0;
	byte _timers_cap = 3;
};
#endif
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
	
	void addTimer(char* input_str);

	void tickAll();
	void addTimer(IEventTimer* timer);
	void deleteTimer(byte num);

	void setState(bool state, int num);
	bool getState(int num);
	int getTimerCount();
private:
	IEventTimer** timers;
	byte _timers_count = 0;
	byte _timers_cap = 3;
};
#endif
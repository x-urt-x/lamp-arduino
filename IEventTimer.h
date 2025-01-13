#ifndef IEVENTTIMER_H
#define IEVENTTIMER_H
#include "Strip_class.h"
#include "StartTimeInfo.h"

class IEventTimer
{
public:
	enum TimerIDEnum : byte
	{
		BrEventTimer = 0,
		CommandTimer,
		EffectEventTimer,
		OnOffTimer
	};

	IEventTimer(const uint _delay, bool _is_active, unsigned long _prev_time, uint16_t addr = 0);
	IEventTimer();
	virtual bool tick(unsigned long cur_time);
	//virtual uint16_t save(uint16_t addr) = 0; //return next free adrr 
	uint _delay;
	bool _is_active;
	uint16_t _addr = 0;
	static Strip* obj;
protected:
	virtual bool action() = 0;
	unsigned long _prev_time;
};

struct IDataHolder
{
	virtual IEventTimer* create() = 0;
	virtual void save() = 0;
	virtual void load(uint16_t addr) = 0;
	void setTime(unsigned long time, bool epoch);
	byte _repInfo;
	unsigned long _timer_time_raw;
protected:
	uint16_t _addr = 0;
	unsigned long calcTime();
	uint16_t reservAddr(uint16_t size);
	void saveCommon(uint16_t& paddr, IEventTimer::TimerIDEnum id);
	void loadCommon(uint16_t& paddr);
};
#endif
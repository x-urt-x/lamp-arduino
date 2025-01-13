#ifndef IEVENTTIMER_H
#define IEVENTTIMER_H
#include "Strip_class.h"
#include <ArduinoJson.h>
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
	virtual void getJson(JsonObject& doc) = 0;
	virtual byte getId() = 0;
	uint _delay;
	bool _is_active;
	uint16_t _addr = 0;
	static Strip* obj;
protected:
	void getJsonCommon(JsonObject& doc);
	virtual bool action() = 0;
	unsigned long _prev_time;
};

struct IDataHolder
{
	virtual IEventTimer* create() = 0;
	virtual void save() = 0;
	virtual void load(uint16_t addr) = 0;
	virtual void getJson(JsonObject& doc) = 0;
	virtual byte getId() = 0;
	void setTime(unsigned long time, bool epoch);
	byte _repInfo;
	unsigned long _timer_time_raw;
protected:
	uint16_t _addr = 0;
	unsigned long calcTime();
	uint16_t reservAddr(uint16_t size);
	void saveCommon(uint16_t& paddr, IEventTimer::TimerIDEnum id);
	void loadCommon(uint16_t& paddr);
	void getJsonCommon(JsonObject& doc);
};

struct IDataHolderArr
{
	void free();
	byte len = 0;
	IDataHolder** arr = nullptr;
};
#endif
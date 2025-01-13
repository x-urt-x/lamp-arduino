#ifndef BREVENTTIMER_H
#define BREVENTTIMER_H

#include "IEventTimer.h"
#include "log.h"
class BrEventTimer : public IEventTimer
{
public:
	BrEventTimer(unsigned long from_time, unsigned long dur, uint16_t to_br, uint delay, uint16_t addr = 0);
protected:
	virtual bool action() override;
private:
	int _step_counter;
	uint16_t _to_br;
};

struct BrEventTimerDataHolder : public IDataHolder
{
	virtual IEventTimer* create() override;
	virtual void save() override;
	virtual void load(uint16_t addr) override;
	virtual void getJson(JsonObject& doc) override;
	virtual byte getId() override;

	unsigned long _dur;
	uint16_t _to_br;
	uint _delay;
};

#endif
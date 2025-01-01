#ifndef IEVENTTIMER_H
#define IEVENTTIMER_H
#include "Strip_class.h"
class IEventTimer
{
public:
	IEventTimer(const uint _delay, bool _is_active, unsigned long _prev_time);
	IEventTimer();
	virtual bool tick(unsigned long cur_time);
	uint _delay;
	bool _is_active;
	static Strip* obj;
protected:
	virtual bool action() = 0;
	unsigned long _prev_time;
};
#endif
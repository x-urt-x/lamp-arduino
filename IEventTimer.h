#ifndef IEVENTTIMER_H
#define IEVENTTIMER_H
#include "Strip_class.h"
class IEventTimer
{
public:
	virtual bool tick(unsigned long cur_time);
	uint _delay = 0;
	bool _is_active = true;
	static Strip* obj;
protected:
	virtual bool action() = 0;
	unsigned long _prev_time = 0;
};
#endif
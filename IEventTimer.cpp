#include "IEventTimer.h"

bool IEventTimer::tick(unsigned long cur_time)
{
	if (_prev_time > cur_time) return false;
	if (cur_time - _prev_time > _delay)
	{
		_prev_time = cur_time;
		return action();
	}
	return false;
}
Strip* IEventTimer::obj = nullptr;
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

IEventTimer::IEventTimer(const uint _delay, bool _is_active, unsigned long _prev_time): _delay(_delay), _is_active(_is_active), _prev_time(_prev_time)
{
}

IEventTimer::IEventTimer() : _delay(100), _is_active(true), _prev_time(0)
{
}

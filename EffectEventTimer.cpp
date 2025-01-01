#include "EffectEventTimer.h"

bool EffectEventTimer::action()
{
	obj->tick();
	return false;
}

bool EffectEventTimer::tick(unsigned long cur_time)
{
	//LOG_USB_TIMER("EffectEventTimer::tick\n");
	if (cur_time - _prev_time > obj->get_strip_update_delay_time())
	{
		_prev_time = cur_time;
		return action();
	}
	return false;
}
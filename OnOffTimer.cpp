#include "OnOffTimer.h"

OnOffTimer::OnOffTimer(bool* target, bool to_set, unsigned long time) :_target(target), _to_set(to_set), IEventTimer(0, true, time)
{
}

bool OnOffTimer::tick(unsigned long cur_time)
{
	if (cur_time > _prev_time)
	{
		return action();
	}
	return false;
}

bool OnOffTimer::action()
{
	*_target = _to_set;
	if (_to_set == 0)
	{
		digitalWrite(MOSFET_PIN, LOW);
		//WiFi.forceSleepBegin();
	}
	else
	{
		//WiFi.forceSleepWake();
		digitalWrite(MOSFET_PIN, HIGH);
	}
	return true;
}

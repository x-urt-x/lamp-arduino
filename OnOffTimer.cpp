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
	LOG_USB_TIMER("OnOffTimer set to %d", _to_set);
	*_target = _to_set;
	if (_to_set == 0)
	{
#ifdef MATR16x16
		digitalWrite(MOSFET_PIN, LOW);
#endif
#ifdef MATR10x10
		obj->fill(obj->Color(0, 0, 0));
		delay(0);
		obj->show();
#endif 

		//WiFi.forceSleepBegin();
	}
	else
	{
		//WiFi.forceSleepWake();
#ifdef MATR16x16
		digitalWrite(MOSFET_PIN, HIGH);
#endif
	}
	return true;
}

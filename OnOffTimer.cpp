#include "OnOffTimer.h"

OnOffTimer::OnOffTimer(unsigned long time, bool* target, bool to_set, uint16_t addr) :_target(target), _to_set(to_set), IEventTimer(0, true, time, addr)
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

void OnOffTimer::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["to_set"] = String(_to_set);
}

byte OnOffTimer::getId()
{
	return IEventTimer::TimerIDEnum::OnOffTimer;
}


IEventTimer* OnOffTimerDataHolder::create()
{
	return new OnOffTimer(calcTime(), _target, _to_set);
}

void OnOffTimerDataHolder::save()
{
	_addr = reservAddr(sizeof(_to_set) + sizeof(_target));
	uint16_t addr = _addr;
	saveCommon(addr, IEventTimer::TimerIDEnum::OnOffTimer);
	EEPROM.put(addr, _to_set);
	addr += sizeof(_to_set);
	EEPROM.put(addr, _target);
	EEPROM.commit();
}

void OnOffTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id

	loadCommon(addr);
	EEPROM.get(addr, _to_set);
	addr += sizeof(_to_set);
	EEPROM.get(addr, _target);
}

void OnOffTimerDataHolder::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["to_set"] = String(_to_set);
}

byte OnOffTimerDataHolder::getId()
{
	return IEventTimer::TimerIDEnum::OnOffTimer;
}

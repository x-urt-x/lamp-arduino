#include "OnOffTimer.h"

#include "InputHandler.h"

OnOffTimer::OnOffTimer(unsigned long time, bool is_active, bool to_set, uint16_t addr) :_to_set(to_set), IEventTimer(0, is_active, time, addr)
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
	if (_to_set)
	{
		InputHandler::parseSingleCommand("ts 0 1");
	}
	else
	{
		InputHandler::parseSingleCommand("ts 0 0");
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
	return new OnOffTimer(calcTime(), _is_active, _to_set , _addr);
}

void OnOffTimerDataHolder::save()
{
	uint16_t addr = reservAddr(sizeof(_to_set));
	if (!addr) return;
	_addr = addr;
	saveCommon(addr, IEventTimer::TimerIDEnum::OnOffTimer);
	EEPROM.put(addr, _to_set);
	addr += sizeof(_to_set);
	EEPROM.commit();
}

void OnOffTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id

	loadCommon(addr);
	EEPROM.get(addr, _to_set);
	addr += sizeof(_to_set);
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

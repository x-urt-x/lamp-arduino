#include "BrEventTimer.h"

bool BrEventTimer::action()
{
	LOG_USB_TIMER("BrEventTimer::action\n");
	int br = obj->get_br();
	int to_set = br + (_to_br - br) / _step_counter;
	LOG_USB_TIMER("br = %d\t_to_br = %d\t_step_counter = %d\tbr to set = %d\n", br, _to_br, _step_counter, to_set);
	obj->set_br(to_set);
	_step_counter--;
	if (_step_counter <= 0)
	{
		LOG_USB_TIMER("end\n");
		return true;
	}
	return false;
}

void BrEventTimer::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["step_counter"] = String(_step_counter);
	doc["to_br"] = String(_to_br);
}

byte BrEventTimer::getId()
{
	return IEventTimer::TimerIDEnum::BrEventTimer;
}

BrEventTimer::BrEventTimer(unsigned long from_time, bool is_active, unsigned long dur, uint16_t to_br, uint delay, uint16_t addr) : _to_br(to_br), IEventTimer(delay, is_active, from_time, addr)
{
	_step_counter = float(dur*1000) / delay + 1;
	LOG_USB_TIMER("create BrEventTimer: now = %d from_time = %d dur = %d to_br = %d delay = %d _step_counter = %d\n", millis(), _prev_time, dur, to_br, delay, _step_counter);
}

IEventTimer* BrEventTimerDataHolder::create()
{
	return new BrEventTimer(calcTime(), _is_active, _dur, _to_br, _delay, _addr);
}

void BrEventTimerDataHolder::save()
{
	uint16_t addr = reservAddr(sizeof(_dur) + sizeof(_to_br) + sizeof(_delay));
	if (!addr) return;
	_addr = addr;
	saveCommon(addr, IEventTimer::TimerIDEnum::BrEventTimer);
	EEPROM.put(addr, _dur);
	addr += sizeof(_dur);
	EEPROM.put(addr, _to_br);
	addr += sizeof(_to_br);
	EEPROM.put(addr, _delay);
	EEPROM.commit();
}

void BrEventTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id
	loadCommon(addr);
	EEPROM.get(addr, _dur);
	addr += sizeof(_dur);
	EEPROM.get(addr, _to_br);
	addr += sizeof(_to_br);
	EEPROM.get(addr, _delay);
}

void BrEventTimerDataHolder::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["dur"] = String(_dur);
	doc["to_br"] = String(_to_br);
	doc["delay"] = String(_delay);
}

byte BrEventTimerDataHolder::getId()
{
	return IEventTimer::TimerIDEnum::BrEventTimer;
}

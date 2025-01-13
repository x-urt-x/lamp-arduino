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

BrEventTimer::BrEventTimer(unsigned long from_time, unsigned long dur, uint16_t to_br, uint delay, uint16_t addr) : _to_br(to_br), IEventTimer(delay, true, from_time, addr)
{
	_step_counter = float(dur*1000) / delay + 1;
	LOG_USB_TIMER("create BrEventTimer: now = %d from_time = %d dur = %d to_br = %d delay = %d _step_counter = %d\n", millis(), _prev_time, dur, to_br, delay, _step_counter);
}

IEventTimer* BrEventTimerDataHolder::create()
{
	return new BrEventTimer(calcTime(), _dur, _to_br, _delay, _addr);
}

void BrEventTimerDataHolder::save()
{
	_addr = reservAddr(sizeof(_dur) + sizeof(_to_br) + sizeof(_delay));

	saveCommon(_addr, IEventTimer::TimerIDEnum::BrEventTimer);
	EEPROM.put(_addr, _dur);
	_addr += sizeof(_dur);
	EEPROM.put(_addr, _to_br);
	_addr += sizeof(_to_br);
	EEPROM.put(_addr, _delay);
	EEPROM.commit();
}

void BrEventTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id

	loadCommon(addr);
	EEPROM.get(_addr, _dur);
	_addr += sizeof(_dur);
	EEPROM.get(_addr, _to_br);
	_addr += sizeof(_to_br);
	EEPROM.get(_addr, _delay);
}

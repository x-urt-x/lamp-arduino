#include "TimerHandler.h"

void TimerHandler::addTimer(IEventTimer* timer)
{
	LOG_USB_TIMER("TimerHandler: start add timer\n");
	if (!IEventTimer::obj)
	{
		LOG_USB_TIMER("nullptr obj\n");
		return;
	}
	if (_timers_count == _timers_cap)
	{
		LOG_USB_TIMER("extend array\n");
		_timers_cap += 2;
		IEventTimer** new_timers = new IEventTimer * [_timers_cap];
		for (int i = 0; i < _timers_count; i++)
		{
			new_timers[i] = timers[i];
		}
		delete[] timers;
		timers = new_timers;
	}
	timers[_timers_count] = timer;
	_timers_count++;
	LOG_USB_TIMER("TimerHandler: end add timer\t count = %d\n", _timers_count);
}


void TimerHandler::deleteTimer(byte num)
{
	LOG_USB_TIMER("TimerHandler: delete timer %d\n", num);
	delete timers[num];
	for (int i = num + 1; i < _timers_count; i++)
	{
		timers[i - 1] = timers[i];
	}
	_timers_count--;
}

void TimerHandler::loadAll()
{
	byte obj_data_count = EEPROM.read(0);
	for (byte i = 0; i < obj_data_count; i++)
	{
		uint16_t adrr;
		EEPROM.get(1 + i * 2, adrr);
		if ((adrr & 0xF000) == 0x2000)
		{
			loadTimer(adrr & 0x0FFF);
		}
	}
}

void TimerHandler::loadTimer(uint16_t addr)
{
	LOG_USB_TIMER("load timer on %d", addr);
	byte timerID = EEPROM.read(addr);


	switch (timerID)
	{
	case IEventTimer::TimerIDEnum::BrEventTimer:
	{
		BrEventTimerDataHolder dataHolder;
		dataHolder.load(addr);
		addTimer(dataHolder.create());
		break;
	}
	case IEventTimer::TimerIDEnum::OnOffTimer:
	{
		OnOffTimerDataHolder dataHolder;
		dataHolder.load(addr);
		addTimer(dataHolder.create());
		break;
	}
	case IEventTimer::TimerIDEnum::CommandTimer:
	{
		CommandTimerDataHolder dataHolder;
		dataHolder.load(addr);
		addTimer(dataHolder.create());
		break;
	}
	default:
		break;
	}
}

void TimerHandler::setState(bool state, int num)
{
	LOG_USB_TIMER("TimerHandler::setState %d %d\n", state, num);
	if (num >= _timers_count) return;
	timers[num]->_is_active = state;
}

bool TimerHandler::getState(int num)
{
	if (num >= _timers_count) return false;
	return timers[num]->_is_active;
}

int TimerHandler::getTimerCount()
{
	return _timers_count;
}

TimerHandler::TimerHandler()
{
	timers = new IEventTimer * [_timers_cap];
}

void TimerHandler::parseTimer(char* input_str)
{
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 'b':
	{
		input_str++;
		BrEventTimerDataHolder dataholder;

		bool save;
		parseIn_int(save);
		parseIn_int(dataholder._repInfo);
		parseIn_int(dataholder._timer_time_raw);
		dataholder.setTime(dataholder._timer_time_raw, !dataholder._repInfo);
		parseIn_int(dataholder._dur);
		parseIn_int(dataholder._to_br);
		parseIn_int(dataholder._delay);


		LOG_USB_TIMER("save = %d _repInfo = %d _timer_time_raw = %d _dur = %d _to_br = %d _delay = %d \n", save, dataholder._repInfo, dataholder._timer_time_raw, dataholder._dur, dataholder._to_br, dataholder._delay);
		if (save) dataholder.save();

		addTimer(dataholder.create());
		break;
	}
	case 's':
	{
		input_str++;
		OnOffTimerDataHolder dataholder;

		bool save;
		parseIn_int(save);
		parseIn_int(dataholder._repInfo);
		parseIn_int(dataholder._timer_time_raw);
		dataholder.setTime(dataholder._timer_time_raw, dataholder._repInfo);
		parseIn_int(dataholder._to_set);
		dataholder._target = &(timers[0]->_is_active);

		LOG_USB_TIMER("save = %d _repInfo = %d _timer_time_raw = %d _to_set = %d \n", save, dataholder._repInfo, dataholder._timer_time_raw, dataholder._to_set);
		if (save) dataholder.save();

		addTimer(dataholder.create());
		break;
	}
	case 'c':
	{
		input_str++;
		CommandTimerDataHolder dataholder;

		bool save;
		parseIn_int(save);
		parseIn_int(dataholder._repInfo);
		parseIn_int(dataholder._timer_time_raw);
		dataholder.setTime(dataholder._timer_time_raw, dataholder._repInfo);
		parseIn_int(dataholder._delay);
		parseIn_int(dataholder._once);
		input_str += 2;
		dataholder._command = input_str;

		LOG_USB_TIMER("save = %d _repInfo = %d _timer_time_raw = %d _delay = %d _once = %d \n", save, dataholder._repInfo, dataholder._timer_time_raw, dataholder._delay, dataholder._once);

		addTimer(dataholder.create());
		break;
	}
	default:
		break;
	}
}

void TimerHandler::tickAll()
{
	unsigned long cur_time = millis();
	for (int i = 0; i < _timers_count; i++)
	{
		if (timers[i]->_is_active)
		{
			//LOG_USB_TIMER("tick %d timer\n", i);
			if (timers[i]->tick(cur_time)) deleteTimer(i);
		}
	}
}



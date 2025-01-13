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

void TimerHandler::createAll()
{
	IDataHolderArr dataArr;
	loadAll(dataArr);
	for (byte i = 0; i < dataArr.len; i++)
	{
		addTimer(dataArr.arr[i]->create());
	}
	dataArr.free();
}

IDataHolderArr TimerHandler::loadAll(IDataHolderArr &dataArr)
{
	byte obj_data_count = EEPROM.read(0);
	for (byte i = 0; i < obj_data_count; i++)
	{
		uint16_t adrr;
		EEPROM.get(1 + i * 2, adrr);
		if ((adrr & 0xF000) == 0x2000)
		{
			dataArr.len++;
		}
	}
	LOG_USB_TIMER("%d mem timers to load\n", dataArr.len);
	dataArr.arr = new IDataHolder * [dataArr.len];
	byte j = 0;
	for (byte i = 0; i < obj_data_count; i++)
	{
		uint16_t adrr;
		EEPROM.get(1 + i * 2, adrr);
		if ((adrr & 0xF000) == 0x2000)
		{
			IDataHolder* dataHolder = loadTimer(adrr & 0x0FFF);
			if (!dataHolder)
			{
				LOG_USB_TIMER("bad timer id\n");
				dataArr.len--;
				continue;
			}
			dataArr.arr[j] = dataHolder; 
			j++;
		}
	}
	LOG_USB_TIMER("%d mem timers have been loaded\n", dataArr.len);
	LOG_USB_TIMER("end load\n");
	return dataArr;
}

IDataHolder* TimerHandler::loadTimer(uint16_t addr)
{
	byte timerID = EEPROM.read(addr);

	LOG_USB_TIMER("load timer on %d\t timerID = %d\n", addr, timerID);

	switch (timerID)
	{
	case IEventTimer::TimerIDEnum::BrEventTimer:
	{
		LOG_USB_TIMER("load BrEventTimerDataHolder\n");
		BrEventTimerDataHolder* dataHolder = new BrEventTimerDataHolder();
		dataHolder->load(addr);
		return dataHolder;
	}
	case IEventTimer::TimerIDEnum::OnOffTimer:
	{
		LOG_USB_TIMER("load OnOffTimerDataHolder\n");
		OnOffTimerDataHolder* dataHolder = new OnOffTimerDataHolder();
		dataHolder->load(addr);
		return dataHolder;
	}
	case IEventTimer::TimerIDEnum::CommandTimer:
	{
		LOG_USB_TIMER("load CommandTimerDataHolder\n");
		CommandTimerDataHolder* dataHolder = new CommandTimerDataHolder();
		dataHolder->load(addr);
		return dataHolder;
	}
	default:
		break;
	}
	return nullptr;
}

JsonDocument TimerHandler::getMemJsonAll()
{
	JsonDocument doc;
	JsonArray memTimers = doc.createNestedArray("memTimers");
	IDataHolderArr dataArr;
	loadAll(dataArr);
	for (byte i = 0; i < dataArr.len; i++)
	{
		LOG_USB_TIMER("json for %d mem timer start\n", i);
		JsonObject memTimer = memTimers.createNestedObject();
		if(dataArr.arr[i] == nullptr) LOG_USB_TIMER("nullptr\n");
		memTimer["id"] = String(dataArr.arr[i]->getId());
		dataArr.arr[i]->getJson(memTimer);
	}
	dataArr.free();
	LOG_USB_TIMER("mem json create end\n");
	return doc;
}

JsonDocument TimerHandler::getActiveJsonAll()
{
	JsonDocument doc;
	JsonArray actimeTimers = doc.createNestedArray("activeTimers");
	for (int i = 0; i < _timers_count; i++)
	{
		LOG_USB_TIMER("json for %d active timer start\n", i);
		JsonObject activeTimer = actimeTimers.createNestedObject();
		if (timers[i] == nullptr) LOG_USB_TIMER("nullptr\n");
		activeTimer["id"] = String(timers[i]->getId());
		timers[i]->getJson(activeTimer);
	}
	LOG_USB_TIMER("active json create end\n");
	return doc;
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
	case 'g':
	{
		BrEventTimerDataHolder dataholder;

		dataholder._repInfo = (byte)0x12;
		dataholder._timer_time_raw = 0x1234'5678;
		dataholder._dur = 0x1234'5678;
		dataholder._to_br = 0x1234;
		dataholder._delay = 0x1234'5678;


		LOG_USB_TIMER("_repInfo = %d _timer_time_raw = %d _dur = %d _to_br = %d _delay = %d \n",dataholder._repInfo, dataholder._timer_time_raw, dataholder._dur, dataholder._to_br, dataholder._delay);
		dataholder.save();

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



#include "TimerHandler.h"

void TimerHandler::addActiveTimer(IEventTimer* timer)
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


void TimerHandler::deleteActiveTimer(byte num)
{
	if (num >= _timers_count) return;
	LOG_USB_TIMER("TimerHandler: delete active timer %d\n", num);
	delete timers[num];
	for (int i = num + 1; i < _timers_count; i++)
	{
		timers[i - 1] = timers[i];
	}
	_timers_count--;
}

void TimerHandler::addActiveAllFromMem()
{
	IDataHolderArr dataArr = getMemDataAll();
	for (byte i = 0; i < dataArr.len; i++)
	{
		if (dataArr.arr[i]->getId() == IEventTimer::TimerIDEnum::OnOffTimer)
		{
			//update ref
			OnOffTimer* timer = static_cast<OnOffTimer*>(dataArr.arr[i]->create());
			timer->_target = &(timers[0]->_is_active);
			addActiveTimer(timer);
		}
		else
		{
			addActiveTimer(dataArr.arr[i]->create());
		}
	}
}

void TimerHandler::deleteMemTimer(byte num)
{
	LOG_USB_TIMER("TimerHandler: delete mem timer %d\n", num);
	DataObjAddrArr timersDataAddrArr = MemManager::getAllById(DataObjectIDEnum::Timer);
	if (num >= timersDataAddrArr.len) return;
	MemManager::deleteAddr(timersDataAddrArr.posArr[num]);
}

IDataHolderArr TimerHandler::getMemDataAll()
{
	DataObjAddrArr timersDataAddrArr = MemManager::getAllById(DataObjectIDEnum::Timer);
	LOG_USB_TIMER("%d mem timers to load\n", timersDataAddrArr.len);
	IDataHolderArr dataArr(timersDataAddrArr.len);
	for (byte i = 0; i < timersDataAddrArr.len; i++)
	{
		IDataHolder* dataHolder = getMemData(timersDataAddrArr.addrArr[i]);
		if (!dataHolder)
		{
			LOG_USB_TIMER("bad timer id\n");
			dataArr.len--;
			continue;
		}
		dataArr.arr[i] = dataHolder;
	}
	return dataArr;
}

IDataHolder* TimerHandler::getMemData(uint16_t addr)
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

void TimerHandler::getMemJsonAll(JsonArray& memTimers)
{
	IDataHolderArr dataArr = getMemDataAll();
	for (byte i = 0; i < dataArr.len; i++)
	{
		LOG_USB_TIMER("json for %d mem timer start\n", i);
		JsonObject memTimer = memTimers.createNestedObject();
		if (dataArr.arr[i] == nullptr) LOG_USB_TIMER("nullptr\n");
		memTimer["id"] = IEventTimer::getIdString(dataArr.arr[i]->getId());
		dataArr.arr[i]->getJson(memTimer);
	}
	LOG_USB_TIMER("mem json create end\n");
}

void TimerHandler::getActiveJsonAll(JsonArray& actimeTimers)
{
	for (int i = 0; i < _timers_count; i++)
	{
		LOG_USB_TIMER("json for %d active timer start\n", i);
		JsonObject activeTimer = actimeTimers.createNestedObject();
		if (timers[i] == nullptr) LOG_USB_TIMER("nullptr\n");
		activeTimer["id"] = IEventTimer::getIdString(timers[i]->getId());
		timers[i]->getJson(activeTimer);
	}
	LOG_USB_TIMER("active json create end\n");
}

void TimerHandler::setActiveState(bool state, int num)
{
	LOG_USB_TIMER("TimerHandler::setState %d %d\n", state, num);
	if (num >= _timers_count) return;
	timers[num]->_is_active = state;
}

bool TimerHandler::getActiveState(int num)
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

void TimerHandler::parse(char* input_str)
{
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 's':
	{
		byte pos;
		parseIn_int(pos);
		bool state;
		parseIn_int(state);
		setActiveState(state, pos);

		if (pos == 0)
			if (state == 0)
			{
#ifdef MATR16x16
				digitalWrite(MOSFET_PIN, LOW);
#endif
#ifdef MATR10x10
				strip.fill(strip.Color(0, 0, 0));
				delay(0);
				strip.show();
				return;
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
		break;
	}
	case 'd':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 'a':
		{
			byte pos;
			parseIn_int(pos);
			if (pos == 0)
			{
				LOG_USB_TIMER("cancel delete main timer\n");
				break;
			}
			deleteActiveTimer(pos);
			break;
		}
		case 'm':
		{
			byte pos;
			parseIn_int(pos);
			deleteMemTimer(pos);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'a':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 'b':
		{
			input_str++;
			BrEventTimerDataHolder dataholder;

			bool save;
			parseIn_int(save);
			parseIn_int(dataholder._is_active);
			parseIn_int(dataholder._repInfo);
			parseIn_int(dataholder._timer_time_raw);
			dataholder.setTime(dataholder._timer_time_raw, !dataholder._repInfo);
			parseIn_int(dataholder._delay);
			parseIn_int(dataholder._dur);
			parseIn_int(dataholder._to_br);


			LOG_USB_TIMER("save = %d _is_active = %d _repInfo = %d _timer_time_raw = %d _dur = %d _to_br = %d _delay = %d \n", save, dataholder._is_active, dataholder._repInfo, dataholder._timer_time_raw, dataholder._dur, dataholder._to_br, dataholder._delay);
			if (save) dataholder.save();

			addActiveTimer(dataholder.create());
			break;
		}
		case 's':
		{
			input_str++;
			OnOffTimerDataHolder dataholder;

			bool save;
			parseIn_int(save);
			parseIn_int(dataholder._is_active);
			parseIn_int(dataholder._repInfo);
			parseIn_int(dataholder._timer_time_raw);
			dataholder.setTime(dataholder._timer_time_raw, dataholder._repInfo);
			parseIn_int(dataholder._to_set);
			dataholder._target = &(timers[0]->_is_active);

			LOG_USB_TIMER("save = %d _is_active = %d _repInfo = %d _timer_time_raw = %d _to_set = %d \n", save, dataholder._is_active, dataholder._repInfo, dataholder._timer_time_raw, dataholder._to_set);
			if (save) dataholder.save();

			addActiveTimer(dataholder.create());
			break;
		}
		case 'c':
		{
			input_str++;
			CommandTimerDataHolder dataholder;

			bool save;
			parseIn_int(save);
			parseIn_int(dataholder._is_active);
			parseIn_int(dataholder._repInfo);
			parseIn_int(dataholder._timer_time_raw);
			dataholder.setTime(dataholder._timer_time_raw, dataholder._repInfo);
			parseIn_int(dataholder._delay);
			parseIn_int(dataholder._once);
			input_str += 1;
			dataholder._command = input_str;

			LOG_USB_TIMER("save = %d _is_active = %d _repInfo = %d _timer_time_raw = %d _delay = %d _once = %d \n", save, dataholder._is_active, dataholder._repInfo, dataholder._timer_time_raw, dataholder._delay, dataholder._once);
			if (save) dataholder.save();

			addActiveTimer(dataholder.create());
			break;
		}
		default:
			break;
		}
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
			if (timers[i]->tick(cur_time))
			{
				LOG_USB_TIMER("delete timer %d\n", i);
				if (timers[i]->_addr)
				{
					IDataHolder* timerdata = getMemData(timers[i]->_addr);
					LOG_USB_TIMER("timer %d with rep = %d on addr %d\n", i, timerdata->_repInfo, timers[i]->_addr);
					if (timerdata->_repInfo)
					{
						addActiveTimer(timerdata->create());
					}
					else
					{
						LOG_USB_TIMER("no rep, delete form mem\n");
						timerdata->deleteMem();
						MemManager::deleteAllAddrOnDelitedDataObj();
					}
				}
				deleteActiveTimer(i);
			}
		}
	}
}



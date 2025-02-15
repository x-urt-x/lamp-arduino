#include "IEventTimer.h"

void IDataHolder::setTime(unsigned long time, bool epoch)
{
	if (epoch)
	{
		_timer_time_raw = time + millis() / 1000 + StartTimeInfo::start_epoch_time;
	}
	else
	{
		_timer_time_raw = time;
	}
}

void IDataHolder::deleteMem()
{
	if (_addr) EEPROM.write(_addr, IEventTimer::TimerIDEnum::Delited);
}

unsigned long IDataHolder::calcTime()
{
	unsigned long now_millis_time = millis();
	if (_repInfo)
	{
		byte now_weekday = StartTimeInfo::start_weekday + (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) / 86400UL - 1;
		unsigned long now_daytime = (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) % 86400UL;
		LOG_USB_TIMER("calcTime with rep: now_weekday = %d now_daytime = %d repInfo = %d time = %lu\n", now_weekday, now_daytime, _repInfo, _timer_time_raw);
		for (byte i = now_weekday; i < 7; i++)
		{
			if ((_repInfo >> i) & 0b1)
			{
				if (_timer_time_raw < now_daytime && i == now_weekday)
					continue;
				LOG_USB_TIMER("calcTime with rep 1: i = %d\n", i);
				return now_millis_time + (86400UL * (i - now_weekday) + _timer_time_raw - now_daytime) * 1000UL + 1;
			}
		}
		for (byte i = 0; i <= now_weekday; i++)
		{
			if ((_repInfo >> i) & 0b1)
			{
				LOG_USB_TIMER("calcTime with rep 2: i = %d\n", i);
				return now_millis_time + (86400UL * (7 - now_weekday + i) + _timer_time_raw - now_daytime) * 1000UL + 1;
			}
		}
	}
	else
	{
		return (_timer_time_raw - StartTimeInfo::start_epoch_time) * 1000UL + StartTimeInfo::start_millis_time;
	}
	return 0;
}

uint16_t IDataHolder::reservAddr(uint16_t uniquePartSize)
{
	return MemManager::reservAddr(uniquePartSize + 6, DataObjectIDEnum::Timer);
}

void IDataHolder::saveCommon(uint16_t &paddr, IEventTimer::TimerIDEnum id)
{
	EEPROM.write(paddr, id);
	paddr++;
	EEPROM.write(paddr, _repInfo | _is_active << 7);
	paddr++;
	EEPROM.put(paddr, _timer_time_raw);
	paddr += 4;
}

void IDataHolder::loadCommon(uint16_t& paddr)
{
	_repInfo = EEPROM.read(paddr);
	_is_active = _repInfo >> 7;
	_repInfo &= 0b0111'1111;
	paddr++;
	EEPROM.get(paddr, _timer_time_raw);
	paddr += 4;
}

void IDataHolder::getJsonCommon(JsonObject& doc)
{
	doc["is_active"] = String(_is_active);
	doc["repInfo"] = String(_repInfo);
	doc["timer_time_raw"] = String(_timer_time_raw);
}


bool IEventTimer::tick(unsigned long cur_time)
{
	if (_prev_time > cur_time) return false;
	if (cur_time > _prev_time + _delay || cur_time == 0)
	{
		_prev_time = cur_time;
		return action();
	}
	return false;
}
String IEventTimer::getIdString(byte id)
{
	switch (id)
	{
	case IEventTimer::TimerIDEnum::BrEventTimer:
		return "Brightness timer";
	case IEventTimer::TimerIDEnum::CommandTimer:
		return "Command timer";
	case IEventTimer::TimerIDEnum::EffectEventTimer:
		return "Main timer";
	case IEventTimer::TimerIDEnum::OnOffTimer:
		return "OnOff timer";
	default:
		break;
	}
	return String();
}
void IEventTimer::getJsonCommon(JsonObject& doc)
{
	doc["prev_time"] = String(_prev_time);
	doc["delay"] = String(_delay);
	doc["is_active"] = String(_is_active);
	doc["addr"] = String(_addr);
}

Strip* IEventTimer::obj = nullptr;

IEventTimer::IEventTimer(const uint _delay, bool _is_active, unsigned long _prev_time, uint16_t addr ) : _delay(_delay), _is_active(_is_active), _prev_time(_prev_time), _addr(addr)
{
}

IEventTimer::IEventTimer() : _delay(100), _is_active(true), _prev_time(0)
{
}

IDataHolderArr::IDataHolderArr(byte len)
{
	this->len = len;
	arr = new IDataHolder * [len];
	count++;
}

IDataHolderArr::IDataHolderArr(const IDataHolderArr& obj)
{
	len = obj.len;
	arr = obj.arr;
	count++;
}

IDataHolderArr::~IDataHolderArr()
{
	count--;
	if (count == 0)
	{
		if (arr) {
			for (int i = 0; i < len; i++)
				delete arr[i];
			delete[] arr;
			arr = nullptr;
		}
		len = 0;
	}
}

unsigned int IDataHolderArr::count = 0;

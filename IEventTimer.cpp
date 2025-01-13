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

unsigned long IDataHolder::calcTime()
{
	unsigned long now_millis_time = millis();
	if (_repInfo)
	{
		byte now_weekday = StartTimeInfo::start_weekday + (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) / 8640UL;
		unsigned long now_daytime = (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) % 8640UL;
		LOG_USB_TIMER("calcTime with rep 1: now_weekday = %d now_daytime = %d\n", now_weekday, now_daytime);
		for (byte i = now_weekday; i < 7; i++)
		{
			if ((_repInfo >> i) & 0b1)
			{
				if (_timer_time_raw < now_daytime)
					continue;
				LOG_USB_TIMER("calcTime with rep 1: i = %d\n", i);
				return now_millis_time + (8640UL * (i - now_weekday) + _timer_time_raw - now_daytime) * 1000UL + 1;
			}
		}
		for (byte i = 0; i < now_weekday; i++)
		{
			if ((_repInfo >> i) & 0b1)
			{
				if (_timer_time_raw < now_daytime)
					continue;
				LOG_USB_TIMER("calcTime with rep 2: i = %d\n", i);
				return now_millis_time + (8640UL * (7 - now_weekday + i) + _timer_time_raw - now_daytime) * 1000UL + 1;

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
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	if (obj_data_count >= OBJ_DATA_CAP)
	{
		LOG_USB_TIMER("max ref\n");
		return 0;
	}
	uint16_t addr = 0;
	EEPROM.get(obj_data_count * 2 + 1, addr); // first byte for counter
	addr &= 0x0FFF; //first 4 bits for obj type id
	//addr += OBJ_DATA_CAP * 2 + 1; // array of uint16 refs to data plus counter
	LOG_USB_TIMER("reserv on %d\n", addr);
	if (addr >= OBJ_DATA_MAX_ADDR - 6 - uniquePartSize)
	{
		LOG_USB_TIMER("max data\n");
		return 0;
	}
	EEPROM.put(obj_data_count * 2 + 1, addr | 0x2000); // uppdate id; 0x200 - obj id for timers
	obj_data_count++;
	EEPROM.write(0, obj_data_count | EEPROM.read(0) & 0b1000'0000);
	LOG_USB_TIMER("next addr on %d\n", addr + 6 + uniquePartSize);
	EEPROM.put(obj_data_count * 2 + 1, addr + 6 + uniquePartSize);
	EEPROM.commit();
	return addr;
}

void IDataHolder::saveCommon(uint16_t &paddr, IEventTimer::TimerIDEnum id)
{
	EEPROM.write(paddr, id);
	paddr++;
	EEPROM.write(paddr, _repInfo);
	paddr++;
	EEPROM.put(paddr, _timer_time_raw);
	paddr += 4;
}

void IDataHolder::loadCommon(uint16_t& paddr)
{
	_repInfo = EEPROM.read(paddr);
	paddr++;
	EEPROM.get(paddr, _timer_time_raw);
	paddr += 4;
}

void IDataHolder::getJsonCommon(JsonObject& doc)
{
	doc["repInfo"] = String(_repInfo);
	doc["timer_time_raw"] = String(_timer_time_raw);
}


bool IEventTimer::tick(unsigned long cur_time)
{
	if (_prev_time > cur_time) return false;
	if (cur_time - _prev_time > _delay)
	{
		_prev_time = cur_time;
		return action();
	}
	return false;
}
Strip* IEventTimer::obj = nullptr;

IEventTimer::IEventTimer(const uint _delay, bool _is_active, unsigned long _prev_time, uint16_t addr ) : _delay(_delay), _is_active(_is_active), _prev_time(_prev_time), _addr(addr)
{
}

IEventTimer::IEventTimer() : _delay(100), _is_active(true), _prev_time(0)
{
}

void IDataHolderArr::free()
{
	if (arr) {
		for (int i = 0; i < len; i++)
			delete arr[i];
		delete[] arr;
		arr = nullptr;
	}
	len = 0;
}

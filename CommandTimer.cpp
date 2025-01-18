#include "CommandTimer.h"

CommandTimer::CommandTimer(unsigned long from_time, bool is_active, uint delay, String command, bool once, uint16_t addr) : _command(command), _once(once), IEventTimer(delay, is_active, from_time, addr)
{
}

bool CommandTimer::action()
{
	LOG_USB_TIMER("CommandTimer::action\n");
	obj->parse(const_cast<char*>(_command.c_str()));
	if (_once) return true;
	return false;
}

void CommandTimer::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["once"] = String(_once);
	doc["command"] = _command;

}

byte CommandTimer::getId()
{
	return IEventTimer::TimerIDEnum::CommandTimer;
}

IEventTimer* CommandTimerDataHolder::create()
{
	return new CommandTimer(calcTime(), _is_active, _delay, _command, _once, _addr);
}

void CommandTimerDataHolder::save()
{
	uint16_t addr = reservAddr(_command.length()+1 + sizeof(_once) + sizeof(_delay));
	if (!addr) return;
	_addr = addr;
	saveCommon(addr, IEventTimer::TimerIDEnum::CommandTimer);
	EEPROM.put(addr, _once);
	addr += sizeof(_once);
	EEPROM.put(addr, _delay);
	addr += sizeof(_delay);
	const char* command = _command.c_str();
	for (int i = 0 ;; i++)
	{
		EEPROM.write(addr++, command[i]);
		if (command[i] == '\0') break;
	}
	EEPROM.commit();
}

void CommandTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id

	loadCommon(addr);
	EEPROM.get(addr, _once);
	addr += sizeof(_once);
	EEPROM.get(addr, _delay);
	addr += sizeof(_delay);

    char ch;
    _command.clear();
    while (true)
    {
        ch = EEPROM.read(addr++);
        if (ch == '\0') break;
        _command += ch;
    }
}

void CommandTimerDataHolder::getJson(JsonObject& doc)
{
	getJsonCommon(doc);
	doc["once"] = String(_once);
	doc["delay"] = String(_delay);
	doc["command"] = _command;
}

byte CommandTimerDataHolder::getId()
{
	return IEventTimer::TimerIDEnum::CommandTimer;
}

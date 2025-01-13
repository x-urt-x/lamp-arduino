#include "CommandTimer.h"

CommandTimer::CommandTimer(unsigned long from_time, uint delay, String command, bool once, uint16_t addr) : _command(command), _once(once), IEventTimer(delay, true, from_time, addr)
{
}

bool CommandTimer::action()
{
	LOG_USB_TIMER("CommandTimer::action\n");
	obj->parse(const_cast<char*>(_command.c_str()));
	if (_once) return true;
	return false;
}

IEventTimer* CommandTimerDataHolder::create()
{
	return new CommandTimer(calcTime(), _delay, _command, _once, _addr);
}

void CommandTimerDataHolder::save()
{
	_addr = reservAddr(_command.length()+1 + sizeof(_once) + sizeof(_delay));

	saveCommon(_addr, IEventTimer::TimerIDEnum::CommandTimer);
	EEPROM.put(_addr, _once);
	_addr += sizeof(_once);
	EEPROM.put(_addr, _delay);
	_addr += sizeof(_delay);
	EEPROM.put(_addr, _command.c_str());
	EEPROM.commit();
}

void CommandTimerDataHolder::load(uint16_t addr)
{
	_addr = addr;
	addr++; //skip id

	loadCommon(addr);
	EEPROM.get(_addr, _once);
	_addr += sizeof(_once);
	EEPROM.get(_addr, _delay);
	_addr += sizeof(_delay);

    char ch;
    _command.clear();
    while (true)
    {
        ch = EEPROM.read(addr++);
        if (ch == '\0') break;
        _command += ch;
    }
}

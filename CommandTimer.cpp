#include "CommandTimer.h"

CommandTimer::CommandTimer(unsigned long from_time, uint delay, String command, bool once) : _command(command), _once(once), IEventTimer(delay, true, from_time)
{
}

bool CommandTimer::action()
{
	LOG_USB_TIMER("CommandTimer::action\n");
	obj->parse(const_cast<char*>(_command.c_str()));
	if (_once) return true;
	return false;
}

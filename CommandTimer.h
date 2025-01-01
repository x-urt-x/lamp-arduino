#ifndef COMMANDTIMER_H
#define COMMANDTIMER_H

#include "IEventTimer.h"
class CommandTimer : public IEventTimer
{
public:
	CommandTimer(unsigned long from_time, uint delay, String command, bool once = true);
protected:
	bool action() override;
private:
	String _command;
	bool _once;
};
#endif
#ifndef COMMANDTIMER_H
#define COMMANDTIMER_H

#include "IEventTimer.h"
class CommandTimer : public IEventTimer
{
public:
	CommandTimer(unsigned long from_time, uint delay, String command, bool once = true, uint16_t addr = 0);
protected:
	bool action() override;
private:
	String _command;
	bool _once;
};

struct CommandTimerDataHolder : public IDataHolder
{
	virtual IEventTimer* create() override;
	virtual void save() override;
	virtual void load(uint16_t addr) override;

	String _command;
	bool _once;
	uint _delay;
};
#endif
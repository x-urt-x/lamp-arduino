#ifndef BREVENTTIMER_H
#define BREVENTTIMER_H

#include "IEventTimer.h"
#include "log.h"
class BrEventTimer : public IEventTimer
{
public:
	BrEventTimer(unsigned long from_time, unsigned long to_time, int to_br, uint delay);
protected:
	virtual bool action() override;
private:
	int _step_counter;
	int _to_br;
};
#endif
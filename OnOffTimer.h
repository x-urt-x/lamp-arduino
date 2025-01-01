#ifndef ONOFFTIMER_H
#define ONOFFTIMER_H

#include "IEventTimer.h"
class OnOffTimer : public IEventTimer
{
public:
	OnOffTimer(bool* target, bool to_set, unsigned long time);
	bool tick(unsigned long cur_time) override;
protected:
	virtual bool action() override;
private:
	bool _to_set;
	bool* _target;
};
#endif
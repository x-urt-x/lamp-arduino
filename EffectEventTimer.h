#ifndef EFFECTEVENTTIMER_H
#define EFFECTEVENTTIMER_H

#include "IEventTimer.h"
#include "Strip_class.h"
class EffectEventTimer : public IEventTimer
{
public:
	bool tick(unsigned long cur_time) override;
protected:
	virtual bool action() override;

	// Унаследовано через IEventTimer
	virtual void getJson(JsonObject& doc) override;
	virtual byte getId() override;
};
#endif
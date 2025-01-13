#ifndef ONOFFTIMER_H
#define ONOFFTIMER_H

#include "IEventTimer.h"
class OnOffTimer : public IEventTimer
{
public:
	OnOffTimer(unsigned long time, bool* target, bool to_set, uint16_t addr = 0);
	bool tick(unsigned long cur_time) override;
	virtual void getJson(JsonObject& doc) override;
	virtual byte getId() override;
protected:
	virtual bool action() override;
private:
	bool _to_set;
	bool* _target;
};

struct OnOffTimerDataHolder : public IDataHolder
{
	virtual IEventTimer* create() override;
	virtual void save() override;
	virtual void load(uint16_t addr) override;
	virtual void getJson(JsonObject& doc) override;
	virtual byte getId() override;

	bool _to_set;
	bool* _target;
};
#endif
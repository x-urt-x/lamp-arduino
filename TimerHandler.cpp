#include "TimerHandler.h"

void TimerHandler::addTimer(IEventTimer* timer)
{
	LOG_USB_TIMER("TimerHandler: start add timer\n");
	if (!IEventTimer::obj)
	{
		LOG_USB_TIMER("nullptr obj\n");
		return;
	}
	if (_timers_count == _timers_cap)
	{
		LOG_USB_TIMER("extend array\n");
		_timers_cap += 2;
		IEventTimer** new_timers = new IEventTimer * [_timers_cap];
		for (int i = 0; i < _timers_count; i++)
		{
			new_timers[i] = timers[i];
		}
		delete[] timers;
		timers = new_timers;
	}
	timers[_timers_count] = timer;
	_timers_count++;
	LOG_USB_TIMER("TimerHandler: end add timer\t count = %d\n", _timers_count);
}

void TimerHandler::deleteTimer(byte num)
{
	LOG_USB_TIMER("TimerHandler: delete timer %d\n", num);
	delete timers[num];
	for (int i = num + 1; i < _timers_count; i++)
	{
		timers[i - 1] = timers[i];
	}
	_timers_count--;
}

void TimerHandler::setState(bool state, int num)
{
	LOG_USB_TIMER("TimerHandler::setState %d %d\n", state, num);
	if (num >= _timers_count) return;
	timers[num]->_is_active = state;
}

bool TimerHandler::getState(int num)
{
	if (num >= _timers_count) return false;
	return timers[num]->_is_active;
}

TimerHandler::TimerHandler()
{
	timers = new IEventTimer * [_timers_cap];
}

void TimerHandler::addTimer(char* input_str)
{
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 'b':
	{
		input_str++;
		unsigned long now = millis();

		uint from_shift = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		uint to_shift = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		uint to_br = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		uint delay = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		LOG_USB_TIMER("from_shift = %d to_shift = %d to_br = %d delay = %d\n", from_shift, to_shift, to_br, delay);
		addTimer(new BrEventTimer(now + from_shift * 1000, now + from_shift * 1000 + to_shift * 1000, to_br, delay));
		break;
	}
	case 's':
	{
		input_str++;
		unsigned long now = millis();

		uint from_shift = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		addTimer(new OnOffTimer(&(timers[0]->_is_active), atoi(input_str), now + from_shift * 1000));
		break;
	}
	case 'c':
	{
		input_str++;
		unsigned long now = millis();

		uint from_shift = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		uint delay = atoi(input_str);
		input_str++;
		while (*input_str != ' ') input_str++;

		bool once = atoi(input_str);
		input_str+=3;

		LOG_USB_TIMER("from_shift = %d delay = %d once = %d comm =%s\n", from_shift, delay, once, input_str);
		addTimer(new CommandTimer(now + from_shift * 1000, delay, input_str, once));
		break;
	}
	default:
		break;
	}
}

void TimerHandler::tickAll()
{
	unsigned long cur_time = millis();
	for (int i = 0; i < _timers_count; i++)
	{
		if (timers[i]->_is_active)
		{
			//LOG_USB_TIMER("tick %d timer\n", i);
			if (timers[i]->tick(cur_time)) deleteTimer(i);
		}
	}
}
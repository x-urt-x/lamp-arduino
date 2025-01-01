#include "BrEventTimer.h"

bool BrEventTimer::action()
{
    LOG_USB_TIMER("BrEventTimer::action\n");
    int br = obj->get_br();
    int to_set = br + (_to_br - br) / _step_counter;
    LOG_USB_TIMER("br = %d\t_to_br = %d\t_step_counter = %d\tbr to set = %d\n", br, _to_br , _step_counter, to_set);
    obj->set_br(to_set);
    _step_counter--;
    if (_step_counter <= 0)
    {
        LOG_USB_TIMER("end\n");
        return true;
    }
    return false;
}

BrEventTimer::BrEventTimer(unsigned long from_time, unsigned long to_time, int to_br, uint delay) : _to_br(to_br), IEventTimer(delay, true, from_time)
{
    _step_counter = float(to_time - from_time) / delay + 1;
    LOG_USB_TIMER("create BrEventTimer: now = %d from_time = %d to_time = %d to_br = %d delay = %d _step_counter = %d\n", millis(), from_time, to_time, to_br, delay, _step_counter);
}

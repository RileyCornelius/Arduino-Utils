#pragma once

#include <Arduino.h>

class Timer
{
protected:
    uint32_t prevTrigger;
    uint32_t period;

public:
    /**
     * @param period Set the period in miliseconds
     */
    Timer(uint32_t period)
    {
        reset();
        setPeriod(period);
    }

    uint32_t getTime() { return millis(); }
    uint32_t getPeriod() { return period; }
    uint32_t getElapsed() { return getTime() - prevTrigger; }
    uint32_t getRemaining() { return period - getElapsed(); }
    void setPeriod(uint32_t period) { period = period; }
    void reset() { prevTrigger = getTime(); }
    bool ready()
    {
        bool isReady = (getElapsed() >= period);
        if (isReady)
        {
            reset();
        }
        return isReady;
    }

    operator bool() { return ready(); }
};

/*------------------------------------------------------------------------------
 * TIMER MACROS
 *----------------------------------------------------------------------------*/

#ifndef EVERY_N_MILLIS

// EVERY_N_MILLIS(1000)
// {
// do something every 1000 miliseconds
// }
#define EVERY_N_MILLIS(N) EVERY_N_MILLIS_I(CONCAT(__timer, __COUNTER__), N)
#define EVERY_N_MILLIS_I(NAME, N) \
    static Timer NAME = Timer(N); \
    if (NAME)

// Join two symbols together
#define CONCAT(x, y) CONCAT_HELPER(x, y)
#define CONCAT_HELPER(x, y) x##y

#endif
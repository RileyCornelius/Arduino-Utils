#pragma once

#include <Arduino.h>

/*------------------------------------------------------------------------------
 * ABSTRACT TIMERS
 *----------------------------------------------------------------------------*/

class AbstractTimer
{
protected:
    uint32_t _prevTrigger;
    uint32_t _period;

public:
    AbstractTimer()
    {
        reset();
    }

    AbstractTimer(uint32_t period)
    {
        reset();
        setPeriod(period);
    }

    virtual uint32_t getTime() = 0; // pure virtual
    uint32_t getPeriod() { return _period; }
    uint32_t getElapsed() { return getTime() - _prevTrigger; }
    uint32_t getRemaining() { return _period - getElapsed(); }
    void setPeriod(uint32_t period) { _period = period; }
    void reset() { _prevTrigger = getTime(); }
    virtual bool ready()
    {
        bool isReady = (getElapsed() >= _period);
        if (isReady)
        {
            reset();
        }
        return isReady;
    }

    operator bool() { return ready(); }
};

class AbstractCallbackTimer : public AbstractTimer
{
    typedef void (*func_t)(void);

private:
    func_t _pFuncCallback;
    void callFunction() { _pFuncCallback(); }

public:
    AbstractCallbackTimer(uint32_t period, func_t funcCallback)
    {
        reset();
        setPeriod(period);
        setCallback(funcCallback);
    }

    void setCallback(func_t funcCallback) { _pFuncCallback = funcCallback; }
    bool ready() override
    {
        bool isReady = (getElapsed() >= _period);
        if (isReady)
        {
            reset();
            callFunction();
        }
        return isReady;
    }
};

/*------------------------------------------------------------------------------
 * TIMER IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/

class TimerMicros : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return micros(); };
};

class TimerMillis : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis(); };
};

class TimerSeconds : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 1000; };
};

class TimerMinutes : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 60000; };
};

class TimerHours : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 3600000; };
};

/*------------------------------------------------------------------------------
 * TIMER CALLBACK IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/

class TimerCallbackMicros : public AbstractCallbackTimer
{
    using AbstractCallbackTimer::AbstractCallbackTimer; // inherit constructors
public:
    uint32_t getTime() override { return micros(); };
};

class TimerCallbackMillis : public AbstractCallbackTimer
{
    using AbstractCallbackTimer::AbstractCallbackTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis(); };
};

class TimerCallbackSeconds : public AbstractCallbackTimer
{
    using AbstractCallbackTimer::AbstractCallbackTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 1000; };
};

class TimerCallbackMinutes : public AbstractCallbackTimer
{
    using AbstractCallbackTimer::AbstractCallbackTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 60000; };
};

class TimerCallbackHours : public AbstractCallbackTimer
{
    using AbstractCallbackTimer::AbstractCallbackTimer; // inherit constructors
public:
    uint32_t getTime() override { return millis() / 3600000; };
};

/*------------------------------------------------------------------------------
 * NON ARDUINO TIMER IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/

#ifdef __HAL_UNLOCK // only available in STM32 HAL framework
class TimerTick : public AbstractTimer
{
    using AbstractTimer::AbstractTimer; // inherit constructors
public:
    uint32_t getTime() override { return HAL_GetTick(); }; // override time getter
};
#endif

/*------------------------------------------------------------------------------
 * TIMER MACROS
 *----------------------------------------------------------------------------*/

#ifndef EVERY_N_MILLIS
// EVERY_N_MILLIS(1000)
// {
// do something every 1000
// }
#define EVERY_N_MILLIS(N) EVERY_N_MILLIS_I(CONCAT(__timer, __COUNTER__), N)
#define EVERY_N_MILLIS_I(NAME, N)             \
    static TimerMillis NAME = TimerMillis(N); \
    if (NAME)

// FUNC_EVERY_N_MILLIS(1000, function);
// call function every 1000
#define EVERY_N_MILLIS_FUNC(N, FUNC) EVERY_N_MILLIS_FUNC_I(CONCAT(__func_timer, __COUNTER__), N, FUNC)
#define EVERY_N_MILLIS_FUNC_I(NAME, N, FUNC)                        \
    static TimerCallbackMillis NAME = TimerCallbackMillis(N, FUNC); \
    NAME.ready();

// EVERY_N_MICROS(1000)
// {
// do something every 1000
// }
#define EVERY_N_MICROS(N) EVERY_N_MICROS_I(CONCAT(__timer, __COUNTER__), N)
#define EVERY_N_MICROS_I(NAME, N)             \
    static TimerMicros NAME = TimerMicros(N); \
    if (NAME)

// FUNC_EVERY_N_MICROS(1000, function);
// call function every 1000
#define EVERY_N_MICROS_FUNC(N, FUNC) EVERY_N_MICROS_FUNC_I(CONCAT(__func_timer, __COUNTER__), N, FUNC)
#define EVERY_N_MICROS_FUNC_I(NAME, N, FUNC)                        \
    static TimerCallbackMicros NAME = TimerCallbackMicros(N, FUNC); \
    NAME.ready();

// Join two symbols together
#define CONCAT(x, y) CONCAT_HELPER(x, y)
#define CONCAT_HELPER(x, y) x##y

#endif
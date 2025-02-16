#pragma once

#include <Arduino.h>

/**-----------------------------------------------------------------------------
 * ABSTRACT TIMERS
 *----------------------------------------------------------------------------*/

class AbstractTimer
{
protected:
    uint32_t lastTrigger;
    uint32_t period;

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
    uint32_t getPeriod() { return period; }
    uint32_t getElapsed() { return getTime() - lastTrigger; }
    uint32_t getRemaining() { return period - getElapsed(); }
    void setPeriod(uint32_t period) { period = period; }
    void reset() { lastTrigger = getTime(); }
    virtual bool isReady()
    {
        bool isReady = (getElapsed() >= period);
        if (isReady)
        {
            reset();
        }
        return isReady;
    }

    operator bool() { return isReady(); }
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
    bool isReady() override
    {
        bool isReady = (getElapsed() >= period);
        if (isReady)
        {
            reset();
            callFunction();
        }
        return isReady;
    }
};

/**-----------------------------------------------------------------------------
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

/**-----------------------------------------------------------------------------
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
#define EVERY_N_MILLIS(N) I_EVERY_N_MILLIS(CONCAT(__timer, __COUNTER__), N)
#define I_EVERY_N_MILLIS(NAME, N)             \
    static TimerMillis NAME = TimerMillis(N); \
    if (NAME)

// FUNC_EVERY_N_MILLIS(1000, function);
// call function every 1000
#define EVERY_N_MILLIS_FUNC(N, FUNC) I_EVERY_N_MILLIS_FUNC(CONCAT(__func_timer, __COUNTER__), N, FUNC)
#define I_EVERY_N_MILLIS_FUNC(NAME, N, FUNC)                        \
    static TimerCallbackMillis NAME = TimerCallbackMillis(N, FUNC); \
    NAME.isReady();

// EVERY_N_MICROS(1000)
// {
// do something every 1000
// }
#define EVERY_N_MICROS(N) I_EVERY_N_MICROS(CONCAT(__timer, __COUNTER__), N)
#define I_EVERY_N_MICROS(NAME, N)             \
    static TimerMicros NAME = TimerMicros(N); \
    if (NAME)

// FUNC_EVERY_N_MICROS(1000, function);
// call function every 1000
#define EVERY_N_MICROS_FUNC(N, FUNC) I_EVERY_N_MICROS_FUNC(CONCAT(__func_timer, __COUNTER__), N, FUNC)
#define I_EVERY_N_MICROS_FUNC(NAME, N, FUNC)                        \
    static TimerCallbackMicros NAME = TimerCallbackMicros(N, FUNC); \
    NAME.isReady();

// Join two symbols together
#ifndef CONCAT
#define CONCAT(x, y) I_CONCAT(x, y)
#define I_CONCAT(x, y) x##y
#endif

#endif
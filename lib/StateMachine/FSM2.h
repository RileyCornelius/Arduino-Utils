#pragma once

#include <Arduino.h>
#include <functional>
#include <list>

#define NO_ENTER nullptr
#define NO_HANDLE nullptr
#define NO_EXIT nullptr

// custom logger for recording all transitions
struct Logger
{
    static const char *stateName(const uint8_t stateId)
    {
        switch (stateId)
        {
        case 1:
            return "Idle";
        case 2:
            return "Playing";
        case 3:
            return "Paused";
        default:
            assert(false);
            return "";
        }
    }

    void log(uint8_t origin, uint8_t target)
    {
        Serial.println(String(stateName(origin)) + " -> " + String(stateName(target)));
    }
};

// class EventControl
// {
// public:
//     std::string context;

//     template <typename State>
//     void changeTo()
//     {
//     }
// };

// // state helper for error reporting
// struct Base
// {
//     template <typename Event>
//     void react(const Event &, EventControl &)
//     {
//         Serial.println("[unsupported transition]");
//     }
// };

// // states
// struct Idle
//     : Base
// {
//     using Base::react;

//     void react(const Play &event, EventControl &control)
//     {
//         control.context = event.title;
//         control.changeTo<Playing>();
//     }
// };

// struct Playing
//     : Base
// {
//     using Base::react;

//     void react(const Pause &, EventControl &control)
//     {
//         control.changeTo<Paused>();
//     }

//     void react(const Stop &, EventControl &control)
//     {
//         control.changeTo<Idle>();
//     }
// };

// struct Paused
//     : Base
// {
//     using Base::react;

//     void react(const Resume &, EventControl &control)
//     {
//         control.changeTo<Playing>();
//     }

//     void react(const Stop &, EventControl &control)
//     {
//         control.changeTo<Idle>();
//     }
// };

// template <typename S>
// class StateMachine
// {
//     S *currentState;

//     StateMachine(S &initialState)
//         : currentState(&initialState) {}

//     template <typename E>
//     void initEvents(E &event)
//     {
//     }
// };

// ----------------------
//          FSM
// ----------------------

class Transition;

class State
{
private:
    std::function<void()> onEnter = nullptr;
    std::function<void()> onExit = nullptr;
    std::function<void()> onHandle = nullptr;

    void trigger(std::function<void()> callback)
    {
        if (callback)
            callback();
    }

public:
    std::list<Transition> transitions;

    State(std::function<void()> onHandle)
        : onHandle(onHandle) {}

    State(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
        : onEnter(onEnter), onHandle(onHandle), onExit(onExit) {}

    bool operator==(const State &other) const
    {
        return this == &other;
    }

    void enter() { trigger(onEnter); }
    void exit() { trigger(onExit); }
    void handle() { trigger(onHandle); }
};

class State2
{
private:
    std::function<void()> onEnter = nullptr;
    std::function<void()> onExit = nullptr;
    std::function<void()> onHandle = nullptr;

    void trigger(std::function<void()> callback)
    {
        if (callback)
            callback();
    }

public:
    std::list<Transition> transitions;

    State2(std::function<void()> onHandle)
        : onHandle(onHandle) {}

    State2(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
        : onEnter(onEnter), onHandle(onHandle), onExit(onExit) {}

    bool operator==(const State2 &other) const
    {
        return this == &other;
    }

    std::string_view name;

    void enter() { trigger(onEnter); }
    void exit() { trigger(onExit); }
    void handle() { trigger(onHandle); }
};

struct Transition
{
    Transition(State *stateTo, std::function<bool()> condition)
        : stateTo(stateTo), condition(condition) {}

    Transition(State *stateTo, uint32_t interval, std::function<bool()> condition)
        : stateTo(stateTo), interval(interval), condition(condition) {}

    Transition(State *stateTo, int16_t event)
        : stateTo(stateTo), event(event) {}

    State *stateTo;
    uint32_t interval = 0;
    int16_t event = -1;
    std::function<bool()> condition = nullptr;
};

class Transition2
{
    Transition2(State *stateTo, std::function<bool()> condition)
        : stateTo(stateTo), condition(condition) {}

    Transition2(State *stateTo, uint32_t interval, std::function<bool()> condition)
        : stateTo(stateTo), interval(interval), condition(condition) {}

    Transition2(State *stateTo, int16_t event)
        : stateTo(stateTo), event(event) {}

    State *stateFrom;
    State *stateTo;
    uint32_t interval = 0;
    int16_t event = -1;
    std::function<bool()> condition = nullptr;
};

class FSM
{
private:
    State *currentState;
    uint32_t lastTransitionTime = 0;
    bool firstRun = false;

public:
    FSM(State &initialState)
        : currentState(&initialState) {}

    void run()
    {
        if (!firstRun)
        {
            currentState->enter();
            lastTransitionTime = millis();
            firstRun = true;
        }

        for (Transition &transition : currentState->transitions)
        {
            if (transition.condition && transition.condition())
            {
                transitionTo(*transition.stateTo);
                break;
            }
            if (transition.interval > 0 && getTimeInState() >= transition.interval)
            {
                transitionTo(*transition.stateTo);
                break;
            }
        }

        currentState->handle();
    }

    void trigger(int16_t event)
    {
        for (Transition &transition : currentState->transitions)
        {
            if (transition.event == event)
            {
                transitionTo(*transition.stateTo);
                break;
            }
        }
    }

    void transitionTo(State &newState)
    {
        currentState->exit();
        currentState = &newState;
        currentState->enter();
        lastTransitionTime = millis();
    }

    void timedTransitionTo(State &newState, uint32_t interval)
    {
        if (getTimeInState() >= interval)
            transitionTo(newState);
    }

    void conditionalTransitionTo(State &newState, std::function<bool()> condition)
    {
        if (condition && condition())
            transitionTo(newState);
    }

    template <typename T>
    State createStateInClass(void (T::*onEnter)(), void (T::*onHandle)(), void (T::*onExit)())
    {
        return State(BIND(onEnter), BIND(onHandle), BIND(onExit));
    }

    State createState(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
    {
        return State(onEnter, onHandle, onExit);
    }

    void addTransition(State &stateFrom, State &stateTo, std::function<bool()> condition)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, condition));
    }

    void addEventTransition(State &stateFrom, State &stateTo, uint8_t event)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, event));
    }

    void addTimedTransition(State &stateFrom, State &stateTo, uint32_t interval, std::function<bool()> condition = nullptr)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, interval, condition));
    }

    State &getState()
    {
        return *currentState;
    }

    bool isState(State &state)
    {
        return (*currentState == state);
    }

    uint32_t getTimeInState()
    {
        return millis() - lastTransitionTime;
    }
};
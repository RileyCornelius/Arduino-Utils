#pragma once

#include <Arduino.h>
#include <functional>
#include <list>

#define NO_ENTER nullptr
#define NO_HANDLE nullptr
#define NO_EXIT nullptr

#define BIND(func) std::bind(func, this)
// #define FUNC2(func) [this]() { func(); }

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

struct Transition
{
    Transition(State *stateTo, std::function<bool()> condition)
        : stateTo(stateTo), condition(condition) {}

    Transition(State *stateTo, uint32_t interval, std::function<bool()> condition = nullptr)
        : stateTo(stateTo), interval(interval), condition(condition) {}

    State *stateTo;
    uint32_t interval = 0;
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
            else if (transition.interval > 0 && getTimeInState() >= transition.interval)
            {
                transitionTo(*transition.stateTo);
                break;
            }
        }

        currentState->handle();
    }

    State createState(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
    {
        return State(onEnter, onHandle, onExit);
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

    void addTransition(State &stateFrom, State &stateTo, std::function<bool()> condition)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, condition));
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

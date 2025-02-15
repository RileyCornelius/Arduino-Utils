#pragma once

#include <Arduino.h>
#include <functional>
#include <list>

#define NO_ENTER nullptr
#define NO_HANDLE nullptr
#define NO_EXIT nullptr

class Transition;

class FsmState
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

    FsmState(std::function<void()> onHandle)
        : onHandle(onHandle) {}

    FsmState(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
        : onEnter(onEnter), onHandle(onHandle), onExit(onExit) {}

    bool operator==(const FsmState &other) const
    {
        return this == &other;
    }

    void enter() { trigger(onEnter); }
    void exit() { trigger(onExit); }
    void handle() { trigger(onHandle); }
};

struct Transition
{
    Transition(FsmState *stateTo, std::function<bool()> condition)
        : stateTo(stateTo), condition(condition) {}

    Transition(FsmState *stateTo, uint32_t interval, std::function<bool()> condition = nullptr)
        : stateTo(stateTo), interval(interval), condition(condition) {}

    FsmState *stateTo;
    uint32_t interval = 0;
    std::function<bool()> condition = nullptr;
};

class StateMachine
{
private:
    FsmState *currentState;
    uint32_t lastTransitionTime = 0;
    bool firstRun = false;

public:
    StateMachine(FsmState &initialState)
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

    FsmState createState(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
    {
        return FsmState(onEnter, onHandle, onExit);
    }

    void transitionTo(FsmState &newState)
    {
        currentState->exit();
        currentState = &newState;
        currentState->enter();
        lastTransitionTime = millis();
    }

    void timedTransitionTo(FsmState &newState, uint32_t interval)
    {
        if (getTimeInState() >= interval)
            transitionTo(newState);
    }

    void conditionalTransitionTo(FsmState &newState, std::function<bool()> condition)
    {
        if (condition && condition())
            transitionTo(newState);
    }

    void addTransition(FsmState &stateFrom, FsmState &stateTo, std::function<bool()> condition)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, condition));
    }

    void addTimedTransition(FsmState &stateFrom, FsmState &stateTo, uint32_t interval, std::function<bool()> condition = nullptr)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, interval, condition));
    }

    FsmState &getState()
    {
        return *currentState;
    }

    bool isState(FsmState &state)
    {
        return (*currentState == state);
    }

    uint32_t getTimeInState()
    {
        return millis() - lastTransitionTime;
    }
};

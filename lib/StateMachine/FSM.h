#pragma once

#include <Arduino.h>
#include <functional>
#include <list>

#define NO_ENTER NULL
#define NO_HANDLE NULL
#define NO_EXIT NULL

class Transition; // forward declaration

class State
{
private:
    std::function<void()> onEnter = NULL;
    std::function<void()> onExit = NULL;
    std::function<void()> onHandle = NULL;

public:
    std::list<Transition> transitions;

public:
    State(std::function<void()> onHandle)
        : onHandle(onHandle) {}

    State(std::function<void()> onEnter, std::function<void()> onHandle, std::function<void()> onExit)
        : onEnter(onEnter), onHandle(onHandle), onExit(onExit) {}

    void enter()
    {
        if (onEnter != NULL)
            onEnter();
    }

    void exit()
    {
        if (onExit != NULL)
            onExit();
    }

    void handle()
    {
        if (onHandle != NULL)
            onHandle();
    }
};

struct Transition
{
    Transition(State *stateTo, std::function<bool()> condition)
        : stateTo(stateTo), condition(condition) {}

    Transition(State *stateTo, uint32_t interval)
        : stateTo(stateTo), interval(interval) {}

    State *stateTo;
    uint32_t interval = 0;
    std::function<bool()> condition = NULL;
};

class FSM
{
private:
    State *currentState;
    uint32_t stateChangeTime;
    bool initialized = false;

public:
    FSM(State &initialState)
    {
        currentState = &initialState;
    }

    void run()
    {
        if (!initialized) // first run only
        {
            initialized = true;
            currentState->enter();
            stateChangeTime = millis();
        }

        for (Transition transition : currentState->transitions)
        {
            if (transition.condition != NULL)
            {
                if (transition.condition())
                {
                    transitionTo(*transition.stateTo);
                    break;
                }
            }
            else if (getTimeInState() >= transition.interval)
            {
                transitionTo(*transition.stateTo);
                break;
            }
        }

        currentState->handle();
    }

    void transitionTo(State &newState)
    {
        currentState->exit();
        currentState = &newState;
        currentState->enter();
        stateChangeTime = millis();
    }

    void timedTransitionTo(State &newState, uint32_t interval)
    {
        if (getTimeInState() >= interval)
            transitionTo(newState);
    }

    void addTransition(State &stateFrom, State &stateTo, std::function<bool()> condition)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, condition));
    }

    void addTimedTransition(State &stateFrom, State &stateTo, uint32_t interval)
    {
        stateFrom.transitions.push_back(Transition(&stateTo, interval));
    }

    State &getState()
    {
        return *currentState;
    }

    bool isState(State &state)
    {
        return (currentState == &state);
    }

    uint32_t getTimeInState()
    {
        return millis() - stateChangeTime;
    }
};

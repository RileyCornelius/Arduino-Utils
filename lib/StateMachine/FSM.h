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
    String name;
    std::list<Transition> transitions;

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

class Transition
{
public:
    Transition(State *stateFrom, State *stateTo, std::function<bool()> condition)
        : stateFrom(stateFrom), stateTo(stateTo), condition(condition) {}

    Transition(State *stateFrom, State *stateTo, uint32_t interval)
        : stateFrom(stateFrom), stateTo(stateTo), interval(interval) {}

    State *stateFrom;
    State *stateTo;
    uint32_t interval = 0;
    std::function<bool()> condition = NULL;
};

class FiniteStateMachine
{
private:
    State *currentState;
    uint32_t stateChangeTime;

public:
    FiniteStateMachine(State &initialState)
    {
        currentState = &initialState;
        stateChangeTime = millis();
    }

    void run()
    {
        for (Transition transition : currentState->transitions)
        {
            if (transition.condition != NULL)
            {
                if (transition.condition())
                {
                    transitionTo(transition.stateTo);
                    break;
                }
            }
            else if (getTimeInState() > transition.interval)
            {
                transitionTo(transition.stateTo);
                break;
            }
        }

        currentState->handle();
    }

    void addTransition(State &stateFrom, State &stateTo, std::function<bool()> condition)
    {
        stateFrom.transitions.push_back(Transition(&stateFrom, &stateTo, condition));
    }

    void addTimedTransition(State &stateFrom, State &stateTo, uint32_t interval)
    {
        stateFrom.transitions.push_back(Transition(&stateFrom, &stateTo, interval));
    }

    void transitionTo(State *newState)
    {
        currentState->exit();
        currentState = newState;
        currentState->enter();
        stateChangeTime = millis();
    }

    State *getState()
    {
        return currentState;
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

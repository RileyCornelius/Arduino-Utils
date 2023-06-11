#pragma once

#include <Arduino.h>
#include <functional>

#define NO_ENTER NULL
#define NO_HANDLE NULL
#define NO_EXIT NULL

class Transition; // forward declaration

class State
{
private:
    std::function<void()> onEnter;
    std::function<void()> onExit;
    std::function<void()> onHandle;

public:
    Transition *transition = NULL;

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
        if (currentState->transition != NULL)
        {
            if (currentState->transition->condition != NULL && currentState->transition->condition())
                transitionTo(*currentState->transition->stateTo);
            else if (getTimeInState() > currentState->transition->interval)
                transitionTo(*currentState->transition->stateTo);
        }

        currentState->handle();
    }

    void addTransition(State *stateFrom, State *stateTo, std::function<bool()> condition)
    {
        stateFrom->transition = new Transition(stateFrom, stateTo, condition);
    }

    void addTimedTransition(State *stateFrom, State *stateTo, uint32_t interval)
    {
        stateFrom->transition = new Transition(stateFrom, stateTo, interval);
    }

    void transitionTo(State &newState)
    {
        currentState->exit();
        currentState = &newState;
        currentState->enter();
        stateChangeTime = millis();
    }

    State *getState()
    {
        return currentState;
    }

    bool isState(State &state)
    {
        return (&state == this->currentState);
    }

    uint32_t getTimeInState()
    {
        return millis() - stateChangeTime;
    }
};

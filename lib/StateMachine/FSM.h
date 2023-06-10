#pragma once

#include <Arduino.h>
#include <functional>

#define NO_ENTER NULL
#define NO_HANDLE NULL
#define NO_EXIT NULL

class State
{
private:
    std::function<void()> onEnter;
    std::function<void()> onExit;
    std::function<void()> onHandle;
    // std::function<State *()> transition;

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

class FiniteStateMachine
{
public:
    FiniteStateMachine(State &initialState)
    {
        state = &initialState;
        stateChangeTime = millis();
    }

    void run()
    {
        state->handle();
    }

    void transitionTo(State &newState)
    {
        state->exit();
        state = &newState;
        state->enter();
        stateChangeTime = millis();
    }

    void timedTransitionTo(State &newState, uint32_t interval)
    {
        if (timeInCurrentState() > interval)
            transitionTo(newState);
    }

    State *getCurrentState()
    {
        return state;
    }

    bool isInState(State &state)
    {
        return (&state == this->state);
    }

    uint32_t timeInCurrentState()
    {
        return millis() - stateChangeTime;
    }

private:
    State *state;
    uint32_t stateChangeTime;
};

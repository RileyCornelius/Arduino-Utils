#pragma once

template <typename State>
class SimpleStateMachine
{
private:
    volatile State currentState;
    void (*onTransition)(State from, State to);

public:
    SimpleStateMachine(State initialState, void (*onTransition)(State from, State to) = nullptr)
        : currentState(initialState), onTransition(onTransition)
    {
    }

    bool is(State state)
    {
        return currentState == state;
    }

    State get()
    {
        return currentState;
    }

    void set(State state)
    {
        if (currentState != state)
        {
            if (onTransition)
                onTransition(currentState, state);
            currentState = state;
        }
    }
};

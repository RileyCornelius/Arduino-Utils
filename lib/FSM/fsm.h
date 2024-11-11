#pragma once

#include <functional>

template <typename T>
class Array
{
private:
    T *beginPtr;
    T *endPtr;

public:
    template <size_t N>
    constexpr Array(T (&ptr)[N]) : beginPtr(ptr), endPtr(ptr + N) {}

    T &operator[](size_t index) { return beginPtr[index]; }

    size_t size() { return static_cast<size_t>(endPtr - beginPtr); }

    T *begin() { return beginPtr; }
    T *end() { return endPtr; }

    T &front() { return *beginPtr; }
    T &back() { return *(endPtr - 1); }
};

namespace fsm
{
    struct State
    {
        const char *name = "";
        std::function<void()> onEnter = nullptr;
        std::function<void()> onHandle = nullptr;
        std::function<void()> onExit = nullptr;

        void _exec(std::function<void()> callback)
        {
            if (callback)
                callback();
        }

        bool operator!=(const State &other) const { return !(*this == other); }
        bool operator==(const State &other) const { return *this == other; }

        void enter() { _exec(onEnter); }
        void exit() { _exec(onExit); }
        void handle() { _exec(onHandle); }
    };

    template <typename Event>
    struct Transition
    {
        State *stateFrom;
        State *stateTo;
        Event event;
        std::function<void()> action = nullptr;
    };

    template <typename Event>
    class Fsm
    {
    private:
        State *currentState;
        Array<Transition<Event>> transitions;

    public:
        template <size_t N>
        constexpr Fsm(State *initialState, Transition<Event> (&transitionTable)[N])
            : currentState(initialState), transitions(transitionTable) {}

        bool trigger(Event event)
        {
            for (auto &transition : transitions)
            {
                if (transition.stateFrom == currentState && transition.event == event)
                {
                    currentState->exit();
                    currentState = transition.stateTo;
                    currentState->enter();

                    if (transition.action)
                    {
                        transition.action();
                    }
                    return true;
                }
            }
            return false;
        }

        void handle()
        {
            currentState->handle();
        }

        State getState()
        {
            return *currentState;
        }
    };
};

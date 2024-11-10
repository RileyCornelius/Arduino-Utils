#pragma once

#include <functional>

template <typename T>
class Array
{
private:
    T *beginPtr;
    T *endPtr;

public:
    template <size_t size>
    constexpr Array(T (&ptr)[size]) : beginPtr(ptr), endPtr(ptr + size) {}

    T &operator[](size_t index) { return beginPtr[index]; }

    size_t size() { return static_cast<size_t>(endPtr - beginPtr); }

    T *begin() { return beginPtr; }
    T *end() { return endPtr; }

    T &front() { return *beginPtr; }
    T &back() { return *(endPtr - 1); }
};

namespace fsm3
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

    template <typename E>
    struct Transition
    {
        State *stateFrom;
        State *stateTo;
        E event;
        std::function<void()> action = nullptr;
        std::function<bool()> guard = nullptr;

        bool canTransition(State *currentState, E &evt)
        {
            return stateFrom == currentState && event == evt && (!guard || !guard());
        }
    };

    template <typename E>
    class Fsm
    {
    private:
        State *currentState;
        Array<Transition<E>> transitions;

    public:
        template <size_t N>
        constexpr Fsm(State *initialState, Transition<E> (&transitionTable)[N])
            : currentState(initialState), transitions(transitionTable) {}

        bool trigger(E event)
        {
            for (auto &transition : transitions)
            {
                if (transition.canTransition(currentState, event))
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

        State &getState()
        {
            return *currentState;
        }
    };
};

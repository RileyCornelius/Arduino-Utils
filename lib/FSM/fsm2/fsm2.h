#pragma once

// #include <array>
// #include <vector>
// #include <functional>

#include <Arduino.h>

template <typename T>
struct Optional
{
    T value;
    bool hasValue;

    constexpr Optional() : value{}, hasValue(false) {}
    constexpr Optional(T t) : value(t), hasValue(true) {}
    operator bool() const { return hasValue; }
    auto operator==(T t) const -> bool { return hasValue && value == t; }
    auto operator!=(T t) const -> bool { return !hasValue || value != t; }
};

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

namespace fsm2
{
    struct State
    {
        const char *name = "";
        std::function<void()> onEnter = nullptr;
        std::function<void()> onHandle = nullptr;
        std::function<void()> onExit = nullptr;

        void trigger(std::function<void()> callback)
        {
            if (callback)
                callback();
        }

        bool operator==(const State &other) const { return strcmp(this->name, other.name) == 0; }

        void enter() { trigger(onEnter); }
        void exit() { trigger(onExit); }
        void handle() { trigger(onHandle); }
    };

    template <typename E, typename A>
    struct Transition
    {
        State stateFrom;
        State stateTo;
        E event;
        A action;
        std::function<bool()> guard = nullptr;

        bool canTransit(State &state, E &evt)
        {
            return stateFrom == state && (!event || event == evt) && (!guard || guard());
        }
    };

    template <typename E, typename A>
    class Fsm
    {
    private:
        State currentState;
        Array<Transition<E, A>> transitions;

    public:
        template <size_t N>
        constexpr Fsm(State initialState, Transition<E, A> (&transitionTable)[N])
            : currentState(initialState), transitions(transitionTable) {}

        A input(E event)
        {
            for (auto &transition : transitions)
            {
                if (transition.canTransit(currentState, event))
                {
                    currentState.exit();
                    currentState = transition.stateTo;
                    currentState.enter();

                    if (transition.action)
                    {
                        return transition.action;
                    }
                    break;
                }
            }
            return nullptr;
        }

        void trigger(E event)
        {
            for (auto &transition : transitions)
            {
                if (transition.canTransit(currentState, event))
                {
                    currentState.exit();
                    currentState = transition.stateTo;
                    currentState.enter();
                }
            }
        }

        State getState()
        {
            return currentState;
        }
    };

};

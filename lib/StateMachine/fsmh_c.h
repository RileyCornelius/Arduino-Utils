#pragma once

#include <array>
#include <vector>

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
struct Array
{
    const T *beginPtr;
    const T *endPtr;

    template <size_t size>
    constexpr Array(T const (&initPtr)[size])
        : beginPtr(initPtr), endPtr(initPtr + size) {}

    const T *begin() const { return beginPtr; }
    const T *end() const { return endPtr; }
};

namespace fsm
{

    template <typename S, typename E>
    using Guard = std::function<bool(S &, E &)>;

    template <typename S, typename E, typename A>
    struct Transition
    {
        S stateFrom;
        Optional<S> stateTo{};
        Optional<E> event{};
        Optional<A> action{};
        Guard<S, E> guard = nullptr;

        bool canTransit(S &state, E &evt)
        {
            return stateFrom == state && (!event || event == evt) && (!guard || guard(state, evt));
        }
    };

    template <typename S, typename E, typename A>
    struct Fsm
    {
        S currentState;
        Array<Transition<S, E, A>> transitions;

        Optional<A> input(E event)
        {
            for (auto &transition : transitions)
            {
                if (transition.canTransit(currentState, event))
                {
                    if (transition.stateTo.hasValue)
                    {
                        currentState = transition.stateTo.value;
                    }
                    return transition.action;
                }
            }
            return Optional<A>{};
        }
    };

}; // namespace fsm

// ----------------
// Example usage:
// ----------------

namespace elevator
{

    enum State
    {
        Top,
        Bottom,
    };

    enum Event
    {
        ButtonUp,
        ButtonDown,
    };

    enum Action
    {
        MoveUp,
        MoveDown,
    };

    fsm::Transition<State, Event, Action> transitions[] = {
        {.stateFrom = Top, .stateTo = Bottom, .event = ButtonDown, .action = MoveDown},
        {.stateFrom = Bottom, .stateTo = Top, .event = ButtonUp, .action = MoveUp},
    };

    fsm::Fsm<State, Event, Action> fsm = {
        .currentState = Top,
        .transitions = transitions,
    };

}

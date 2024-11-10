#pragma once

// #include <array>
// #include <vector>

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
    constexpr Array(T (&initPtr)[size])
        : beginPtr(initPtr), endPtr(initPtr + size) {}

    T &operator[](size_t index) { return beginPtr[index]; }

    size_t size() { return static_cast<size_t>(endPtr - beginPtr); }
    bool isEmpty() { return size() == 0; }

    T *begin() { return beginPtr; }
    T *end() { return endPtr; }

    T &front() { return *beginPtr; }
    T &back() { return *(endPtr - 1); }
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

};

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

    class ElevatorController
    {
    private:
        fsm::Fsm<State, Event, Action> fsm;

    public:
        ElevatorController(fsm::Fsm<State, Event, Action> &fsm)
            : fsm(fsm) {}

        void trigger(Event event)
        {
            auto action = fsm.input(event);
            if (!action.hasValue)
            {
                Serial.println("Invalid action");
                return;
            }

            switch (action.value)
            {
            case MoveUp:
                moveUp();
                break;
            case MoveDown:
                moveDown();
                break;
            }
        }

        void run()
        {
            switch (fsm.currentState)
            {
            case Top:
                Serial.println("Top");
                break;
            case Bottom:
                Serial.println("Bottom");
                break;
            default:
                Serial.println("Unknown state");
                break;
            }
        }

        void moveUp()
        {
            Serial.println("Moving up");
        }

        void moveDown()
        {
            Serial.println("Moving down");
        }
    };

}

void testt()
{
    static elevator::ElevatorController controller(elevator::fsm);

    if (random(2) == 0)
    {
        elevator::Event event = (random(2) == 0) ? elevator::ButtonUp : elevator::ButtonDown;
        controller.trigger(event);
    }

    controller.run();
    delay(1000);
}

#pragma once

// #include <array>
// #include <vector>
#include <functional>

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

namespace fsm1
{
    template <typename S, typename E, typename A>
    struct Transition
    {
        S stateFrom;
        S stateTo;
        E event;
        Optional<A> action{};
        std::function<bool()> guard = nullptr;

        bool canTransit(S &state, E &evt)
        {
            return stateFrom == state && (!event || event == evt) && (!guard || guard());
        }
    };

    template <typename S, typename E, typename A>
    class Fsm
    {
    private:
        S currentState;
        Array<Transition<S, E, A>> transitions;

    public:
        template <size_t N>
        constexpr Fsm(S initialState, Transition<S, E, A> (&transitionTable)[N])
            : currentState(initialState), transitions(transitionTable) {}

        Optional<A> input(E event)
        {
            for (auto &transition : transitions)
            {
                if (transition.canTransit(currentState, event))
                {
                    currentState = transition.stateTo;
                    return transition.action;
                }
            }
            return Optional<A>{};
        }

        S getState()
        {
            return currentState;
        }
    };

};

// ----------------
// Example usage:
// ----------------

namespace elevator
{
    enum Direction
    {
        None,
        Down,
        Up,
    };

    enum State
    {
        Top,
        MovingUp,
        MovingDown,
        Bottom,
    };

    enum Event
    {
        ButtonUp,
        ButtonDown,
        TopReached,
        BottomReached,
    };

    enum Action
    {
        MoveUp,
        MoveDown,
        StopMoving,
    };

    fsm::Transition<State, Event, Action> transitions[] = {
        {.stateFrom = Top, .stateTo = MovingDown, .event = ButtonDown, .action = MoveDown},
        {.stateFrom = MovingUp, .stateTo = Top, .event = TopReached, .action = StopMoving},
        {.stateFrom = MovingDown, .stateTo = Bottom, .event = BottomReached, .action = StopMoving},
        {.stateFrom = Bottom, .stateTo = MovingUp, .event = ButtonUp, .action = MoveUp},
    };

    auto fsm = fsm::Fsm<State, Event, Action>(Top, transitions);

    class ElevatorController
    {
    private:
        fsm::Fsm<State, Event, Action> fsm;
        Direction direction = None;

        // State methods

        void top() { Serial.println("Top"); }
        void bottom() { Serial.println("Bottom"); }
        void movingUp() { Serial.println("Moving up"); }
        void movingDown() { Serial.println("Moving down"); }

        // Action methods

        void moveUp()
        {
            direction = Up;
            Serial.println("Direction up");
        }

        void moveDown()
        {
            direction = Down;
            Serial.println("Direction down");
        }

        void stopMoving()
        {
            direction = None;
            Serial.println("Stopped");
        }

    public:
        ElevatorController(fsm::Fsm<State, Event, Action> &fsm) : fsm(fsm) {}

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
            case StopMoving:
                stopMoving();
                break;
            }
        }

        void run()
        {
            State state = fsm.getState();
            switch (state)
            {
            case Top:
                top();
                break;
            case Bottom:
                bottom();
                break;
            case MovingUp:
                movingUp();
                break;
            case MovingDown:
                movingDown();
                break;
            default:
                Serial.println("Unknown state");
                break;
            }
        }
    };

    void test()
    {
        static auto controller = ElevatorController(fsm);

        if (random(2) == 0)
        {
            Event event = static_cast<Event>(random(4));
            controller.trigger(event);
        }

        controller.run();
        delay(1000);
    }

}

#pragma once

#include <functional>

namespace fsm
{
    template <typename State = uint8_t, typename Event = uint8_t>
    struct SimpleFsm
    {
        struct Transition
        {
            Event event;
            State stateFrom;
            State stateTo;
            std::function<void()> action = nullptr;
        };

        State currentState;
        std::vector<Transition> transitions;
        std::function<void(const Event &event, const State &from, const State &to)> onTransition = nullptr;

        bool trigger(Event event)
        {
            for (auto &transition : transitions)
            {
                if (transition.stateFrom == currentState && transition.event == event)
                {
                    currentState = transition.stateTo;

                    if (transition.action)
                        transition.action();

                    if (onTransition)
                        onTransition(event, transition.stateFrom, transition.stateTo);

                    return true;
                }
            }
            return false;
        }
    };
};

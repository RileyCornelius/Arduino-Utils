#pragma once

#include <functional>

namespace fsm
{
    template <typename State = uint8_t, typename Event = uint8_t>
    struct Fsm
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
        std::function<void(const Event &, const State &, const State &)> transit = nullptr;

        bool trigger(Event event)
        {
            for (auto &transition : transitions)
            {
                if (transition.stateFrom == currentState && transition.event == event)
                {
                    currentState = transition.stateTo;

                    if (transition.action)
                        transition.action();

                    if (transit)
                        transit(event, transition.stateFrom, transition.stateTo);

                    return true;
                }
            }
            return false;
        }
    };
};

#pragma once

#include <functional>
#include <vector>

namespace fsm
{
    struct State
    {
        const char *name = "";
        std::function<void()> onEnter = nullptr;
        std::function<void()> onHandle = nullptr;
        std::function<void()> onExit = nullptr;

        bool operator!=(const State &other) const { return !(*this == other); }
        bool operator==(const State &other) const { return *this == other; }
    };

    template <typename Event>
    struct Fsm
    {
        struct Transition
        {
            Event event;
            State *stateFrom;
            State *stateTo;
            std::function<void()> action = nullptr;
        };

        State *currentState;
        std::vector<Transition> transitions;
        std::function<void(const Event &event, const State &from, const State &to)> onTransition = nullptr;

        void _call(std::function<void()> callback)
        {
            if (callback)
                callback();
        }

        bool trigger(const Event &event)
        {
            for (auto &transition : transitions)
            {
                if (transition.stateFrom == currentState && transition.event == event)
                {
                    _call(currentState->onExit);
                    currentState = transition.stateTo;
                    _call(currentState->onEnter);
                    _call(transition.action);
                    if (onTransition)
                        onTransition(event, *transition.stateFrom, *transition.stateTo);
                    return true;
                }
            }
            return false;
        }

        void handle()
        {
            _call(currentState->onHandle);
        }
    };
};

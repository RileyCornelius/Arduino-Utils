#pragma once

#include "fsm.h"

namespace player
{
    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }
    void stopped() { log_d("Stopped"); }

    void start() { log_d("Start player"); }
    void stop() { log_d("Stop player"); }
    void pause() { log_d("Pause player"); }

    template <typename S>
    struct StateMap
    {
        const char *name;
        S state;
        std::function<void()> func;
    };

    template <typename A>
    struct ActionMap
    {
        const char *name;
        A action;
        std::function<void()> func;
    };

    template <typename E>
    struct EventMap
    {
        const char *name;
        E event;
    };

    enum State
    {
        Idle,
        Playing,
        Paused,
        Stopped,

        MAX_STATE_SIZE
    };

    enum Event
    {
        PlayPressed,
        PausePressed,
        StoppedPressed,

        MAX_EVENT_SIZE
    };

    enum Action
    {
        Start,
        Stop,
        Pause,

        MAX_ACTION_SIZE
    };

    ActionMap<Action> actions[] = {
        {.name = "Start", .action = Start, .func = start},
        {.name = "Stop", .action = Stop, .func = stop},
        {.name = "Pause", .action = Pause, .func = pause},
    };

    StateMap<State> states[] = {
        {.name = "Idle", .state = Idle, .func = idle},
        {.name = "Playing", .state = Playing, .func = playing},
        {.name = "Paused", .state = Paused, .func = paused},
        {.name = "Stopped", .state = Stopped, .func = stopped},
    };

    EventMap<Event> events[] = {
        {.name = "PlayPressed", .event = PlayPressed},
        {.name = "PausePressed", .event = PausePressed},
        {.name = "StoppedPressed", .event = StoppedPressed},
    };

    fsm::Transition<State, Event, Action> transitions[] = {
        {.stateFrom = Idle, .stateTo = Playing, .event = PlayPressed, .action = Start},
        {.stateFrom = Playing, .stateTo = Paused, .event = PausePressed, .action = Pause},
        {.stateFrom = Playing, .stateTo = Idle, .event = StoppedPressed, .action = Stop},
        {.stateFrom = Paused, .stateTo = Playing, .event = PlayPressed, .action = Start},
        {.stateFrom = Paused, .stateTo = Idle, .event = StoppedPressed, .action = Stop},
    };

    auto fsm = fsm::Fsm<State, Event, Action>(Idle, transitions);

    void trigger(Event event)
    {
        Optional<Action> action = fsm.input(event);
        if (!action.hasValue)
        {
            log_d("No action for event [%s] in state [%s]", events[event].name, states[fsm.getState()].name);
            return;
        }

        auto actionFunc = actions[action.value].func;
        actionFunc();
    }

    void run()
    {
        State state = fsm.getState();
        auto stateFunc = states[state].func;
        stateFunc();
    }

    void test()
    {
        Event event = static_cast<Event>(random(MAX_EVENT_SIZE));
        trigger(event);

        run();
        delay(1000);
    }
};
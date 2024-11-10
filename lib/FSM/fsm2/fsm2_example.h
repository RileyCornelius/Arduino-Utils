#pragma once

#include "fsm2/fsm2.h"

// ----------------
// Example usage:
// ----------------
using namespace fsm2;

namespace player2
{

    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }
    void stopped() { log_d("Stopped"); }

    void loading() { log_d("Loading"); }

    void start() { log_d("Start player"); }
    void stop() { log_d("Stop player"); }
    void pause() { log_d("Pause player"); }

    using Action = std::function<void()>;
    using Event = const char *;

    State Playing = {.name = "Playing", .onHandle = playing};
    State Paused = {.name = "Paused", .onHandle = paused};
    State Idle = {.name = "Idle", .onHandle = idle};
    State Stopped = {.name = "Stopped", .onHandle = stopped};
    State Loading = {.name = "Loading", .onHandle = loading};

    Event PlayPressed = "PlayPressed";
    Event PausePressed = "PausePressed";
    Event StoppedPressed = "StoppedPressed";

    Event events[] = {PlayPressed, PausePressed, StoppedPressed};

    fsm2::Transition<Event, Action> transitions[] = {
        {.stateFrom = Idle, .stateTo = Playing, .event = PlayPressed, .action = start},
        {.stateFrom = Playing, .stateTo = Paused, .event = PausePressed, .action = pause},
        {.stateFrom = Playing, .stateTo = Idle, .event = StoppedPressed, .action = stop},
        {.stateFrom = Paused, .stateTo = Playing, .event = PlayPressed, .action = start},
        {.stateFrom = Paused, .stateTo = Idle, .event = StoppedPressed, .action = stop},
    };

    auto fsm = fsm2::Fsm<Event, Action>(Idle, transitions);

    void trigger(Event event)
    {
        auto action = fsm.input(event);
        if (!action)
        {
            log_w("No action for event [%s] in state [%s]", event, fsm.getState().name);
            return;
        }

        auto actionFunc = action;
        actionFunc();
    }

    void run()
    {
        State state = fsm.getState();
        state.handle();
    }

    void test()
    {
        Event event = (events[random(3)]);
        trigger(event);

        run();
        delay(1000);
    }
};
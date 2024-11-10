#pragma once

#include "fsm3/fsm3.h"

// ----------------
// Example usage:
// ----------------
using namespace fsm3;

namespace player3
{
    // state handlers
    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }

    enum Event
    {
        PlayPressed,
        PausePressed,
        StoppedPressed,

        MAX_EVENT_SIZE
    };

    State Playing = {.onHandle = playing};
    State Paused = {.onHandle = paused};
    State Idle = {.onHandle = idle};

    fsm3::Transition<Event> transitions[] = {
        {.stateFrom = &Idle, .stateTo = &Playing, .event = PlayPressed},
        {.stateFrom = &Playing, .stateTo = &Paused, .event = PausePressed},
        {.stateFrom = &Playing, .stateTo = &Idle, .event = StoppedPressed},
        {.stateFrom = &Paused, .stateTo = &Playing, .event = PlayPressed},
        {.stateFrom = &Paused, .stateTo = &Idle, .event = StoppedPressed},
    };

    auto fsm = fsm3::Fsm<Event>(&Idle, transitions);

    void trigger(Event event)
    {
        fsm.trigger(event);
    }

    void run()
    {
        fsm.handle();
    }

    void test()
    {
        Event event = static_cast<Event>(random(MAX_EVENT_SIZE));
        trigger(event);

        run();
        delay(1000);
    }
};
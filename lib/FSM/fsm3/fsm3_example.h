#pragma once

#include <map>
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

    // actions
    void start() { log_d("Start player"); }
    void stop() { log_d("Stop player"); }
    void pause() { log_d("Pause player"); }

    // guards
    bool randomLag()
    {
        auto num = random(2);
        if (num >= 1)
        {
            log_w("Guarded");
        }
        return num;
    }

    enum Event
    {
        PlayPressed,
        PausePressed,
        StoppedPressed,

        MAX_EVENT_SIZE
    };

    std::map<Event, const char *> eventNames = {
        {PlayPressed, "PlayPressed"},
        {PausePressed, "PausePressed"},
        {StoppedPressed, "StoppedPressed"},
    };

    State Playing = {.name = "Playing", .onHandle = playing};
    State Paused = {.name = "Paused", .onHandle = paused};
    State Idle = {.name = "Idle", .onHandle = idle};

    fsm3::Transition<Event> transitions[] = {
        {.stateFrom = &Idle, .stateTo = &Playing, .event = PlayPressed, .action = start, .guard = randomLag},
        {.stateFrom = &Playing, .stateTo = &Paused, .event = PausePressed, .action = pause},
        {.stateFrom = &Playing, .stateTo = &Idle, .event = StoppedPressed, .action = stop},
        {.stateFrom = &Paused, .stateTo = &Playing, .event = PlayPressed, .action = start},
        {.stateFrom = &Paused, .stateTo = &Idle, .event = StoppedPressed, .action = stop},
    };

    auto fsm = fsm3::Fsm<Event>(&Idle, transitions);

    void trigger(Event event)
    {
        log_i("Triggered event [%s]", eventNames[event]);

        auto stateChanged = fsm.trigger(event);
        if (!stateChanged)
        {
            log_w("Failed to transition from [%s] on event [%s]", fsm.getState().name, eventNames[event]);
        }
    }

    void run()
    {
        fsm.handle();
    }

    void test()
    {
        if (random(2))
        {
            Event event = static_cast<Event>(random(3));
            trigger(event);
        }

        run();
        delay(1000);
    }
};
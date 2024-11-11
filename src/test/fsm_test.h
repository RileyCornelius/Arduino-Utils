#include <Arduino.h>
#include <map>

#include "fsm.h"

// ----------------
// Example Usage:
// ----------------

namespace player
{
    void playing() { log_d("Playing"); }
    void paused() { log_d("Paused"); }
    void idle() { log_d("Idle"); }
    void start() { log_d("Start"); }

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

    fsm::State Playing = {.name = "Playing", .onHandle = playing};
    fsm::State Paused = {.name = "Paused", .onHandle = paused};
    fsm::State Idle = {.name = "Idle", .onHandle = idle};

    fsm::Fsm<Event> fsm = {
        .currentState = &Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = &Idle, .stateTo = &Playing, .action = start},
            {.event = PausePressed, .stateFrom = &Playing, .stateTo = &Paused},
            {.event = PlayPressed, .stateFrom = &Paused, .stateTo = &Playing},
        },
        .onTransition = [](Event &event, fsm::State &from, fsm::State &to)
        {
            log_i("Event: %s - State: %s => %s", eventNames[event], from.name, to.name);
        },
    };

    void trigger(Event event)
    {
        bool stateChanged = fsm.trigger(event);
        if (!stateChanged)
        {
            log_w("Invalid transition from %s with event %s", fsm.currentState->name, eventNames[event]);
        }
    }

    void run()
    {
        fsm.handle();
    }
};

// -----------------------------------
//  TEST CODE
// -----------------------------------

void fsm_setup_test()
{
}

void fsm_loop_test()
{
    using namespace player;

    Event event = static_cast<Event>(random(MAX_EVENT_SIZE));
    trigger(event);

    run();
    delay(1000);
}

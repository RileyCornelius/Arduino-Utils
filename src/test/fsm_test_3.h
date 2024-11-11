#include <Arduino.h>
#include <map>

#include "fsm3/fsm3.h"

// ----------------
// Basic Example Usage:
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

    fsm::State Playing = {.onHandle = playing};
    fsm::State Paused = {.onHandle = paused};
    fsm::State Idle = {.onHandle = idle};

    fsm::Fsm<Event> fsm = {
        .currentState = &Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = &Idle, .stateTo = &Playing, .action = start},
            {.event = PausePressed, .stateFrom = &Playing, .stateTo = &Paused},
            {.event = PlayPressed, .stateFrom = &Paused, .stateTo = &Playing},
        },
    };

    void trigger(Event event)
    {
        fsm.trigger(event);
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
    player::Event event = static_cast<player::Event>(random(player::MAX_EVENT_SIZE));
    player::trigger(event);

    player::run();
    delay(1000);
}

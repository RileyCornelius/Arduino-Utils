#include <Arduino.h>
#include <map>
#include "fsm.h"

// ----------------
// Basic Example Usage:
// ----------------

namespace player
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

    fsm::State Playing = {.onHandle = playing};
    fsm::State Paused = {.onHandle = paused};
    fsm::State Idle = {.onHandle = idle};

    fsm::Transition<Event> transitions[] = {
        {.stateFrom = &Idle, .stateTo = &Playing, .event = PlayPressed},
        {.stateFrom = &Playing, .stateTo = &Paused, .event = PausePressed},
        {.stateFrom = &Playing, .stateTo = &Idle, .event = StoppedPressed},
        {.stateFrom = &Paused, .stateTo = &Playing, .event = PlayPressed},
        {.stateFrom = &Paused, .stateTo = &Idle, .event = StoppedPressed},
    };

    auto fsm = fsm::Fsm<Event>(&Idle, transitions);

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

#include <Arduino.h>
#include <map>
#include "fsm.h"

// ----------------
// Example usage:
// ----------------

namespace player
{
    // state handlers
    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }

    // actions
    void start() { log_d("Start player"); }
    void stop() { log_d("Stop player"); }
    void pause() { log_d("Pause player"); }

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

    fsm::Transition<Event> transitions[] = {
        {.stateFrom = &Idle, .stateTo = &Playing, .event = PlayPressed, .action = start},
        {.stateFrom = &Playing, .stateTo = &Paused, .event = PausePressed, .action = pause},
        {.stateFrom = &Playing, .stateTo = &Idle, .event = StoppedPressed, .action = stop},
        {.stateFrom = &Paused, .stateTo = &Playing, .event = PlayPressed, .action = start},
        {.stateFrom = &Paused, .stateTo = &Idle, .event = StoppedPressed, .action = stop},
    };

    fsm::Fsm<Event> fsm(&Idle, transitions);

    void trigger(Event event)
    {
        fsm::State prevState = fsm.getState();
        bool stateChanged = fsm.trigger(event);
        if (!stateChanged)
        {
            log_w("State [%s] did not change", fsm.getState().name, eventNames[event]);
        }
        log_i("Event: [%s] - State: [%s] => [%s]", eventNames[event], prevState.name, fsm.getState().name);
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
    if (random(2))
    {
        player::Event event = static_cast<player::Event>(random(3));
        player::trigger(event);
    }

    player::run();
    delay(1000);
}

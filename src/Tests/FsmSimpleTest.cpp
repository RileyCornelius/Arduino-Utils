#include <Arduino.h>
#include <map>

#include "FsmSimple.h"

// ----------------
// Basic Example Usage:
// ----------------

namespace player
{
    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }

    enum Event
    {
        PlayPressed,
        PausePressed,
        StoppedPressed,
    };

    enum State
    {
        Playing,
        Paused,
        Idle,
    };

    fsm::FsmSimple<State, Event> fsm = {
        .currentState = Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = Idle, .stateTo = Playing, .action = playing},
            {.event = PausePressed, .stateFrom = Playing, .stateTo = Paused, .action = paused},
            {.event = PlayPressed, .stateFrom = Paused, .stateTo = Playing, .action = playing},
        },
        .onTransition = [](const Event &event, const State &from, const State &to)
        {
            log_i("Event: %d - State: %d => %d", event, from, to);
        },
    };

    void trigger(Event event)
    {
        fsm.trigger(event);
    }
};

namespace player2
{
    void playing() { log_d("Playing"); }
    void idle() { log_d("Idle"); }
    void paused() { log_d("Paused"); }

    using Event = const char *;
    using State = const char *;

    Event PlayPressed = "PlayPressed";
    Event PausePressed = "PausePressed";
    Event StoppedPressed = "StoppedPressed";

    State Playing = "Playing";
    State Paused = "Paused";
    State Idle = "Idle";

    fsm::FsmSimple<State, Event> fsm = {
        .currentState = Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = Idle, .stateTo = Playing, .action = playing},
            {.event = PausePressed, .stateFrom = Playing, .stateTo = Paused, .action = paused},
            {.event = PlayPressed, .stateFrom = Paused, .stateTo = Playing, .action = playing},
        },
        .onTransition = [](const Event &event, const State &from, const State &to)
        {
            log_i("2Event: %s - State: %s => %s", event, from, to);
        },
    };

    void trigger(Event event)
    {
        fsm.trigger(event);
    }
};

// -----------------------------------
//  TEST CODE
// -----------------------------------

void fsmSimpleSetup()
{
}

void fsmSimpleLoop()
{
    player::Event event = static_cast<player::Event>(random(3));
    player::trigger(event);

    player2::Event events2[] = {player2::PlayPressed, player2::PausePressed, player2::StoppedPressed};
    player2::Event event2 = events2[random(3)];
    player2::trigger(event2);

    delay(1000);
}

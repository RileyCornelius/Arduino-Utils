#include <Arduino.h>
#include <map>

#include "SimpleFsm.h"

// ----------------
// Basic Example Usage:
// ----------------

namespace player
{
    void playing() { Serial.println("Playing"); }
    void idle() { Serial.println("Idle"); }
    void paused() { Serial.println("Paused"); }

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

    fsm::SimpleFsm<State, Event> fsm = {
        .currentState = Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = Idle, .stateTo = Playing, .action = playing},
            {.event = PausePressed, .stateFrom = Playing, .stateTo = Paused, .action = paused},
            {.event = PlayPressed, .stateFrom = Paused, .stateTo = Playing, .action = playing},
        },
        .onTransition = [](const Event &event, const State &from, const State &to)
        {
            Serial.printf("Event: %d - State: %d => %d\n", event, from, to);
        },
    };

    void trigger(Event event)
    {
        fsm.trigger(event);
    }
};

namespace player2
{
    void playing() { Serial.println("Playing"); }
    void idle() { Serial.println("Idle"); }
    void paused() { Serial.println("Paused"); }

    using Event = const char *;
    using State = const char *;

    Event PlayPressed = "PlayPressed";
    Event PausePressed = "PausePressed";
    Event StoppedPressed = "StoppedPressed";

    State Playing = "Playing";
    State Paused = "Paused";
    State Idle = "Idle";

    fsm::SimpleFsm<State, Event> fsm = {
        .currentState = Idle,
        .transitions = {
            {.event = PlayPressed, .stateFrom = Idle, .stateTo = Playing, .action = playing},
            {.event = PausePressed, .stateFrom = Playing, .stateTo = Paused, .action = paused},
            {.event = PlayPressed, .stateFrom = Paused, .stateTo = Playing, .action = playing},
        },
        .onTransition = [](const Event &event, const State &from, const State &to)
        {
            Serial.printf("2Event: %s - State: %s => %s\n", event, from, to);
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

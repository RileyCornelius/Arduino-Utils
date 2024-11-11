
#include <Arduino.h>

#include "iot_fsm/iot_fsm.hpp"

enum PlayerEvent : uint8_t
{
    PLAY,
    PAUSE,
    RESUME,
    STOP
};

enum PlayerState : uint8_t
{
    IDLE,
    PLAYING,
    PAUSED
};

typedef uint8_t event_t;
typedef uint8_t state_t;
typedef fsm::statemachine<event_t, state_t> myfsm_t;

myfsm_t myFsm{
    .state = IDLE,
    .transitions = {
        {.filter = myfsm_t::match({PLAY}), .source = IDLE, .target = PLAYING, .transit = nullptr},
        {.filter = myfsm_t::match({PAUSE}), .source = PLAYING, .target = PAUSED, .transit = nullptr},
        {.filter = myfsm_t::match({PAUSE}), .source = PLAYING, .target = PAUSED, .transit = nullptr},
        {.filter = myfsm_t::match({RESUME}), .source = PAUSED, .target = PLAYING, .transit = nullptr},
        {.filter = myfsm_t::match({STOP}), .source = PLAYING, .target = IDLE, .transit = nullptr},
        {.filter = myfsm_t::match({STOP}), .source = PAUSED, .target = IDLE, .transit = nullptr},
    },
    .transit = [](const event_t &evt, const state_t &src, const state_t &dst)
    {
        Serial.println("Event: " + String(evt) + " - State: " + String(src) + " => " + String(dst));
    },
};

void iot_fsm_loop_test()
{
    event_t event = static_cast<event_t>(random(4));
    myFsm(event);

    delay(1000);
}

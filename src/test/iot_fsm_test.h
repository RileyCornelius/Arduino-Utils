
#include <Arduino.h>

#include "iot_fsm/iot_fsm.hpp"

typedef uint8_t event_t;
typedef uint8_t state_t;
typedef fsm::statemachine<event_t, state_t> myfsm_t;

myfsm_t myFsm{
    .state = 0, // starting in this state
    .transitions = {

        // transition 1
        {
            .filter = myfsm_t::match({1, 2, 3}), // matching events
            .source = 0,                         // source state
            .target = 1,                         // destination state
            .transit = [](const event_t &evt, const state_t &src, const state_t &dst)
            {
                // do stuff during transition...
            }},

        // transition 2
        {
            .filter = myfsm_t::match({2, 3, 4}),
            .source = 1,
            .target = 0,
            // multiple callbacks
            .transit = myfsm_t::series({[](const event_t &evt, const state_t &src, const state_t &dst)
                                        {
                                            // do stuff during transition...
                                        },
                                        [](const event_t &evt, const state_t &src, const state_t &dst)
                                        {
                                            // do stuff during transition...
                                        }}),
        },
    },
    // for every transition
    .transit = [](const event_t &evt, const state_t &src, const state_t &dst)
    {
        // do stuff during any transition...
    },
};

void iot_fsm_loop_test()
{
    state_t prevState = myFsm.state;
    uint8_t event = random(5);
    myFsm(event);

    Serial.println("Event: " + String(event) + " - State: " + String(prevState) + " => " + String(myFsm.state));
    delay(1000);
}

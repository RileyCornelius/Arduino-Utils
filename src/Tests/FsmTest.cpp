// #include <Arduino.h>
// #include <map>

// #include "Fsm.h"

// // ----------------
// // Example Usage:
// // ----------------

// namespace player3
// {
//     void playing() { Serial.println("Playing"); }
//     void paused() { Serial.println("Paused"); }
//     void idle() { Serial.println("Idle"); }
//     void start() { Serial.println("Start"); }

//     enum Event
//     {
//         PlayPressed,
//         PausePressed,
//         StoppedPressed,

//         MAX_EVENT_SIZE
//     };

//     std::map<Event, const char *> eventNames = {
//         {PlayPressed, "PlayPressed"},
//         {PausePressed, "PausePressed"},
//         {StoppedPressed, "StoppedPressed"},
//     };

//     fsm::State Playing = fsm::State::create("Playing", playing);
//     fsm::State Paused = fsm::State::create("Paused", paused);
//     fsm::State Idle = fsm::State::create("Idle", idle);

//     fsm::Fsm<Event> fsm = {
//         .currentState = &Idle,
//         .transitions = {
//             {.event = PlayPressed, .stateFrom = &Idle, .stateTo = &Playing, .action = start},
//             {.event = PausePressed, .stateFrom = &Playing, .stateTo = &Paused},
//             {.event = PlayPressed, .stateFrom = &Paused, .stateTo = &Playing},
//         },
//         .onTransition = [](const Event &event, const fsm::State &from, const fsm::State &to)
//         {
//             Serial.printf("Event: %s - State: %s => %s\n", eventNames[event], from.name, to.name);
//         },
//     };

//     void trigger(Event event)
//     {
//         bool stateChanged = fsm.trigger(event);
//         if (!stateChanged)
//         {
//             Serial.printf("Invalid transition from %s with event %s\n", fsm.currentState->name, eventNames[event]);
//         }
//     }

//     void run()
//     {
//         fsm.handle();
//     }
// };

// // -----------------------------------
// //  TEST CODE
// // -----------------------------------

// void fsmSetup()
// {
// }

// void fsmLoop()
// {
//     using namespace player3;

//     Event event = static_cast<Event>(random(MAX_EVENT_SIZE));
//     trigger(event);

//     run();
//     delay(1000);
// }

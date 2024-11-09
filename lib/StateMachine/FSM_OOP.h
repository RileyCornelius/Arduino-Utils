#pragma once

#include <Arduino.h>
#include <functional>
#include <list>

#define NO_ENTER NULL
#define NO_HANDLE NULL
#define NO_EXIT NULL

// class Transition; // forward declaration

// class Transition
// {
// public:
//     Transition(State *stateFrom, State *stateTo, std::function<bool()> condition)
//         : stateFrom(stateFrom), stateTo(stateTo), condition(condition) {}

//     Transition(State *stateFrom, State *stateTo, uint32_t interval)
//         : stateFrom(stateFrom), stateTo(stateTo), interval(interval) {}

//     State *stateFrom;
//     State *stateTo;
//     uint32_t interval = 0;
//     std::function<bool()> condition = NULL;
// };

// class FSM;

// class BaseState
// {
// protected:
// public:
//     // FSM *fsm;
//     // std::list<Transition> transitions;

// public:
//     // BaseState(FSM *fsm) : fsm(fsm) {}

//     virtual void enter() {}

//     virtual void exit() {}

//     virtual void handle(FSM *fsm) = 0;
// };

// class FSM
// {
// private:
//     BaseState *currentState;
//     uint32_t stateChangeTime;

// public:
//     FSM()
//     {
//     }

//     void run()
//     {
//         currentState->handle(this);
//     }

//     void transitionTo(BaseState &newState)
//     {
//         currentState->exit();
//         currentState = &newState;
//         currentState->enter();
//         stateChangeTime = millis();
//     }

//     void timedTransitionTo(BaseState &newState, uint32_t interval)
//     {
//         if (getTimeInState() > interval)
//             transitionTo(newState);
//     }

//     BaseState &getState()
//     {
//         return *currentState;
//     }

//     bool isState(BaseState &state)
//     {
//         return (currentState == &state);
//     }

//     uint32_t getTimeInState()
//     {
//         return millis() - stateChangeTime;
//     }
// };
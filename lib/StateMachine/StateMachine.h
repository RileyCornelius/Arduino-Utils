#pragma once

#include <Arduino.h>

// Example:

// void handleLedOn()
// {
//     digitalWrite(LED_BUILTIN, HIGH);
// }

// void handleLedOff()
// {
//     digitalWrite(LED_BUILTIN, LOW);
// }

// void handleLedBlinking()
// {
//     static unsigned long lastTime = 0;
//     static bool ledState = false;

//     if (millis() - lastTime > 1000)
//     {
//         lastTime = millis();
//         ledState = !ledState;
//         digitalWrite(LED_BUILTIN, ledState);
//     }
// }

// State LedOn(
//     "LedOn",
//     handleLedOn,
//     NULL,
//     NULL);
// State LedOff(
//     "LedOff",
//     handleLedOff,
//     NULL,
//     NULL);
// State LedBlinking(
//     "LedBlinking",
//     NULL,
//     handleLedBlinking,
//     NULL);

// StateMachine ledState(LedOff);
// void loop()
// {
//     if (button.released())
//     {
//         ledState.set(LedOn);
//     }
// ledState.handle();
// }

typedef void (*FunctionCallback)();

/**
 * @param name String name of the state
 * @param onEnter Callback function when entering the state
 * @param handler Callback function when handling the state
 * @param onExit Callback function when exiting the state
 */
struct State
{
    State(String name, FunctionCallback onEnter, FunctionCallback handler, FunctionCallback onExit)
        : name(name), onEnter(onEnter), handler(handler), onExit(onExit) {}
    String name;
    FunctionCallback onEnter;
    FunctionCallback handler;
    FunctionCallback onExit;

    bool operator==(State &other)
    {
        State *otherPtr = &other;
        return this == otherPtr;
    }

    bool operator!=(State &other)
    {
        State *otherPtr = &other;
        return this != otherPtr;
    }
};

class StateMachine
{
private:
    State state;

public:
    StateMachine(State &initialState) : state(initialState) {}
    bool is(State &isState) { return (state == isState); }
    State get() { return state; }
    void set(State &newState)
    {
        if (state != newState)
        {
            if (state.onExit != NULL)
                state.onExit();

            Serial.println(state.name + " => " + newState.name);

            if (newState.onEnter != NULL)
                newState.onEnter();

            state = newState;
        }
    }
    void handle()
    {
        if (state.handler != NULL)
            state.handler();
    }
};
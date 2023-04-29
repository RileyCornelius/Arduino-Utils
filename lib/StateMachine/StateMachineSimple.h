#pragma once

#include <Arduino.h>

// typedef void (*FunctionCallback)();
// FunctionCallback stateHandlers[MAX_SIZE] = {
//     []()
//     { Serial.println("handleLedOn"); },
//     []()
//     { Serial.println("handleLedOff"); },
//     []()
//     { Serial.println("handleLedBlinking"); },
// };

// void handle()
// {
//     EnumState enumState = LedState::get();
//     for (int i = 0; i < MAX_SIZE; i++)
//     {
//         if (enumState == i)
//         {
//             stateHandlers[i]();
//             break;
//         }
//     }
// }

#define DEBUG 1

#if DEBUG
const char *stateNames[] = {
    "ledOn",
    "ledOff",
    "ledBlinking",
};
#endif

enum EnumState
{
    LedOn,
    LedOff,
    LedBlinking,

    MAX_SIZE
};

namespace LedState
{
    volatile EnumState state;

    bool is(EnumState isState) { return (state == isState); }
    EnumState get() { return state; }
    void set(EnumState newState)
    {
        if (state != newState)
        {
#if DEBUG
            Serial.println(String(stateNames[state]) + " => " + stateNames[newState]);
#endif
            state = newState;
        }
    }
};

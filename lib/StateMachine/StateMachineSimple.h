#pragma once

#include <Arduino.h>
#include <map>

#define DEBUG 1

enum EnumState
{
    LedOn,
    LedOff,
    LedBlinking,

    MAX_SIZE
};

#if DEBUG
std::map<volatile EnumState, const char *> stateMap = {
    {LedOn, "ledOn"},
    {LedOff, "ledOff"},
    {LedBlinking, "ledBlinking"}};
#endif

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
            Serial.println(String(stateMap[state]) + " => " + stateMap[newState]);
#endif
            state = newState;
        }
    }
};

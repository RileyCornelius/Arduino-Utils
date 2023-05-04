#pragma once

#ifndef BUTTON_STATE_PRINT
// #define BUTTON_STATE_PRINT 1
#endif

enum ButtonStateEnum
{
    IDLE,
    DEBOUNCING,
    CLICKED,
    PRESSED,
    RELEASED,
    LONG_PRESSED,
};

class ButtonState
{

private:
#if BUTTON_STATE_PRINT
    const String buttonStateNames[6] = {
        "IDLE",
        "DEBOUNCING",
        "CLICKED",
        "PRESSED",
        "RELEASED",
        "LONG_PRESSED",
    };
#endif
    ButtonStateEnum currentState = IDLE;
    ButtonStateEnum lastState = currentState;
    bool firstRun = true;

public:
    bool was(ButtonStateEnum state) { return (lastState == state); }
    bool is(ButtonStateEnum state) { return (currentState == state); }
    ButtonStateEnum get() { return currentState; }
    void set(ButtonStateEnum newState)
    {
        if (currentState != newState)
        {
#if BUTTON_STATE_PRINT
            Serial.println(buttonStateNames[currentState] + " => " + buttonStateNames[newState]);
#endif
            firstRun = true;
            lastState = currentState;
            currentState = newState;
        }
    }
    bool justChanged()
    {
        if (firstRun)
        {
            firstRun = false;
            return true;
        }
        return false;
    }
};
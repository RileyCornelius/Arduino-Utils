#pragma once

#ifndef BUTTON_STATE_PRINT
// #define BUTTON_STATE_PRINT 1
#endif

enum ButtonStateEnum
{
    IDLE,
    DEBOUNCING,
    PRESSED,
    CLICKED,
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
        "PRESSED",
        "CLICKED",
        "RELEASED",
        "LONG_PRESSED",
    };
#endif

    ButtonStateEnum currentState = IDLE;
    bool enteredState = true;

public:
    bool is(ButtonStateEnum state) { return (currentState == state); }
    ButtonStateEnum get() { return currentState; }
    void set(ButtonStateEnum newState)
    {
        if (currentState != newState)
        {
#if BUTTON_STATE_PRINT
            Serial.println(buttonStateNames[currentState] + " => " + buttonStateNames[newState]);
#endif
            enteredState = true;
            currentState = newState;
        }
    }

    bool justEntered()
    {
        if (enteredState)
        {
            enteredState = false;
            return true;
        }
        return false;
    }
};
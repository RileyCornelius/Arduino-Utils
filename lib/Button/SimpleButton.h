#pragma once

#include <Arduino.h>

class SimpleButton
{
private:
    int8_t lastState = -1; // -1 means not initialized
    int8_t heldState = -1;
    bool activeState;

    uint8_t pin;
    uint32_t lastTime;

public:
    uint32_t debounceDelayMs = 20; // Default 20 ms

    SimpleButton()
    {
    }

    SimpleButton(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeState = LOW)
    {
        init(pinNumber, inputMode, activeState);
    }

    /**
     * Default inputMode: INPUT_PULLUP - Default activeState: LOW
     * \param pin The pin number of the button
     * \param inputMode The type of input - INPUT, INPUT_PULLUP, INPUT_PULLDOWN
     * \param buttonActiveState The state that triggers the button
     */
    void init(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t buttonActiveState = LOW)
    {
        pinMode(pinNumber, inputMode);
        pin = pinNumber;
        activeState = buttonActiveState;
    }

    /**
     * \return True only once when the button is pressed after the debounce delay
     */
    bool pressed()
    {
        bool currentState = digitalRead(pin);
        uint32_t now = millis();

        if (currentState != lastState)
        {
            lastTime = now;
            lastState = currentState;
        }
        else if (now - lastTime > debounceDelayMs && currentState != heldState)
        {
            heldState = currentState; // Prevents entering this block again on the same press
            if (currentState == activeState)
            {
                return true;
            }
        }

        return false;
    }
};
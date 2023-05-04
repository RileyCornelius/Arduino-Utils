#pragma once

#include <Arduino.h>
#include "ButtonState.h"

class TheButton
{
private:
    uint8_t pin;
    bool activeState;
    ButtonState state;

    uint8_t lastPressedState = LOW;

    uint16_t lastDebounceTime = 0;
    uint16_t lastPressTime = 0;
    uint16_t lastClickTime = 0;
    uint16_t clickCount = 0;

    uint16_t debounceDelay = 20;
    uint16_t clickDelay = 500;
    uint16_t longPressDelay = 2000;

    // bool debounce();
    // bool checkPressed(uint16_t now, uint8_t state);
    // bool checkDebounce(u_int32_t now, uint8_t state);

public:
    TheButton();
    TheButton(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeHigh = 0);

    void setPinMode(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeHigh = 0);
    void setDebounce(uint32_t debounceDelayMs);
    void setClick(uint32_t clickDelayMs);
    void setLongPress(uint32_t longPressDelayMs);

    void check();

    bool clicked();
    bool doubleClicked();
    bool tripleClicked();
    bool pressed();
    bool released();
    bool longPressed();
    bool longPressReleased(bool runCheck = true);
};
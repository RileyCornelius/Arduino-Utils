#pragma once

#include <Arduino.h>
#include "ButtonState.h"

class Button
{
private:
    uint8_t pin;
    bool activeState;
    ButtonState state;

    uint16_t lastDebounceTime = 0;
    uint16_t lastPressTime = 0;
    uint16_t lastClickTime = 0;
    uint16_t clickCount = 0;

    uint16_t debounceDelay = 20;
    uint16_t clickDelay = 400;
    uint16_t longPressDelay = 1000;

public:
    Button();
    Button(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeState = LOW);

    void init(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeState = LOW);

    void setDebounceDelay(uint16_t debounceDelayMs) { debounceDelay = debounceDelayMs; }
    void setClickDelay(uint16_t clickDelayMs) { clickDelay = clickDelayMs; }
    void setLongPressDelay(uint16_t longPressDelayMs) { longPressDelay = longPressDelayMs; }

    bool clicked();
    bool doubleClicked();
    bool tripleClicked();
    bool pressed();
    bool released();
    bool longPressed();
    bool longPressReleased();

    void check();
};
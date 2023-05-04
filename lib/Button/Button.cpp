#include "Button.h"

TheButton::TheButton()
{
}

TheButton::TheButton(uint8_t pinNumber, uint8_t inputMode /* = INPUT_PULLUP */, uint8_t isActiveHigh /* = 0 */)
{
    setPinMode(pinNumber, inputMode, isActiveHigh);
}

/**
 * \param pin The pin number of the button
 * \param inputMode The type of input - INPUT, INPUT_PULLUP, INPUT_PULLDOWN
 * \param activeHigh The state that triggers the button - LOW or HIGH
 */
void TheButton::setPinMode(uint8_t pinNumber, uint8_t inputMode /* = INPUT_PULLUP */, uint8_t activeHigh /* = 0 */)
{
    pinMode(pinNumber, inputMode);
    pin = pinNumber;
    activeState = activeHigh;
}

void TheButton::setDebounce(uint32_t debounceDelayMs)
{
    debounceDelay = debounceDelayMs;
}
void TheButton::setClick(uint32_t clickDelayMs)
{
    clickDelay = clickDelayMs;
}
void TheButton::setLongPress(uint32_t longPressDelayMs)
{
    longPressDelay = longPressDelayMs;
}

bool TheButton::clicked()
{
    return (state.is(CLICKED) && clickCount == 1);
}

bool TheButton::doubleClicked()
{
    return (state.is(CLICKED) && clickCount == 2);
}

bool TheButton::tripleClicked()
{
    return (state.is(CLICKED) && clickCount == 3);
}

bool TheButton::pressed()
{
    return (state.is(PRESSED) && state.justChanged());
}

bool TheButton::released()
{
    return (state.is(RELEASED));
}

bool TheButton::longPressed()
{
    return (state.is(LONG_PRESSED) && state.justChanged());
}

bool TheButton::longPressReleased(bool runCheck /* = true */)
{
    return (state.is(RELEASED) && state.was(LONG_PRESSED) && state.justChanged());
}

void TheButton::check()
{
    bool pressedState = (digitalRead(pin) == activeState);

    switch (state.get())
    {
    case (IDLE):
        if (pressedState)
        {
            lastDebounceTime = millis();
            state.set(DEBOUNCING);
        }
        break;

    case (DEBOUNCING):
        if (millis() - lastDebounceTime > debounceDelay)
        {
            if (pressedState)
            {
                lastPressTime = millis();
                state.set(PRESSED);
            }
            else
            {
                state.set(IDLE);
            }
        }
        break;

    case (PRESSED):
        if (!pressedState)
        {
            state.set(CLICKED);
        }
        else if (millis() - lastPressTime > longPressDelay)
        {
            state.set(LONG_PRESSED);
        }
        break;

    case (CLICKED):
        if (millis() - lastClickTime < clickDelay)
        {
            clickCount++;
        }
        else
        {
            clickCount = 1;
            lastClickTime = millis();
        }
        state.set(RELEASED);
        break;

    case (RELEASED):
        state.set(IDLE);
        break;

    case (LONG_PRESSED):
        if (!pressedState)
        {
            state.set(RELEASED);
        }
        break;
    }
}

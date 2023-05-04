#pragma once

#include <Arduino.h>

#if defined(ARDUINO_ARCH_STM32)
class Button2
{
private:
    bool prevState;
    bool state;
    bool activeState;
    uint32_t prevTime; // Milliseconds
    uint8_t pin;

public:
    uint32_t holdTime; // Milliseconds

    Button2()
    {
        holdTime = 50;
    }

    Button2(uint8_t pinNumber) : Button2()
    {
        setPin(pinNumber);
    }

    Button2(uint8_t pinNumber, bool isActiveHigh) : Button2()
    {
        setPin(pinNumber, isActiveHigh);
    }

    /**
     * \param pin The pin number of the button
     * \param isActiveHigh The state that triggers the button
     */
    void setPin(uint8_t pinNumber, bool isActiveHigh = 0)
    {
        pin = pinNumber;
        activeState = isActiveHigh;
        pinMode(pin, isActiveHigh ? INPUT_PULLDOWN : INPUT_PULLUP); // supports pulldown and pullup resistors
    }

    /**
     * \return True if button is held for 'holdTime' default is 50 milliseconds
     */
    bool debounce()
    {
        uint8_t reading = digitalRead(pin);

        if (reading != prevState)
            prevTime = millis();

        if ((millis() - prevTime) > holdTime && reading != state)
        {
            state = reading;
            if (state == activeState)
            {
                return true;
            }
        }
        prevState = reading;

        return false;
    }

    operator bool() { return debounce(); }
};
#else
class Button2
{
private:
    bool prevState;
    bool state;
    uint32_t prevTime; // Milliseconds
    uint8_t pin;

public:
    uint32_t holdTime; // Milliseconds

    Button2()
    {
        holdTime = 50;
    }

    Button2(uint8_t pinNumber) : Button2()
    {
        setPin(pinNumber);
    }

    /**
     * \param pinNumber The pin number of the button
     * \param isActiveHigh The state that triggers the button
     */
    void setPin(uint8_t pinNumber)
    {
        pin = pinNumber;
        pinMode(pin, INPUT_PULLUP);
    }

    /**
     * \return True if button is held for 'holdTime' default is 50 milliseconds
     */
    bool debounce()
    {
        uint8_t reading = digitalRead(pin);

        if (reading != prevState)
            prevTime = millis();

        if ((millis() - prevTime) > holdTime && reading != state)
        {
            state = reading;
            if (state == LOW)
            {
                return true;
            }
        }
        prevState = reading;

        return false;
    }

    operator bool() { return debounce(); }
};
#endif
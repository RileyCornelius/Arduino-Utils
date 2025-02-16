#pragma once

#include <Arduino.h>

class DigitalInput
{
private:
    uint8_t pin;

public:
    DigitalInput() {};
    DigitalInput(uint8_t pin, uint8_t mode = INPUT) { init(pin, mode); }

    /// @brief Initializes the digital input pin
    /// @param pinNumber The pin number to initialize
    /// @param mode The mode of the pin (INPUT, INPUT_PULLUP, INPUT_PULLDOWN) default is INPUT
    void init(const uint8_t pinNumber, uint8_t mode = INPUT)
    {
        pin = pinNumber;
        pinMode(pin, mode);
    }

    bool read() { return digitalRead(pin); }
};
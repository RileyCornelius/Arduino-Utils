#pragma once

#include <Arduino.h>

template <typename TPin = uint8_t>
class DigitalInput
{
private:
    uint8_t pin;

public:
    DigitalInput() {};
    DigitalInput(TPin pin, uint8_t mode = INPUT) { init(pin, mode); }

    /// @brief Initializes the digital input pin
    /// @param pinNumber The pin number to initialize
    /// @param mode The mode of the pin (INPUT, INPUT_PULLUP, INPUT_PULLDOWN) default is INPUT
    void init(const TPin pinNumber, uint8_t mode = INPUT)
    {
        pin = (uint8_t)pinNumber;
        pinMode(pin, mode);
    }

    bool read()
    {
        return digitalRead(pin);
    }
};
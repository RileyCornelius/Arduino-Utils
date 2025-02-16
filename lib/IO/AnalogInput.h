#pragma once

#include <Arduino.h>

template <typename TPin = uint8_t>
class AnalogInput
{
private:
    uint8_t pin;

public:
    AnalogInput() {};
    AnalogInput(TPin pin) { init(pin); }

    /// @brief Initializes the analog input pin
    /// @param pinNumber The pin number to initialize
    void init(const TPin pinNumber)
    {
        pin = (uint8_t)pinNumber;
        pinMode(pin, INPUT);
    }

    uint16_t read()
    {
        return analogRead(pin);
    }

    float readVoltage()
    {
        return (analogRead(pin) * 3.3f) / 1023;
    }

    float readPercentage()
    {
        return ((analogRead(pin)) / 1023.0f) * 100;
    }
};
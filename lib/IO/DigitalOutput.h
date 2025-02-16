#pragma once

#include <Arduino.h>

template <typename TPin = uint8_t>
class DigitalOutput
{
private:
    uint8_t pin;

public:
    DigitalOutput() {};
    DigitalOutput(TPin pin, uint8_t mode = OUTPUT) { init(pin, mode); }

    /// @brief Initializes the digital output pin
    /// @param pinNumber The pin number to use
    /// @param mode The mode for the pin (OUTPUT, PULLUP, PULLDOWN) default is OUTPUT
    void init(TPin pinNumber, uint8_t mode = OUTPUT)
    {
        pin = (uint8_t)pinNumber;
        pinMode(pin, mode);
    }

    bool isOn() { return digitalRead(pin); }
    void on() { digitalWrite(pin, HIGH); }
    void off() { digitalWrite(pin, LOW); }
    void set(bool value) { digitalWrite(pin, value); }
    void toggle() { digitalWrite(pin, !digitalRead(pin)); }
};

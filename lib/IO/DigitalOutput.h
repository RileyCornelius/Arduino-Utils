#pragma once

#include <Arduino.h>

class DigitalOutput
{
private:
    uint8_t pin;

public:
    DigitalOutput() {};
    DigitalOutput(uint8_t pin, uint8_t mode = OUTPUT) { init(pin, mode); }

    /// @brief Initializes the digital output pin
    /// @param pinNumber The pin number to use
    /// @param mode The mode for the pin (OUTPUT, PULLUP, PULLDOWN) default is OUTPUT
    void init(uint8_t pinNumber, uint8_t mode = OUTPUT)
    {
        pin = pinNumber;
        pinMode(pin, mode);
    }

    void on() { digitalWrite(pin, HIGH); }
    void off() { digitalWrite(pin, LOW); }
    void set(bool value) { digitalWrite(pin, value); }
    void toggle() { digitalWrite(pin, !digitalRead(pin)); }
    bool isOn() { return digitalRead(pin); }
};

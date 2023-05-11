#pragma once

#include <Arduino.h>

class DigitalOutput
{
private:
    uint8_t pin;

public:
    DigitalOutput(){};
    DigitalOutput(uint8_t pinNumber) { init(pinNumber); }

    void init(uint8_t pinNumber)
    {
        pin = pinNumber;
        pinMode(pin, OUTPUT);
    }

    bool isOn() { return digitalRead(pin); }

    void on() { digitalWrite(pin, HIGH); }

    void off() { digitalWrite(pin, LOW); }

    void toggle() { digitalWrite(pin, !digitalRead(pin)); }

    /**
     * \param scale Duty cycle of the pulse width modulation with a scale of 0-255
     */
    void pwm(uint8_t scale) { analogWrite(pin, scale); }
};

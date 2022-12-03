#pragma once

#include <Arduino.h>

// #if defined(NRF52)
// #if defined(ESP32)

#if defined(ARDUINO_ARCH_AVR)
class DigitalOutput
{
private:
    uint8_t _pin;

public:
    DigitalOutput(){};
    DigitalOutput(uint8_t pin) { setPin(pin); }

    /**
     * \param pin The pin number of the digital output
     */
    void setPin(uint8_t pin)
    {
        _pin = pin;
        pinMode(_pin, OUTPUT);
    }

    void on() { digitalWrite(_pin, HIGH); }

    void off() { digitalWrite(_pin, LOW); }

    void toggle() { digitalWrite(_pin, ~digitalRead(_pin)); }

    /**
     * \param scale Duty cycle of the pulse width modulation with a scale of 0-255
     */
    void pwm(uint8_t scale) { analogWrite(_pin, scale); }
};

#elif defined(ARDUINO_ARCH_STM32)
class DigitalOutput
{
private:
    PinName _pin;

public:
    DigitalOutput(){};
    DigitalOutput(uint8_t pin) { setPin(pin); }

    /**
     * \param pin The pin number of the digital output
     */
    void setPin(uint8_t pin)
    {
        _pin = digitalPinToPinName(pin);
        pinMode(_pin, OUTPUT);
    }

    void on() { digitalWriteFast(_pin, HIGH); }

    void off() { digitalWriteFast(_pin, LOW); }

    void toggle() { digitalToggleFast(_pin); }

    /**
     * \param scale Duty cycle of the pulse width modulation with a scale of 0-255
     */
    void pwm(uint8_t scale) { analogWrite(_pin, scale); }
};

#endif
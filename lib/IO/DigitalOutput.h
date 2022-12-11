#pragma once

#include <Arduino.h>

// #if defined(NRF52)
// #if defined(ESP32)

#if defined(ARDUINO_ARCH_STM32)
class DigitalOutput
{
private:
    PinName pin;

public:
    DigitalOutput(){};
    DigitalOutput(uint8_t pinNumber) { setPin(pinNumber); }

    /**
     * \param pinNumber The pin number of the digital output
     */
    void setPin(uint8_t pinNumber)
    {
        pin = digitalPinToPinName(pin);
        pinMode(pin, OUTPUT);
    }

    void on() { digitalWriteFast(pin, HIGH); }

    void off() { digitalWriteFast(pin, LOW); }

    void toggle() { digitalToggleFast(pin); }

    /**
     * \param scale Duty cycle of the pulse width modulation with a scale of 0-255
     */
    void pwm(uint8_t scale) { analogWrite(pin, scale); }
};

#else
class DigitalOutput
{
private:
    uint8_t pin;

public:
    DigitalOutput(){};
    DigitalOutput(uint8_t pinNumber) { setPin(pinNumber); }

    /**
     * \param pinNumber The pin number of the digital output
     */
    void setPin(uint8_t pinNumber)
    {
        pin = pinNumber;
        pinMode(pin, OUTPUT);
    }

    void on() { digitalWrite(pin, HIGH); }

    void off() { digitalWrite(pin, LOW); }

    void toggle() { digitalWrite(pin, !digitalRead(pin)); }

    /**
     * \param scale Duty cycle of the pulse width modulation with a scale of 0-255
     */
    void pwm(uint8_t scale) { analogWrite(pin, scale); }
};

#endif
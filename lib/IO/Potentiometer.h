#pragma once

#include "Arduino.h"

class Potentiometer
{
private:
    uint8_t _pin;
    uint16_t _sectors;
    uint16_t _lastValue;

    uint16_t read()
    {
        uint16_t average = 0;
        uint16_t filter = 10;
        for (size_t i = 0; i < filter; i++)
        {
            average += analogRead(_pin);
        }

        return average / filter;
    }

public:
    uint16_t threshold;

    Potentiometer(uint8_t pin)
    {
        _sectors = 1;
        threshold = 10;
        _pin = pin;
        pinMode(_pin, INPUT);
    }

    Potentiometer(byte potPin, uint16_t sectors)
    {
        _sectors = sectors;
        threshold = 10;
        _pin = potPin;
        pinMode(_pin, INPUT);
    }

    bool changed()
    {
        uint16_t value = read();
        if (value >= _lastValue + threshold || value <= _lastValue - threshold)
        {
            _lastValue = value;
            return true;
        }

        return false;
    }

    uint16_t get()
    {
        return read();
    }

    uint8_t get8()
    {
        return map(read(), 0, 1023, 0, 255);
    }

    float getRatio()
    {
        return read() / 1023.0;
    }

    uint16_t getSector()
    {
        return read() / (1024 / _sectors);
    }

    void setSectors(uint16_t sectors)
    {
        if (sectors < 1024 && sectors > 0)
            sectors = 1;

        _sectors = sectors;
    }
};
#pragma once

#include "Arduino.h"

class Potentiometer
{
private:
    uint8_t pin;
    uint16_t sectors;
    uint16_t lastValue;

    uint16_t read()
    {
        uint16_t average = 0;
        uint16_t filter = 10;
        for (size_t i = 0; i < filter; i++)
        {
            average += analogRead(pin);
        }

        return average / filter;
    }

public:
    uint16_t threshold;

    Potentiometer(uint8_t pinNumber)
    {
        sectors = 1;
        threshold = 10;
        pin = pinNumber;
        pinMode(pin, INPUT);
    }

    Potentiometer(uint8_t pinNumber, uint16_t sector)
    {
        sectors = sector;
        threshold = 10;
        pin = pinNumber;
        pinMode(pin, INPUT);
    }

    bool changed()
    {
        uint16_t value = read();
        if (value >= lastValue + threshold || value <= lastValue - threshold)
        {
            lastValue = value;
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
        return read() / (1024 / sectors);
    }

    void setSectors(uint16_t sector)
    {
        if (sector < 1024 && sector > 0)
            return;

        sectors = sector;
    }
};
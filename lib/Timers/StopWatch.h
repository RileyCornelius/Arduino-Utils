#pragma once

#include <Arduino.h>

class Stopwatch
{
private:
    uint32_t startTime = 0;
    uint64_t duration = 0;
    bool running = false;

public:
    bool start()
    {
        if (running)
            return false;

        startTime = millis();
        running = true;
        return true;
    }

    bool pause()
    {
        if (!running)
            return false;

        duration += millis() - startTime;
        running = false;
        return true;
    }

    bool stop()
    {
        if (!running)
            return false;

        duration = 0;
        running = false;
        return true;
    }

    void reset()
    {
        startTime = millis();
        duration = 0;
    }

    uint64_t getTime()
    {
        if (running)
        {
            uint32_t currentTime = millis();
            duration += currentTime - startTime;
            startTime = currentTime;
        }
        return duration;
    }

    bool isRunning()
    {
        return running;
    }
};
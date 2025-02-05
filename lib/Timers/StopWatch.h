#pragma once

#include <Arduino.h>

class Stopwatch
{
private:
    uint32_t startTime = 0;
    uint64_t duration = 0;
    bool running = false;

    virtual uint32_t now() { return millis(); }

public:
    bool start()
    {
        if (running)
            return false;

        startTime = now();
        running = true;
        return true;
    }

    bool pause()
    {
        if (!running)
            return false;

        duration += now() - startTime;
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
        startTime = now();
        duration = 0;
    }

    uint64_t getTime()
    {
        if (running)
        {
            uint32_t currentTime = now();
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

class StopwatchMicros : public Stopwatch
{
private:
    uint32_t now() override { return micros(); }
};
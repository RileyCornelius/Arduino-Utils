#include <Arduino.h>

class StopWatch
{
private:
    uint32_t startTime = 0;
    uint32_t stopTime = 0;
    bool running = false;

public:
    void start()
    {
        startTime = millis();
        running = true;
    }

    void stop()
    {
        stopTime = millis();
        running = false;
    }

    void reset()
    {
        startTime = millis();
        stopTime = millis();
    }

    uint32_t getTime()
    {
        if (running)
            return millis() - startTime;
        else
            return stopTime - startTime;
    }

    bool isRunning()
    {
        return running;
    }
};
#include <Arduino.h>

class StopWatch
{
private:
    uint32_t startTime = 0;
    uint32_t duration = 0;
    bool running = false;

public:
    void start()
    {
        startTime = millis();
        running = true;
    }

    void stop()
    {
        duration += millis() - startTime;
        running = false;
    }

    void reset()
    {
        startTime = millis();
        duration = 0;
    }

    uint32_t getTime()
    {
        if (running)
            return millis() - startTime + duration;
        else
            return duration;
    }

    bool isRunning()
    {
        return running;
    }
};
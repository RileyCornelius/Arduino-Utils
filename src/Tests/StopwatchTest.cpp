#include <Arduino.h>
#include <Stopwatch.h>
#include <SimpleTimer.h>
#include <SimpleButton.h>

static Stopwatch stopwatch;
static Timer timer(1000);
static SimpleButton button(2);
static SimpleButton button2(3);

void stopWatchTest()
{
    if (button.pressed())
    {
        if (stopwatch.isRunning())
        {
            Serial.println("stop");
            stopwatch.stop();
        }
        else
        {
            Serial.println("start");
            stopwatch.start();
        }
    }

    if (stopwatch.getTime() > 5000)
    {
        Serial.println("reset");
        stopwatch.reset();
    }

    if (timer.isReady())
    {
        // Serial.println(stopwatch.getTime() / 1000);
    }
}

void stopwatchSetup()
{
}

void stopwatchLoop()
{
    stopWatchTest();
}
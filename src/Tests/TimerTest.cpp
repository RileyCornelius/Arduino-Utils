#include <SimpleTimer.h>

static Timer timer = Timer(1000);

void timerSetup()
{
}

void timerLoop()
{
    if (timer)
    {
        Serial.println("Timer isReady");
    }
}
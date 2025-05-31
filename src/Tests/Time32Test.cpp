#include <Time32.h>

#ifdef ARDUINO_ARCH_ESP32
Time32 timeService;

void time32Setup()
{
    timeService.setTimeFromComputer();
    Serial.println("Time32 Setup");
    Serial.println(timeService.getDateTime());
}

void time32Loop()
{
    delay(1000);
    Serial.println(timeService.getFormattedTime("%A, %B %Y-%m-%d %H:%M:%S"));
}
#endif
#include <DigitalOutput.h>

#define LED_PIN 5

enum class Pin : uint8_t
{
    LED1 = 3,
    LED2 = 4,
};
using LedDigitalOutput = DigitalOutput<Pin>;

static LedDigitalOutput led = LedDigitalOutput(Pin::LED1);
static DigitalOutput led2;

void digitalOutputSetup()
{
    led2.init(LED_PIN);
}

void digitalOutputLoop()
{
    led.on();
    led2.on();
    delay(1000);
    led.off();
    led2.off();
    delay(1000);
    led.toggle();
    led2.toggle();
    delay(1000);
    led.toggle();
    led2.toggle();
    delay(1000);
    led.set(true);
    led2.set(true);
    delay(1000);
    led.set(false);
    led2.set(false);
    delay(1000);
}
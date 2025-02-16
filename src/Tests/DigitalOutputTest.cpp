#include <DigitalOutput.h>

#define LED_PIN 5

enum class Led : uint8_t
{
    LED1 = 3,
    LED2 = 4,
};
using LedDigitalOutput = DigitalOutput<Led>;

static LedDigitalOutput led = LedDigitalOutput(Led::LED1);
static DigitalOutput<uint8_t> led2;

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
#include <DigitalOutput.h>

#define LED_PIN 5

enum Led : uint8_t
{
    LED1 = 3,
    LED2 = 4,
};

static DigitalOutput led1;
static DigitalOutput led2;

void digitalOutputSetup()
{
    led1.init(LED1);
    led2.init(LED_PIN);
}

void digitalOutputLoop()
{
    led1.on();
    led2.on();
    delay(1000);
    led1.off();
    led2.off();
    delay(1000);
    led1.toggle();
    led2.toggle();
    delay(1000);
    led1.toggle();
    led2.toggle();
    delay(1000);
    led1.set(true);
    led2.set(true);
    delay(1000);
    led1.set(false);
    led2.set(false);
    delay(1000);
}
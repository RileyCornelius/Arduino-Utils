#include <Arduino.h>

#include <Button.h>
#include <SimpleTimer.h>
#include <DigitalOutput.h>
#include <types.h>

Timer timer = Timer(1000);
Button button = Button(2);
DigitalOutput led = DigitalOutput(3);

uint16 x;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (button)
  {
    x++;
    Serial.println("Pressed " + x);
  }

  if (timer)
  {
    led.toggle();
  }
}
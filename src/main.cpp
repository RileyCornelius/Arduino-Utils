#include <Arduino.h>
#include "Tests/_Tests.h"

static const char *TAG = "Main";

void setup()
{
  Serial.begin(115200);
  delay(1000);

  arraySetup();
}

void loop()
{
  // timerLoop();
  // simpleStateMachineLoop();

  arrayLoop();
}
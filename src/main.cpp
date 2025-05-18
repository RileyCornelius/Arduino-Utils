#include <Arduino.h>
#include "Tests/_Tests.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  loggerSetup();
}

void loop()
{
  loggerLoop();
}
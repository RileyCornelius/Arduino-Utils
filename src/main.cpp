#include <Arduino.h>
#include "Tests/_Tests.h"
// #include "log.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.print("C++ version: ");
  Serial.println(__cplusplus);
  // LOG_I("Starting tests...");

  optionalSetup();
}

void loop()
{
  optionalLoop();
}
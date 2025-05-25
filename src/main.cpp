#include <Arduino.h>
#include "Tests/_Tests.h"
// #include "log.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  loggerSetup();
  Serial.print("C++ version: ");
  Serial.println(__cplusplus);
  // LOG_I("Starting tests...");
}

void loop()
{
  loggerLoop();
}
#include <Arduino.h>
#include "Tests/_Tests.h"
#include <array>
#include <etl/array.h>

#include <map>
#include <etl/map.h>
// #include "log.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting ESP32 Tests...");
  Serial.print("C++ version: ");
  Serial.println(__cplusplus);

  loggerSetup();
}

void loop()
{
  loggerLoop();
}
#pragma once

// Toggle debug mode
#ifndef DEBUG
#define DEBUG 0
#endif

// Define debug print macros
#if DEBUG
#define PRINTLN(x) Serial.println(x)                   // Debug print new line
#define PRINT(x) Serial.print(x)                       // Debug print
#define PRINTF(x, ...) Serial.printf(x, ##__VA_ARGS__) // Debug print formatted
#else
#define PRINTLN(x)
#define PRINT(x)
#define PRINTF(x, ...)
#endif
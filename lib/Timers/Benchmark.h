#pragma once

// Toggle debug benchmarking
#ifndef DEBUG_BENCHMARK
#define DEBUG_BENCHMARK 1
#endif

// Benchmarking macros
#if DEBUG_BENCHMARK
uint32_t _prevTime_;

// Gets the current time, use BENCHMARK_END get elapsed time in milliseconds
#define BENCHMARK_BEGIN() _prevTime_ = millis()
// Prints elapsed time since BENCHMARK_BEGIN with a label before the time in milliseconds
#define BENCHMARK_END(label) Serial.println(label + String((millis() - _prevTime_)) + " ms");

// Gets the current time, use BENCHMARK_MICROS_END get elapsed time in microseconds
#define BENCHMARK_MICROS_BEGIN() _prevTime_ = micros()
// Prints elapsed time since BENCHMARK_MICROS_BEGIN with a label before the time in microseconds
#define BENCHMARK_MICROS_END(label) Serial.println(label + String((micros() - _prevTime_)) + " us")
#else
#define BENCHMARK_BEGIN()
#define BENCHMARK_END(label)

#define BENCHMARK_MICROS_BEGIN()
#define BENCHMARK_MICROS_END(label)

#endif
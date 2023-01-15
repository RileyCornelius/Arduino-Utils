/**--------------------------------------------------------------------------------------
 * Benchmark Macros
 *-------------------------------------------------------------------------------------*/

uint32_t _prevTime_;

// Toggle debug benchmarking
#ifndef DEBUG_BENCHMARK
#define DEBUG_BENCHMARK 1
#endif

// Benchmarking macros
#if DEBUG_BENCHMARK

// Gets the current time, use BENCHMARK_END get elapsed time
#define BENCHMARK_BEGIN() _prevTime_ = millis()
// Prints elapsed time since BENCHMARK_BEGIN
#define BENCHMARK_END() Serial.println(millis() - _prevTime_);
// Prints elapsed time since BENCHMARK_BEGIN with a label before the time
#define BENCHMARK_LABEL_END(label) Serial.println(String(label) + (millis() - _prevTime_))
#else
#define BENCHMARK_BEGIN()
#define BENCHMARK_END()
#endif
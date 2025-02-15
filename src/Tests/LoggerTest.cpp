#include <Arduino.h>
#include "log.h"
#include "Benchmark.h"

void loggerSetup()
{
    // Serial.begin(115200);
}

void loggerLoop()
{
    // LOG_I("std" << 1.00);

    // BENCHMARK_MICROS_BEGIN();
    // LOG_VERBOSE(LOG_TAG, "Verbose");
    // BENCHMARK_MICROS_END("LOG_VERBOSE execution time: ");

    // BENCHMARK_MICROS_BEGIN();
    // LOG_INFO(LOG_TAG, "Info");
    // BENCHMARK_MICROS_END("LOG_INFO execution time: ");

    // LOG_DEBUG(LOG_TAG, "Debug");
    // LOG_WARNING("LOG", "TEST got %d", 1);

    // LOG_E("Error %s", "error");
    // LOG_W("Warning %d", 1);

    // BENCHMARK_MICROS_BEGIN();
    // Serial.println("Serial3");
    // Serial.flush();
    // BENCHMARK_MICROS_END("Serial3 execution time: ");

    // BENCHMARK_MICROS_BEGIN();
    // log_i("Info");
    // BENCHMARK_MICROS_END("log_i execution time: ");

    // BENCHMARK_MICROS_BEGIN();
    // ESP_LOGI("gd", "Info");
    // BENCHMARK_MICROS_END("ESP execution time: ");
}
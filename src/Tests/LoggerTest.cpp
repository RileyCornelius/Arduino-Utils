#include <Arduino.h>
#include <esp32-hal-log.h>
// #include "log32.h"
// #include "LogC.h"
#include "Log.h"
#include "Benchmark.h"

static const char *TAG = "TAG";

void loggerSetup()
{
    Serial.begin(115200);
}

void loggerLoop()
{
    BENCHMARK_MICROS_BEGIN(LOG__I);
    LOG_I("Timeing %d %s", 1, "ms");
    BENCHMARK_MICROS_END(LOG__I);

    delay(1000);

    BENCHMARK_MICROS_BEGIN(log__d);
    log_d("Dbug %d, %s", 1, "debug");
    BENCHMARK_MICROS_END(log__d);

    delay(1000);

    BENCHMARK_MICROS_BEGIN(LOG__INFO2);
    LOG_INFO("TAG2", "Sensor %d %s", 34, "%");
    BENCHMARK_MICROS_END(LOG__INFO2);

    delay(1000);

    BENCHMARK_MICROS_BEGIN(LOG__INFO3);
    LOG_INFO("TAG3", "Sensor %d %s", 352, "%");
    LOG_OUTPUT.flush();
    BENCHMARK_MICROS_END(LOG__INFO3);

    delay(1000);

    BENCHMARK_MICROS_BEGIN(ESP__LOGD);
    ESP_LOGD(TAG, "Dbug %d, %s", 1.0, "debug");
    BENCHMARK_MICROS_END(ESP__LOGD);

    delay(1000);

    BENCHMARK_MICROS_BEGIN(Seral);
    Serial.printf("[INFO] [TAG] Info %d %s\n", 1, "seral");
    BENCHMARK_MICROS_END(Seral);

    delay(1000);
}
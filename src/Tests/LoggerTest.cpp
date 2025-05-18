#include <Arduino.h>
// #include <esp32-hal-log.h>
// #include "log32.h"
// #include "LogC.h"

#include <string_view>

#include "Log.h"
#include "Benchmark.h"

// #define LOG_PREAMBLE "-"
// #include "DebugLog.h"

// #include <format>
// #include "fmt.h"

static const char *TAG = "TAG";

void loggerSetup()
{
    Serial.begin(115200);
}

void loggerLoop()
{
    int i = rand();
    const char *str = "Hello, world!";
    float f = 1.11;
    std::string_view strView = "Hello, world!";

    // BENCHMARK_MICROS_BEGIN(log_x_fmt);
    // LOG_INFO(TAG, "test {} {} {}", strView.data(), i, f);
    // // LOG_W("f {} {} {}", str, i, f);
    // BENCHMARK_MICROS_END(log_x_fmt);

    // BENCHMARK_MICROS_BEGIN(ESP__LOGD);
    // ESP_LOGD(TAG, "test  %s %f %f", str, i, f);
    // BENCHMARK_MICROS_END(ESP__LOGD);

    // BENCHMARK_MICROS_BEGIN(DebugLog1);
    // for (int j = 0; j < 20; j++)
    // LOG_WARN(TAG, "test", str, i, f);
    // BENCHMARK_MICROS_END(DebugLog1);

    BENCHMARK_MICROS_BEGIN(LOG_INFO2);
    LOG_INFO(TAG, "test %s %d %.2f", str, i, f);
    BENCHMARK_MICROS_END(LOG_INFO2);

    BENCHMARK_MICROS_BEGIN(serial);
    // for (int j = 0; j < 20; j++)
    Serial.printf("[INFO][%s] test %s %d %.2f\r\n", TAG, str, i, f);
    BENCHMARK_MICROS_END(serial);

    BENCHMARK_MICROS_BEGIN(SPrintf);
    char fmtBuffer[64];
    sprintf(fmtBuffer, "[INFO][%s] test %s %d %.2f", TAG, str, i, f);
    Serial.println(fmtBuffer);
    BENCHMARK_MICROS_END(SPrintf);

    // BENCHMARK_MICROS_BEGIN(LOG__I);
    // for (int j = 0; j < 20; j++)
    //     LOG_I("[%s:%f:%s] test [%s] %s %f %f", FILE_NAME, __LINE__, __FUNCTION__, TAG, str, j, f);
    // BENCHMARK_MICROS_END(LOG__I);

    // BENCHMARK_MICROS_BEGIN(fmtPrint);
    // // fmt::print("test [{}] {} {} {}\n", TAG, str, i, f);
    // // char *buff[64];
    // // std::string
    // // std::string str = std::format("[INFO] test [{}] {} {} {}", TAG, str, i, f);
    // // Serial.println(str.c_str());
    // // std::string fmtStr = std::format("[INFO] test [{}] {} {} {}", TAG, str, i, f);
    // // Serial.println(fmtStr.c_str());
    // BENCHMARK_MICROS_END(fmtPrint);

    // BENCHMARK_MICROS_BEGIN(format);
    // const char *buff[64];
    // fmt::format_to(buff, "[INFO] test [{}] {} {} {}", TAG, str, i, f);
    // fmt::format_to(buff, "[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // Serial.print(buff);
    // std::string str_view = std::format("[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // Serial.print(str_view.c_str());
    // BENCHMARK_MICROS_END(format);

    // BENCHMARK_MICROS_BEGIN(_Logger);
    // char fmtBuffer[50];
    // sprintf(fmtBuffer, "[INFO] test [%s] %s %f %f", TAG, str, i, f);
    // BENCHMARK_MICROS_END(_Logger);

    Serial.println("");

    delay(10000);
}
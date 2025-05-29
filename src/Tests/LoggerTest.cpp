#include <Arduino.h>
#include <string_view>
// #include <string>

// #include <esp32-hal-log.h>
// #include "log32.h"
// #include "LogC.h"

#include "Benchmark.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "format.h"
#include "DebugLog.h"

#include <format>
#include "fmt.h"
// #include "simple_fmt.h"
// #include "arduino_fmt.h"

static const char *TAG = "LOG";

void loggerSetup()
{
    Serial.begin(115200);
}

void loggerLoop()
{
    // int i = millis();
    // std::string_view strView = "Hello, world!";
    int i = rand();
    float f = (i / 10440.3f) + 1.11f;

    int value = rand() / 10000;
    int small_value = value / 10003;
    int neg_value = -value / 105;
    float pi = 3.1415f;
    const char *text = "Hello";
    int *intPtr = &i;
    int hex = 0x45;

    constexpr const char *format = "hex:{:#x} shift:{:<5} pi:{} precision:{:.6} neg:{} float precision:{:.5f} \n";

    Serial.println();

    // BENCHMARK_MICROS_BEGIN(ardfmt_toPrint);
    // char tesBuffer[64];
    // afmt::format_to(tesBuffer, format, hex, "str", pi, f, neg_value,  f);
    // BENCHMARK_MICROS_END(ardfmt_toPrint)
    // Serial.print(tesBuffer);

    BENCHMARK_MICROS_BEGIN(ardfmtPrint);
    std::string afmtString = afmt::format(format, hex, "str", pi, f, neg_value, f);
    BENCHMARK_MICROS_END(ardfmtPrint)
    Serial.print(afmtString.c_str());
    
    BENCHMARK_MICROS_BEGIN(fmtPrint);
    std::string fmtString = fmt::format(format, hex, "str", pi, f, neg_value,  f);
    BENCHMARK_MICROS_END(fmtPrint);
    Serial.print(fmtString.c_str());
    
    BENCHMARK_MICROS_BEGIN(stdFormat);
    std::string fom = std::format(format, hex, "str", pi, f, neg_value, f);
    BENCHMARK_MICROS_END(stdFormat);
    Serial.print(fom.c_str());
    

    // BENCHMARK_MICROS_BEGIN(fmt_toPrint);
    // // Serial.print(fmt::format(format, hex, "str", i, f, neg_value, 4).c_str());
    // char fmtBuff[100];
    // fmt::format_to_n(fmtBuff, 100, format, hex, "str", i, f, neg_value, 4, text);
    // BENCHMARK_MICROS_END(fmt_toPrint);
    // Serial.print(fmtBuff);

    // BENCHMARK_MICROS_BEGIN(ardfmtPrint);
    // // char buffer[100];
    // // afmt::format_to_n(buffer, 100, format, hex, "str", i, f, neg_value);
    // // Serial.print(buffer);
    // // afmt::format_to(buffer, format, hex, "str", i, f, neg_value);
    // // Serial.print(buffer);
    // // Serial.print(afmt::format(format, hex, "str", i, f, neg_value).c_str());
    // // Serial.print(afmt::format(format, hex, "str", i, f, neg_value, 4).c_str());
    // // afmt::print(format, hex, "str", i, f, neg_value);
    // // LOG_DEBUG(TAG, "{:#x} {:>4} {:11} {:.3f} {:+} {{ hi }}\n", hex, "str", i, f, neg_value);
    // std::string ardfmtString = afmt::format(format, hex, "str", i, f, neg_value, 4, text).c_str();
    // BENCHMARK_MICROS_END(ardfmtPrint);
    // Serial.print(ardfmtString.c_str());

    // BENCHMARK_MICROS_BEGIN(fPrint);
    // char padded_str[7];
    // sprintf(padded_str, "%6s", "str"); // Right-align (but no custom fill char)

    // char vbuffer[64];
    // sprintf(vbuffer, "0X%X %s %16d %.3f %d { hi }\n",
    //         hex, padded_str, i, f, neg_value);
    // std::string fString = vbuffer;
    // BENCHMARK_MICROS_END(fPrint);
    // Serial.print(fString.c_str());

    // char buff[64];
    // // fmt::format_to(buff,  "[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // BENCHMARK_MICROS_BEGIN(fmtPrint);
    // Serial.print(fmt::format("[INFO] test [{}] {} {} {}\n", TAG, "str", i, f).c_str());
    // BENCHMARK_MICROS_END(fmtPrint);

    // BENCHMARK_MICROS_BEGIN(CUSTOM_FORMAT);
    // char buffer[100];
    // fmt::format_to_n(buffer, sizeof(buffer), "Value: {:14}, Pi: {:.4f}, Text: {} Hex: {:#x} Neg: {:+}", value, pi, text, hex, neg_value);
    // Serial.println(buffer);
    // BENCHMARK_MICROS_END(CUSTOM_FORMAT);

    // BENCHMARK_MICROS_BEGIN(DEBUG_LOG_SINGLE);
    // LOG_DEBUG(TAG, "test ", strView.data(), i, f);
    // BENCHMARK_MICROS_END(DEBUG_LOG_SINGLE);

    // BENCHMARK_MICROS_BEGIN(DEBUG_LOG);
    // LOG_TRACE(TAG, "test ", strView.data(), i, f);
    // LOG_INFO("Test");
    // LOG_DEBUG(TAG, "test", strView.data(), i, f);
    // LOG_WARN(TAG, "test", strView.data(), i, f);
    // LOG_ERROR(TAG, "test", strView.data(), i, f);
    // BENCHMARK_MICROS_END(DEBUG_LOG);

    // #if LOG_PRINT_TYPE == LOG_PRINT_TYPE_PRINTF
    //     BENCHMARK_MICROS_BEGIN(PRINTF);
    //     LOG_V("V %s %d %.3f", strView.data(), i, f);
    //     LOG_D("D %s %d %.3f", strView.data(), i, f);
    //     LOG_I("I %s %d %.3f", strView.data(), i, f);
    //     LOG_W("W %s %d %.3f", strView.data(), i, f);
    //     LOG_E("E %s %d %.3f", strView.data(), i, f);
    //     BENCHMARK_MICROS_END(PRINTF);

    //     LOG_PRINTLN();

    //     LOG_VERB(TAG, "VERB %s %d %.3f", strView.data(), i, f);
    //     LOG_DEBUG(TAG, "DBUG %s %d %.3f", strView.data(), i, f);
    //     LOG_INFO(TAG, "INFO %s %d %.3f", strView.data(), i, f);
    //     LOG_WARN(TAG, "WARN %s %d %.3f", strView.data(), i, f);
    //     LOG_ERROR(TAG, "EROR %s %d %.3f", strView.data(), i, f);

    //     LOG_PRINTLN();
    // #endif

    // #if LOG_PRINT_TYPE == LOG_PRINT_TYPE_STD_FORMAT || LOG_PRINT_TYPE == LOG_PRINT_TYPE_FMT_FORMAT
    //     BENCHMARK_MICROS_BEGIN(FMT);
    //     LOG_V("V {} {} {:.3f}", strView.data(), i, f);
    //     LOG_D("D {} {} {:.3f}", strView.data(), i, f);
    //     LOG_I("I {} {} {:.3f}", strView.data(), i, f);
    //     LOG_W("W {} {} {:.2f}", strView.data(), i, f);
    //     LOG_E("E {} {} {:.1f}", strView.data(), i, f);

    //     LOG_PRINTLN();

    //     LOG_VERB(TAG, "VERB {} {:d} {:.3f}", strView.data(), i, f);
    //     LOG_DEBUG(TAG, "DBUG {} {} {:.3f}", strView.data(), i, f);
    //     LOG_INFO(TAG, "INFO {} {} {:.3f}", strView.data(), i, f);
    //     LOG_WARN(TAG, "WARN {} {} {:.2f}", strView.data(), i, f);
    //     LOG_ERROR(TAG, "EROR {} {} {:.1f}", strView.data(), i, f);
    //     BENCHMARK_MICROS_END(FMT);

    //     LOG_PRINTLN();
    // #endif

    // ASSERT(i > 0, "i must be greater than 0");
    delay(10000);
}

void loggerBenchmark()
{
    // int i = rand();
    // const char *str = "Hello, world!";
    // float f = 1.11;
    // std::string_view strView = "Hello, world!";
    // String str2 = "Hello, world!";

    // // Serial.printf(str2);

    // BENCHMARK_MICROS_BEGIN(log_i_fmt);
    // // LOG_I("test {} {} {}", strView.data(), i, f);
    // // LOG_WARN("War", "test {} {} {}", strView.data(), i, f);
    // // LOG_WARN("DSP", "test {} {} {}", strView.data(), i, f);

    // LOG_W("f %s %d %.2f", str, i, f);
    // // LOG_W("f {} {} {}", str, i, f);
    // BENCHMARK_MICROS_END(log_i_fmt);

    // BENCHMARK_MICROS_BEGIN(log_w_fmt);
    // // LOG_I("test {} {} {}", strView.data(), i, f);
    // // LOG_W("test {} {} {}", strView.data(), i, f);

    // // LOG_WARN("War", "test {} {} {}", strView.data(), i, f);
    // LOG_WARN(TAG, "test %s %d %.2f", str, i, f);
    // BENCHMARK_MICROS_END(log_w_fmt);

    // // BENCHMARK_MICROS_BEGIN(ESP__LOGD);
    // // ESP_LOGD(TAG, "test  %s %f %f", str, i, f);
    // // BENCHMARK_MICROS_END(ESP__LOGD);

    // // BENCHMARK_MICROS_BEGIN(DebugLog1);
    // // for (int j = 0; j < 20; j++)
    // // LOG_WARN(TAG, "test", str, i, f);
    // // BENCHMARK_MICROS_END(DebugLog1);

    // // BENCHMARK_MICROS_BEGIN(LOG_INFO2);
    // // LOG_INFO(TAG, "test %s %d %.2f", str, i, f);
    // // BENCHMARK_MICROS_END(LOG_INFO2);

    // BENCHMARK_MICROS_BEGIN(serial);
    // // for (int j = 0; j < 20; j++)
    // Serial.printf("[INFO][%s] test %s %d %.2f\r\n", TAG, str, i, f);
    // BENCHMARK_MICROS_END(serial);

    // BENCHMARK_MICROS_BEGIN(SPrintf);
    // char fmtBuffer[64];
    // sprintf(fmtBuffer, "[INFO][%s] test %s %d %.2f", TAG, str, i, f);
    // Serial.println(fmtBuffer);
    // BENCHMARK_MICROS_END(SPrintf);

    // // BENCHMARK_MICROS_BEGIN(LOG__I);
    // // for (int j = 0; j < 20; j++)
    // //     LOG_I("[%s:%f:%s] test [%s] %s %f %f", FILE_NAME, __LINE__, __FUNCTION__, TAG, str, j, f);
    // // BENCHMARK_MICROS_END(LOG__I);

    // // BENCHMARK_MICROS_BEGIN(fmtPrint);
    // // // fmt::print("test [{}] {} {} {}\n", TAG, str, i, f);
    // // // char *buff[64];
    // // // std::string
    // // // std::string str = std::format("[INFO] test [{}] {} {} {}", TAG, str, i, f);
    // // // Serial.println(str.c_str());
    // // // std::string fmtStr = std::format("[INFO] test [{}] {} {} {}", TAG, str, i, f);
    // // // Serial.println(fmtStr.c_str());
    // // BENCHMARK_MICROS_END(fmtPrint);

    // // BENCHMARK_MICROS_BEGIN(format);
    // // fmt::format_to(buff, "[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // // std::string strFor = fmt::format("[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // // Serial.print(strFor.c_str());
    // // std::string str_view = std::format("[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // // Serial.print(str_view.c_str());

    // // char buff[64];
    // // fmt::format_to_n(buff, 64, "[INFO] test [{}] {} {} {}\n", TAG, str, i, f);
    // // Serial.print(fmt::format("[INFO] test [{}] {} {} {}\n", TAG, str, i, f).c_str());

    // // BENCHMARK_MICROS_END(format);

    // // BENCHMARK_MICROS_BEGIN(_Logger);
    // // char fmtBuffer[50];
    // // sprintf(fmtBuffer, "[INFO] test [%s] %s %f %f", TAG, str, i, f);
    // // BENCHMARK_MICROS_END(_Logger);

    // BENCHMARK_MICROS_BEGIN(concat_str);
    // // String str1 = "Hello";
    // // str1 += ", ";
    // // str1 += "World";
    // // str1 += "!";
    // // BENCHMARK_MICROS_END(concat_str);
    // // Serial.println(str1);

    // // BENCHMARK_MICROS_BEGIN(concat_str2);
    // // String str2 = "Hello" + String(", ") + "World" + String("!");
    // // BENCHMARK_MICROS_END(concat_str2);
    // // Serial.println(str2);

    // Serial.println("");

    // delay(10000);
}
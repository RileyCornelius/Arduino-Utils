#pragma once

const char *pathToName(const char *path);

#include <esp32-hal-log.h>
#undef ARDUHAL_LOG_FORMAT
#define ARDUHAL_LOG_FORMAT(letter, format) ARDUHAL_LOG_COLOR_##letter "[" #letter "][%s:%u] " format ARDUHAL_LOG_RESET_COLOR "\r\n", pathToName(__FILE__), __LINE__
// #define ARDUHAL_LOG_FORMAT(letter, format) ARDUHAL_LOG_COLOR_##letter "[%6u][" #letter "][%s:%u] %s(): " format ARDUHAL_LOG_RESET_COLOR "\r\n", (unsigned long)(esp_timer_get_time() / 1000ULL), pathToFileName(__FILE__), __LINE__, __FUNCTION__

#pragma once

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

/**--------------------------------------------------------------------------------------
 * Example
 *-------------------------------------------------------------------------------------*/

//   WiFi.begin("SSID", "PASSWORD");
//   Time32 time32;
//   time32.setTimeFromNTP(TIMEZONE_UTC);
//   Serial.println(time32.getDateTime());

/**--------------------------------------------------------------------------------------
 * ESP32 Time library using C standard time library and Network Time Protocol (NTP)
 *-------------------------------------------------------------------------------------*/

#define TIMEZONE_WINNIPEG "CST6CDT,M3.2.0,M11.1.0" // America/Winnipeg timezone
#define TIMEZONE_UTC "UTC0"                        // Universal Time Coordinated timezone

#define NTP_SERVER0 "0.ca.pool.ntp.org" // Canada NTP server 0
#define NTP_SERVER1 "1.ca.pool.ntp.org" // Canada NTP server 1
#define NTP_SERVER2 "2.ca.pool.ntp.org" // Canada NTP server 2

class Time32
{
private:
    // Buffer to store the formatted date and time
    char dataTime[20];
    char date[11];
    char time[9];

    struct tm timeinfo = {0};

public:
    Time32() {};

    Time32(tm time)
    {
        setTime(time);
    };

    Time32(tm time, const char *tz)
    {
        setTimeZone(tz);
        setTime(time); // called after setTimeZone else time will shift
    };

    Time32(int seconds, int minutes, int hours, int day, int month, int year)
    {
        struct tm time = {0}; // Initialize the timeinfo struct with all zeros
        time.tm_sec = seconds;
        time.tm_min = minutes;
        time.tm_hour = hours;
        time.tm_mday = day;
        time.tm_mon = month - 1;    // tm_mon is value 0-11
        time.tm_year = year - 1900; // tm_year is the number of years since 1900
        setTime(time);
    };

    /**
     * Update timeinfo struct with the current time
     * This function must be called before year(), month(), day(), hour(), minute(), second()
     */
    void updateTime()
    {
        getLocalTime(&timeinfo);
    }

    /**
     * Set time using the computers time at compilation
     */
    void setTimeFromComputer()
    {
        String timeDate = String(__TIME__) + " " + String(__DATE__);
        strptime(timeDate.c_str(), "%H:%M:%S %b %d %Y", &timeinfo);
        setTime(timeinfo);
    }

    /**
     * Set time using time struct
     *
     * @param time Time struct
     */
    void setTime(tm &time)
    {
        timeinfo = time;
        timeval tv = {
            .tv_sec = mktime(&time),
            .tv_usec = 0,
        };
        settimeofday(&tv, NULL);
        updateTime();
    }

    /**
     * Set time using time struct
     *
     * @param year Year
     * @param month Month
     * @param day Day
     * @param hour Hour
     * @param min Minute
     * @param sec Second
     */
    void setTime(int year, int month, int day, int hour, int min, int sec)
    {
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = min;
        timeinfo.tm_sec = sec;
        setTime(timeinfo);
        updateTime();
    }

    /**
     * Config time using the Network Time Protocol server and adds a timezone
     *
     * @param tz Timezone strings can be found here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     * @param timeout Timeout in milliseconds (default 10000)
     */
    bool setTimeFromNTP(const char *tz = TIMEZONE_UTC, int timeout = 10000)
    {
        configTzTime(tz, NTP_SERVER0, NTP_SERVER1, NTP_SERVER2); // esp32 function to set time using NTP and a timezone
        return getLocalTime(&timeinfo, timeout);                 // wait timeout ms for the time to sync
    }

    /**
     * Config timezone using standard library functions should be called before setTime
     *
     * @param tz Timezone strings can be found here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     */
    void setTimeZone(const char *tz)
    {
        setenv("TZ", tz, 1);
        tzset();
        updateTime();
    }

    /**
     * Get string formatted time - https://cplusplus.com/reference/ctime/strftime/
     *
     * @param buff buffer to store the time string
     * @param size size of the buffer
     * @param format format of the time string
     * @return Formatted time string
     */
    const char *getFormattedTime(char *buff, size_t size, const char *format)
    {
        updateTime();
        strftime(buff, size, format, &timeinfo);
        return buff;
    }

    /**
     * Get string formatted time - https://cplusplus.com/reference/ctime/strftime/
     *
     * @param format format of the time string
     * @return Formatted time string
     */
    String getFormattedTime(const char *format)
    {
        updateTime();
        char buff[64];
        strftime(buff, sizeof(buff), format, &timeinfo);
        return String(buff);
    }

    /**
     * Get current date in YYYY-MM-DD format
     */
    const char *getDate()
    {
        updateTime();
        sprintf(date, "%04d-%02d-%02d", year(), month(), day());
        return date;
    }

    /**
     * Get current time in HH:MM:SS format
     */
    const char *getTime()
    {
        updateTime();
        sprintf(time, "%02d:%02d:%02d", hour(), minute(), second());
        return time;
    }

    /**
     * Get current time in YYYY-MM-DD HH:MM:SS format
     */
    const char *getDateTime()
    {
        updateTime();
        sprintf(dataTime, "%04d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
        return dataTime;
    }

    int year()
    {
        return timeinfo.tm_year + 1900; // since 1900
    }

    int month()
    {
        return timeinfo.tm_mon + 1; // tm_mon is value 0-11
    }

    int day()
    {
        return timeinfo.tm_mday;
    }

    int hour()
    {
        return timeinfo.tm_hour;
    }

    int minute()
    {
        return timeinfo.tm_min;
    }

    int second()
    {
        return timeinfo.tm_sec;
    }
};

#endif
#pragma once

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

/**--------------------------------------------------------------------------------------
 * Example
 *-------------------------------------------------------------------------------------*/

//   WiFi.begin("SSID", "PASSWORD");
//   CTime time;
//   time.setTimeFromNTP(WINNIPEG_TIMEZONE);
//   time.updateTime();
//   Serial.println(time.getFormatDateTime());

/**--------------------------------------------------------------------------------------
 * ESP32 Time library using C standard time library and Network Time Protocol (NTP)
 *-------------------------------------------------------------------------------------*/

#define WINNIPEG_TIMEZONE "CST6CDT,M3.2.0,M11.1.0" // America/Winnipeg timezone
#define UTC_TIMEZONE "UTC0"                        // Universal Time Coordinated timezone

class CTime
{
private:
    const char *ntpServer0 = "0.ca.pool.ntp.org"; // Canada NTP server 0
    const char *ntpServer1 = "1.ca.pool.ntp.org"; // Canada NTP server 1
    const char *ntpServer2 = "2.ca.pool.ntp.org"; // Canada NTP server 2

    // Buffer to store the formatted date and time
    char dataTime[20];
    char date[11];
    char time[9];

    struct tm timeinfo = {0};

public:
    CTime() {};

    CTime(tm time)
    {
        setTime(time);
    };

    CTime(tm time, const char *tz)
    {
        setTimeZone(tz);
        setTime(time); // called after setTimeZone else time will shift
    };

    CTime(int seconds, int minutes, int hours, int day, int month, int year)
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
     * Update time using standard library functions
     */
    void updateTime()
    {
        if (!getLocalTime(&timeinfo)) // esp32 function to get internal time
        {
            Serial.println("Set time is invalid");
        }
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

    // /**
    //  * Set time using time struct
    //  *
    //  * @param time Time struct
    //  */
    void setTime(tm &time)
    {
        timeinfo = time;
        timeval tv = {
            .tv_sec = mktime(&time),
            .tv_usec = 0,
        };
        settimeofday(&tv, NULL);
    }

    void setTime(int year, int month, int day, int hour, int min, int sec)
    {
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = min;
        timeinfo.tm_sec = sec;
        setTime(timeinfo);
    }

    /**
     * Config time using the Network Time Protocol server and adds a timezone
     *
     * @param tz Timezone strings can be found here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     */
    void setTimeFromNTP(const char *tz = UTC_TIMEZONE)
    {
        configTzTime(tz, ntpServer0, ntpServer1, ntpServer2); // esp32 function to set time using NTP and a timezone
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
    }

    /**
     * Config time and date using standard library functions
     *
     * @param time time struct
     */
    void setTime(tm &time)
    {
        timeinfo = time;
        time_t t = mktime(&timeinfo);
        struct timeval now = {.tv_sec = t};
        settimeofday(&now, NULL);
    }

    /**
     * Config time and date using standard library functions
     */
    void setTime(int seconds, int minutes, int hours, int day, int month, int year)
    {
        struct tm time = {0}; // Initialize the timeinfo struct with all zeros
        time.tm_sec = seconds;
        time.tm_min = minutes;
        time.tm_hour = hours;
        time.tm_mday = day;
        time.tm_mon = month - 1;    // tm_mon is value 0-11
        time.tm_year = year - 1900; // tm_year is the number of years since 1900
        setTime(time);
    }

    /**
     * Get string formatted time - https://cplusplus.com/reference/ctime/strftime/
     *
     * @param buff buffer to store the time string
     * @param size size of the buffer
     * @param format format of the time string
     * @return Formatted time string
     */
    const char *getStrfTime(char *buff, size_t size, const char *format)
    {
        strftime(buff, size, format, &timeinfo);
        return buff;
    }

    const char *getDate()
    {
        sprintf(date, "%04d-%02d-%02d",
                getYear(),
                getMonth(),
                getDay());

        return date;
    }

    const char *getTime()
    {
        sprintf(time, "%02d:%02d:%02d",
                getHours(),
                getMinutes(),
                getSeconds());

        return time;
    }

    const char *getDateTime()
    {
        sprintf(dataTime, "%s %s",
                getDate(),
                getTime());

        return dataTime;
    }

    int getYear()
    {
        return timeinfo.tm_year + 1900; // since 1900
    }

    int getMonth()
    {
        return timeinfo.tm_mon + 1; // tm_mon is value 0-11
    }

    int getDay()
    {
        return timeinfo.tm_mday;
    }

    int getHours()
    {
        return timeinfo.tm_hour;
    }

    int getMinutes()
    {
        return timeinfo.tm_min;
    }

    int getSeconds()
    {
        return timeinfo.tm_sec;
    }
};

#endif
#pragma once

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>
#include <WiFi.h>

/**--------------------------------------------------------------------------------------
 * Example
 *-------------------------------------------------------------------------------------*/

//   WiFi.begin("SSID", "PASSWORD");
//   NTPTime time;
//   time.setTimeNTPAfterWiFi(10000);
//   time.updateTime();
//   Serial.println(time.getFormatDateTime());

/**--------------------------------------------------------------------------------------
 * Time library for ESP32 using Network Time Protocol (NTP)
 *-------------------------------------------------------------------------------------*/

const char *NTP_SERVER = "pool.ntp.org";                   // NTP server
const char *WINNIPEG_TIME_ZONE = "CST6CDT,M3.2.0,M11.1.0"; // America/Winnipeg timezone

class NTPTime
{
private:
    const char weekDayName[7][4] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char monthDayName[12][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    char dataTime[20];
    char date[11];
    char time[9];

public:
    struct tm timeinfo;
    NTPTime(){};

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
     * Config time using NTP and a timezone
     *
     * @param tz Timezone can be found here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
     */
    void setTimeNTP(const char *tz = WINNIPEG_TIME_ZONE)
    {
        configTzTime(tz, NTP_SERVER); // esp32 function to set time using NTP and a timezone
    }

    /**
     * Blocks until connected to WiFi then config time using NTP and a timezone
     *
     * @param timeOutMillis Time out in milliseconds
     * @return True if connected to WiFi
     */
    bool setTimeNTPAfterWiFi(int32_t timeOutMillis, const char *timeZone = WINNIPEG_TIME_ZONE)
    {
        Serial.print("Waiting for WiFi.");
        while (WiFi.status() != WL_CONNECTED && timeOutMillis > 0)
        {
            delay(300);
            Serial.print(".");
            timeOutMillis -= 300;
        }
        if (timeOutMillis <= 0)
        {
            Serial.println("Wifi connection timed out");
            return false;
        }

        Serial.println("");
        configTzTime(timeZone, NTP_SERVER);
        return true;
    }

    const char *getFormatDate()
    {
        sprintf(date, "%04d-%02d-%02d",
                getYear(),
                getMonth(),
                getDay());

        return date;
    }

    const char *getFormatTime()
    {
        sprintf(time, "%02d:%02d:%02d",
                getHour(),
                getMinute(),
                getSecond());

        return time;
    }

    const char *getFormatDateTime()
    {
        sprintf(dataTime, "%s %s",
                getFormatDate(),
                getFormatTime());

        return dataTime;
    }

    const char *getWeekDayName()
    {
        return weekDayName[timeinfo.tm_wday];
    }

    const char *getMonthName()
    {
        return monthDayName[timeinfo.tm_mon];
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

    int getHour()
    {
        return timeinfo.tm_hour;
    }

    int getMinute()
    {
        return timeinfo.tm_min;
    }

    int getSecond()
    {
        return timeinfo.tm_sec;
    }
};

#endif
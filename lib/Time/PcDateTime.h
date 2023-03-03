#pragma once

#include <Arduino.h>

class PcDateTime
{
public:
    uint8_t year;    ///< Year 0-99
    uint8_t month;   ///< Month 1-12
    uint8_t day;     ///< Day 1-31
    uint8_t hours;   ///< Hours 0-23
    uint8_t minutes; ///< Minutes 0-59
    uint8_t seconds; ///< Seconds 0-59

    PcDateTime()
    {
        const __FlashStringHelper *date = F(__DATE__);
        const __FlashStringHelper *time = F(__TIME__);

        char buff[11];
        memcpy(buff, date, 11);
        year = conv2d(buff + 9);
        // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
        switch (buff[0])
        {
        case 'J':
            month = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7);
            break;
        case 'F':
            month = 2;
            break;
        case 'A':
            month = buff[2] == 'r' ? 4 : 8;
            break;
        case 'M':
            month = buff[2] == 'r' ? 3 : 5;
            break;
        case 'S':
            month = 9;
            break;
        case 'O':
            month = 10;
            break;
        case 'N':
            month = 11;
            break;
        case 'D':
            month = 12;
            break;
        }
        day = conv2d(buff + 4);
        memcpy(buff, time, 8);
        hours = conv2d(buff);
        minutes = conv2d(buff + 3);
        seconds = conv2d(buff + 6);
    }

    String toString()
    {
        char buff[20];
        sprintf(buff, "%02d/%02d/%02d %02d:%02d:%02d", year, month, day, hours, minutes, seconds);
        return String(buff);
    }

private:
    uint8_t conv2d(const char *p)
    {
        uint8_t v = 0;
        if ('0' <= *p && *p <= '9')
            v = *p - '0';
        return 10 * v + *++p - '0';
    }
};
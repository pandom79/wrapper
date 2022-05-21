/*
(C) 2021 by Domenico Panella <pandom79@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3.
See http://www.gnu.org/licenses/gpl-3.0.html for full license text.
*/

#include "wrapper.h"

int
msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void
stringSetDateTime(char **ret, bool hasMilliseconds)
{
    int millisec;
    char millisecStr[5];
    struct tm* timeInfo;
    struct timeval tv;
    char dateTime[50];

    gettimeofday(&tv, NULL);

    timeInfo = localtime(&tv.tv_sec);
    strftime(dateTime, 50, "%d %B %Y %H:%M:%S", timeInfo);
    if (hasMilliseconds) {
        millisec = tv.tv_usec/1000.0;
        if (millisec >= 1000) {
            millisec -= 1000;
            tv.tv_sec++;
        }
        sprintf(millisecStr, ".%03d", millisec);
        strcat(dateTime, millisecStr);
    }
    objectRelease(ret);
    *ret = stringNew(dateTime);
}

DateTime*
dateTimeCreate(DateTime *dateTimeFrom)
{
    DateTime *dateTime = calloc(1, sizeof(DateTime));
    assert(dateTime);

    //Sec
    int32_t *sec = calloc(1, sizeof(int32_t));
    assert(sec);

    //Millisec
    int32_t *millisec = calloc(1, sizeof(int32_t));
    assert(millisec);

    if (!dateTimeFrom) {
        //Timeval
        struct timeval timeVal;
        gettimeofday(&timeVal, NULL);
        *sec = timeVal.tv_sec;
        *millisec = timeVal.tv_usec/1000.0;
        if (*millisec >= 1000) {
            *millisec -= 1000;
            (*sec)++;
        }
    }
    else {
        *sec = *dateTimeFrom->sec;
        *millisec = *dateTimeFrom->millisec;
    }

    dateTime->sec = sec;
    dateTime->millisec = millisec;

    return dateTime;
}

void
dateTimeRelease(DateTime **dateTime)
{
    DateTime *dateTimeTemp = *dateTime;
    if (dateTimeTemp) {
        objectRelease(&dateTimeTemp->sec);
        objectRelease(&dateTimeTemp->millisec);
        objectRelease(&dateTimeTemp);
    }
}

void
dateTimeSet(DateTime **dateTime)
{
    if (*dateTime)
        dateTimeRelease(dateTime);
    *dateTime = dateTimeCreate(NULL);
}


void
stringSetCurrentTime(char **ret, DateTime *dateTime, bool hasMilliseconds)
{
    char dateTimeStr[50];
    char millisecStr[5];

    struct timeval tv;
    tv.tv_sec = *dateTime->sec;

    struct tm *timeInfo = localtime(&tv.tv_sec);

    strftime(dateTimeStr, 50, "%d %B %Y %H:%M:%S", timeInfo);
    if (hasMilliseconds) {
        sprintf(millisecStr, ".%03d", *dateTime->millisec);
        strcat(dateTimeStr, millisecStr);
    }
    objectRelease(ret);
    *ret = stringNew(dateTimeStr);
}

void
stringSetDiffTime(char **ret, double diffTime, DateTime *dateTimeEnd, DateTime *dateTimeStart)
{
    char timeStr[50];
    int day, hour, min, sec;
    day = hour = min = sec = -1;

    int32_t *millisecStart = dateTimeStart->millisec;
    int32_t *millisecEnd = dateTimeEnd->millisec;

    if (diffTime > 0) {
        uint32_t diffMillisec = (1000 - *millisecStart + *millisecEnd);
        if (diffMillisec < 1000)
            diffTime--;
        else
            diffMillisec = diffMillisec - 1000;

        sec = diffTime;
        if (sec >= 60) {
            min = diffTime / 60;
            if (min > 0) {
                sec -= min * 60;
            }
        }
        if (min >= 60) {
            hour = min / 60;
            min -= hour * 60;
        }
        if (hour >= 24) {
            day = hour / 24;
            hour -= day * 24;
        }

        if (day != -1 && day > 0) {
            char dayStr[10];
            sprintf(dayStr, "%dd ", day);
            strcat(timeStr, dayStr);
        }
        if (hour != -1 && hour > 0) {
            char hourStr[10];
            sprintf(hourStr, "%dh ", hour);
            strcat(timeStr, hourStr);
        }
        if (min != -1 && min > 0) {
            char minStr[10];
            sprintf(minStr, "%dm ", min);
            strcat(timeStr, minStr);
        }
        if (sec != -1) {
            char secStr[10];
            sprintf(secStr, "%d.%ds ", sec, diffMillisec);
            strcat(timeStr, secStr);
        }

    }
    else {
        uint32_t diffMillisec = *millisecEnd - *millisecStart;
        sprintf(timeStr, "0.%ds", diffMillisec);
    }
    objectRelease(ret);
    *ret = stringNew(timeStr);
}

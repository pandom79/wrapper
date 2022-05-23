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
    char millisecStr[5] = {0};
    struct tm* timeInfo;
    struct timeval tv;
    char dateTime[50] = {0};

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

Time*
timeNew(Time *timeFrom)
{
    Time *time = calloc(1, sizeof(Time));
    assert(time);

    //Sec
    long *sec = calloc(1, sizeof(long));
    assert(sec);

    //Millisec
    long *millisec = calloc(1, sizeof(long));
    assert(millisec);

    if (!timeFrom) {
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
        *sec = *timeFrom->sec;
        *millisec = *timeFrom->millisec;
    }

    time->sec = sec;
    time->millisec = millisec;

    return time;
}

void
timeRelease(Time **time)
{
    Time *timeTemp = *time;
    if (timeTemp) {
        objectRelease(&timeTemp->sec);
        objectRelease(&timeTemp->millisec);
        objectRelease(&timeTemp);
    }
}

void
timeSetCurrent(Time **time)
{
    if (*time)
        timeRelease(time);
    *time = timeNew(NULL);
}

void
stringSetTimeStamp(char **ret, Time *time, bool hasMilliseconds)
{
    char dateTimeStr[50] = {0};
    char millisecStr[5] = {0};
    long millisec = 0;

    struct timeval tv;
    if (time) {
        tv.tv_sec = *time->sec;
        millisec = *time->millisec;
    }
    else {
        gettimeofday(&tv, NULL);
        millisec = tv.tv_usec/1000.0;
        if (millisec >= 1000) {
            millisec -= 1000;
            tv.tv_sec++;
        }
    }

    struct tm *timeInfo = localtime(&tv.tv_sec);

    strftime(dateTimeStr, 50, "%d %B %Y %H:%M:%S", timeInfo);
    if (hasMilliseconds) {
        sprintf(millisecStr, ".%03ld", millisec);
        strcat(dateTimeStr, millisecStr);
    }
    objectRelease(ret);
    *ret = stringNew(dateTimeStr);
}

void
stringSetDiffTime(char **ret, Time *timeEnd, Time *timeStart)
{
    char timeStr[50] = {0};
    int day, hour, min, sec;
    day = hour = min = sec = -1;

    double diffTime = difftime(*timeEnd->sec, *timeStart->sec);
    long *millisecStart = timeStart->millisec;
    long *millisecEnd = timeEnd->millisec;

    if (diffTime > 0) {
        long diffMillisec = (1000 - *millisecStart + *millisecEnd);
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
            char dayStr[10] = {0};
            sprintf(dayStr, "%dd ", day);
            strcat(timeStr, dayStr);
        }
        if (hour != -1 && hour > 0) {
            char hourStr[10] = {0};
            sprintf(hourStr, "%dh ", hour);
            strcat(timeStr, hourStr);
        }
        if (min != -1 && min > 0) {
            char minStr[10] = {0};
            sprintf(minStr, "%dm ", min);
            strcat(timeStr, minStr);
        }
        if (sec != -1) {
            char secStr[10] = {0};
            sprintf(secStr, "%d.%lds ", sec, diffMillisec);
            strcat(timeStr, secStr);
        }

    }
    else {
        long diffMillisec = *millisecEnd - *millisecStart;
        sprintf(timeStr, "0.%lds", diffMillisec);
    }
    objectRelease(ret);
    *ret = stringNew(timeStr);
}

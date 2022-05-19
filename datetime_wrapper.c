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
printDiffTime(double diffTime)
{
    int day, hour, min, sec;
    day = hour = min = sec = -1;

    if (diffTime > 0) {
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

        if (day != -1 && day > 0)
            printf("%dd ", day);
        if (hour != -1 && hour > 0)
            printf("%dh ", hour);
        if (min != -1 && min > 0)
            printf("%dmin ", min);
        if (sec != -1 && sec > 0)
            printf("%dsec ", sec);
    }
    else
        printf("about 1sec");
    printf("\n");

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


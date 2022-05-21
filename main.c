#include "wrapper.h"

int main()
{
//    time_t now = time(NULL);
//    struct tm *localTime = localtime(&now);
//    printf("localTime->tm_sec = %d\n", localTime->tm_sec);
//    printf("localTime->tm_min = %d\n", localTime->tm_min);

////    localTime->tm_mday -= 1;
////    localTime->tm_hour -= 2;
//    localTime->tm_sec -= 1;
////    localTime->tm_min -= 1;

//    printf("localTime->tm_sec = %d\n", localTime->tm_sec);
//    printf("localTime->tm_min = %d\n", localTime->tm_min);

//    time_t start = mktime(localTime);

//    time_t now2 = time(NULL);

//    double diffTime = difftime(now2, start);
//    printf("diffTime = %0.0f\n", diffTime);

//    printDiffTime(diffTime);

//Example 2
//    time_t now = time(NULL);

//    msleep(500);

//    time_t after = time(NULL);

//    printDiffTime(difftime(after, now));

//TIME 1

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
//    localTime->tm_mday -= 4;
//    localTime->tm_mon -= 3;
    localTime->tm_hour -= 500;
    localTime->tm_min -= 43;
//    localTime->tm_sec -= 23000;

    DateTime *dateTime = dateTimeCreate(NULL);
    *dateTime->sec = mktime(localTime);

    dateTimeSet(&dateTime);


    char *date = NULL;
    stringSetCurrentTime(&date, dateTime, true);

    printf("Date = %s\n", date);

    msleep(105);


//TIME 2
    DateTime *dateTime2 = dateTimeCreate(NULL);
    stringSetCurrentTime(&date, dateTime2, true);
    printf("Date2 = %s\n", date);

//TIME DIFF

    long diffTimeLong = difftime(*dateTime2->sec, *dateTime->sec);
    char *diffTimeStr = NULL;
    stringSetDiffTime(&diffTimeStr, diffTimeLong, dateTime2, dateTime);
    printf("diffTimeStr = %s\n", diffTimeStr);
    objectRelease(&diffTimeStr);

    dateTimeRelease(&dateTime2);
    dateTimeRelease(&dateTime);
    objectRelease(&date);


}

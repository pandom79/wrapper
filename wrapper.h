#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stddef.h>
#include <errno.h>
#include <stdint.h>

/* Types */
typedef struct {
    void **arr;
    int size;
    void (*release_fn)(void **);
} Array;

/* String */
char* stringNew(const char *);
bool stringSet(char **, const char *);
const char* stringGet(char *str);
bool stringStartsWithChr(const char *, const char);
bool stringStartsWithStr(const char *, const char *);
bool stringEndsWithChr(const char *, const char);
bool stringEndsWithStr(const char *, const char *);
bool stringContainsChr(const char *str, const char c);
bool stringContainsStr(const char *, const char *);
void stringToupper(char *);
void stringTolower(char *);
bool stringAppendChr(char **, const char);
bool stringAppendStr(char **, const char *);
bool stringConcat(char **, const char *);
bool stringPrependChr(char **, const char);
bool stringPrependStr(char **, const char *);
bool stringInsertChrAt(char **, const char, int);
bool stringInsertStrAt(char **, const char *, int);
char* stringLtrim(char *, const char *);
char* stringRtrim(char *, const char *);
char* stringTrim(char *, const char *);
int stringIndexOfChr(const char *, const char);
int stringIndexOfStr(const char *, const char *);
int stringLastIndexOfChr(const char *, const char);
int stringLastIndexOfStr(const char *, const char *);
char* stringSub(const char *, int, int);
void stringReplaceChr(char **, const char, const char);
void stringReplaceAllChr(char **, const char, const char);
void stringReplaceStr(char **, const char *, const char *);
void stringReplaceAllStr(char **, const char *, const char *);
void objectRelease(void **);
Array* stringSplit(char *str, const char *sep, bool);

/* Array */
Array* arrayNew(void (*)(void **));
Array* arrayNewWithAmount(int, void (*)(void **));
bool arrayAdd(Array *, void *);
bool arrayAddFirst(Array *, void *);
bool arrayRemoveAt(Array *array, int idx);
bool arrayRemove(Array *array, void *);
bool arrayRelease(Array **);
bool arraySet(Array *, void *, int);
bool arrayContainsStr(Array *, const char *);
Array* arrayStrCopy(Array *);
void* arrayGet(Array *, int);
int arrayGetIdx(Array *, void *element);

/* Date time */
typedef struct {
    int32_t *sec;
    int32_t *millisec;
} DateTime;

int msleep(long);
//FIXME remove function below
void stringSetDateTime(char **, bool);
DateTime* dateTimeCreate(DateTime *);
void dateTimeRelease(DateTime **);
void stringSetCurrentTime(char **, DateTime *, bool);
void stringSetDiffTime(char **, double, DateTime *, DateTime *);

#endif // WRAPPER_H

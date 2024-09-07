#pragma once
//windows.h time function wrapper

typedef unsigned short WORD;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

struct SYSTEMTIME getTimes(void);

const char* convertDow(unsigned short dow);
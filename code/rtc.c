#include "rtc.h"
#include "func_def.h"

static union TimeFlag g_timeFlag;
static struct TimeType g_time;

void SetTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags |= flags;
}

BOOLEAN GetTimeFlag(uint8_t flags)
{
    BOOLEAN ret;
    union TimeFlag *timeFlag = &g_timeFlag;

    if ((timeFlag->flags & flags) == 0) {
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

void ToggleTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags ^= flags;
}

void ResetTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags &= ~flags;
}

static uint8_t GetMaxDay(uint16_t year, uint8_t month)
{
    uint8_t day;
    uint8_t daysTable[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2) {
        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
            day = 29;
        } else {
            day = 28;
        }
    } else {
        day = daysTable[month];
    }

    return day;
}

struct TimeType *GetTime(void)
{
    return &g_time;
}

void TimeInit(void)
{
    struct TimeType *time = &g_time;

    time->year  = 2022;
    time->month = 1;
    time->day   = 1;
    time->hour = 8;
    time->min  = 0;
    time->sec  = 0;
    CalculateWeek();
}

BOOLEAN ClockRun(void)
{
    struct TimeType *time = &g_time;

    time->sec++;
    if (time->sec < 60) {
        return false;
    }

    time->sec = 0;
    time->min++;
    if (time->min < 60) {
        return false;
    }

    time->min = 0;
    time->hour++;
    if (time->hour < 24) {
        return false;
    }

    time->hour = 0;
    time->day++;
    if (time->day <= GetMaxDay(time->year, time->month)) {
        goto calc_week;
    }

    time->day = 1;
    time->month++;
    if (time->month < 13) {
        goto calc_week;
    }

    time->month = 1;
    time->year++;

calc_week:
    CalculateWeek();

    return true;
}

void CalculateWeek(void)
{
    int16_t yearTemp = 0;
    int16_t yearHigh;
    int16_t yearLow;
    int8_t monthTemp = 0;
    int8_t wk;
    struct TimeType *time = &g_time;

    if (time->month < 3) {
        monthTemp = time->month + 12;
        yearTemp  = time->year - 1;
    } else {
        monthTemp = time->month;
        yearTemp  = time->year;
    }

    yearHigh = yearTemp / 100;
    yearLow  = yearTemp % 100;

    wk = yearLow + (yearLow / 4) + (yearHigh / 4);
    wk = wk - (2 * yearHigh) + (26 * (monthTemp + 1) / 10) + time->day - 1;
    wk = (wk + 140) % 7;

    time->week = wk;
}

void IncMin(void)
{
    g_time.sec = 0x00;
    if (++g_time.min >= 60) {
        g_time.min = 0;
    }
}

void DecMin(void)
{
    g_time.sec = 0x00;
    if (--g_time.min == 0xff) {
        g_time.min = 59;
    }
}

void IncHour(void)
{
    if (++g_time.hour >= 24) {
        g_time.hour = 0;
    }
}

void DecHour(void)
{
    if (--g_time.hour == 0xff) {
        g_time.hour = 23;
    }
}

void IncDay(void)
{
    if (++g_time.day > GetMaxDay(g_time.year, g_time.month)) {
        g_time.day = 1;
    }
}

void DecDay(void)
{
    if (--g_time.day == 0) {
        g_time.day = GetMaxDay(g_time.year, g_time.month);
    }
}

void IncMonth(void)
{
    uint8_t MaxDay;

    if (++g_time.month >= 13) {
        g_time.month = 1;
    }

    //update Max day
    MaxDay = GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void DecMonth(void)
{
    uint8_t MaxDay;

    if (--g_time.month == 0) {
        g_time.month = 12;
    }

    //update Max day
    MaxDay = GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void IncYear(void)
{
    uint8_t MaxDay;

    if (++g_time.year > 2099) {
        g_time.year = 2000;
    }

    //update Max day
    MaxDay = GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void DecYear(void)
{
    uint8_t MaxDay;

    if (--g_time.year == 1999) {
        g_time.year = 2099;
    }

    //update Max day
    MaxDay = GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

#include "rtc.h"
#include "type_define.h"

static union TimeFlag g_timeFlag;
static struct TimeType g_time;

void RTC_ClearTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags &= ~flags;
}

void RTC_SetTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags |= flags;
}

bool RTC_GetTimeFlag(uint8_t flags)
{
    bool ret;
    union TimeFlag *timeFlag = &g_timeFlag;

    if ((timeFlag->flags & flags) == 0) {
        ret = FALSE;
    } else {
        ret = TRUE;
    }

    return ret;
}

void RTC_ToggleTimeFlag(uint8_t flags)
{
    union TimeFlag *timeFlag = &g_timeFlag;

    timeFlag->flags ^= flags;
}

static uint8_t RTC_GetMaxDay(uint16_t year, uint8_t month)
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

struct TimeType *RTC_GetTime(void)
{
    return &g_time;
}

void RTC_TimeInit(void)
{
    struct TimeType *time = &g_time;

    time->year  = 2022;
    time->month = 1;
    time->day   = 1;
    time->hour = 8;
    time->min  = 0;
    time->sec  = 0;
    RTC_CalculateWeek();
}

bool RTC_ClockRun(void)
{
    struct TimeType *time = &g_time;

    time->sec++;
    if (time->sec < 60) {
        return FALSE;
    }

    time->sec = 0;
    time->min++;
    if (time->min < 60) {
        return FALSE;
    }

    time->min = 0;
    time->hour++;
    if (time->hour < 24) {
        return FALSE;
    }

    time->hour = 0;
    time->day++;
    if (time->day <= RTC_GetMaxDay(time->year, time->month)) {
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
    RTC_CalculateWeek();

    return TRUE;
}

void RTC_CalculateWeek(void)
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

void RTC_IncMin(void)
{
    g_time.sec = 0x00;
    if (++g_time.min >= 60) {
        g_time.min = 0;
    }
}

void RTC_DecMin(void)
{
    g_time.sec = 0x00;
    if (--g_time.min == 0xff) {
        g_time.min = 59;
    }
}

void RTC_IncHour(void)
{
    if (++g_time.hour >= 24) {
        g_time.hour = 0;
    }
}

void RTC_DecHour(void)
{
    if (--g_time.hour == 0xff) {
        g_time.hour = 23;
    }
}

void RTC_IncDay(void)
{
    if (++g_time.day > RTC_GetMaxDay(g_time.year, g_time.month)) {
        g_time.day = 1;
    }
}

void RTC_DecDay(void)
{
    if (--g_time.day == 0) {
        g_time.day = RTC_GetMaxDay(g_time.year, g_time.month);
    }
}

void RTC_IncMonth(void)
{
    uint8_t MaxDay;

    if (++g_time.month >= 13) {
        g_time.month = 1;
    }

    MaxDay = RTC_GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void RTC_DecMonth(void)
{
    uint8_t MaxDay;

    if (--g_time.month == 0) {
        g_time.month = 12;
    }

    MaxDay = RTC_GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void RTC_IncYear(void)
{
    uint8_t MaxDay;

    if (++g_time.year > 2099) {
        g_time.year = 2000;
    }

    MaxDay = RTC_GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

void RTC_DecYear(void)
{
    uint8_t MaxDay;

    if (--g_time.year == 1999) {
        g_time.year = 2099;
    }

    MaxDay = RTC_GetMaxDay(g_time.year, g_time.month);
    if (g_time.day >= MaxDay) {
        g_time.day = MaxDay;
    }
}

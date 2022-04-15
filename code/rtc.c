#ifndef OS_MASTER_FILE
#define OS_GLOBALS
#include "includes.h"
#endif

rtc_counter_value_t TIME;

void TimeInit(void)
{
    TIME.year  = 2017;
    TIME.month = 7;
    TIME.day   = 8;
    Week_Deal(TIME.year, TIME.month, TIME.day);
    TIME.hour = 10;
    TIME.min  = 0;
    TIME.sec  = 0;
}

void Time_Deal(void)
{
    TIME.sec++;
    if (TIME.sec == 60) {
        flag_1min_on = 1;
        TIME.sec     = 0;
        TIME.min++;
        if (TIME.min == 60) {
            TIME.min = 0;
            TIME.hour++;
            if (TIME.hour > 23) {
                TIME.hour = 0;
                TIME.day++;
                if (TIME.day > Date_Day(TIME.year, TIME.month)) {
                    TIME.day = 1;
                    TIME.month++;
                    if (TIME.month > 12) {
                        TIME.month = 1;
                        TIME.year++;
                    }
                }
                Week_Deal(TIME.year, TIME.month, TIME.day);
            }
        }
    }
}

INT8U Date_Day(INT16U Year, INT8U Month)
{
    INT8U day_temp;
    INT8U const_days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (Month == 2) {
        if (((Year % 4 == 0) && (Year % 100 != 0)) || (Year % 400 == 0)) {
            day_temp = 29;
        } else {
            day_temp = 28;
        }
    } else {
        day_temp = const_days[Month];
    }

    return day_temp;
}

void Week_Deal(INT16U Year, INT8U Month, INT8U Day)
{
    INT16S temp_year = 0;
    INT8S temp_cen   = 0;
    INT8S temp_month = 0;
    INT8S week_data;

    if (Month < 3) {
        temp_month = Month + 12;
        temp_year  = Year - 1;
    } else {
        temp_month = Month;
        temp_year  = Year;
    }

    temp_cen  = temp_year / 100;
    temp_year = temp_year % 100;

    week_data = temp_year + temp_year / 4 + temp_cen / 4;
    week_data = week_data - 2 * temp_cen + 26 * (temp_month + 1) / 10 + Day - 1;
    TIME.week = (week_data + 140) % 7;
}

void MIN_INC(void)
{
    TIME.sec = 0x00;
    if (++TIME.min >= 60) {
        TIME.min = 0;
    }
}

void MIN_DEC(void)
{
    TIME.sec = 0x00;
    if (--TIME.min == 0xff) {
        TIME.min = 59;
    }
}

void HOUR_INC(void)
{
    if (++TIME.hour >= 24) {
        TIME.hour = 0;
    }
}

void HOUR_DEC(void)
{
    if (--TIME.hour == 0xff) {
        TIME.hour = 23;
    }
}

void DAY_INC(void)
{
    if (++TIME.day > Date_Day(TIME.year, TIME.month)) {
        TIME.day = 1;
    }
}

void DAY_DEC(void)
{
    if (--TIME.day == 0) {
        TIME.day = Date_Day(TIME.year, TIME.month);
    }
}

void MONTH_INC(void)
{
    INT8U MaxDay;

    if (++TIME.month >= 13) {
        TIME.month = 1;
    }

    //update Max day
    MaxDay = Date_Day(TIME.year, TIME.month);
    if (TIME.day >= MaxDay) {
        TIME.day = MaxDay;
    }
}

void MONTH_DEC(void)
{
    INT8U MaxDay;

    if (--TIME.month == 0) {
        TIME.month = 12;
    }

    //update Max day
    MaxDay = Date_Day(TIME.year, TIME.month);
    if (TIME.day >= MaxDay) {
        TIME.day = MaxDay;
    }
}

void YEAR_INC(void)
{
    INT8U MaxDay;

    if (++TIME.year > 2099) {
        TIME.year = 2000;
    }

    //update Max day
    MaxDay = Date_Day(TIME.year, TIME.month);
    if (TIME.day >= MaxDay) {
        TIME.day = MaxDay;
    }
}

void YEAR_DEC(void)
{
    INT8U MaxDay;

    if (--TIME.year == 1999) {
        TIME.year = 2099;
    }

    //update Max day
    MaxDay = Date_Day(TIME.year, TIME.month);
    if (TIME.day >= MaxDay) {
        TIME.day = MaxDay;
    }
}

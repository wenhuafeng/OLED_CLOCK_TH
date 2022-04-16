#ifndef RTC_H
#define RTC_H

#include "func_def.h"

struct TimeType {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t week;
    uint8_t month;
    uint16_t year;
};

#define SET_500MS_FLAG  (1 << 0)
#define SET_1000MS_FLAG (1 << 1)
#define SET_COL_FLAG    (1 << 2)
#define SET_HR_FLAG     (1 << 3)
#define SET_MD_FLAG     (1 << 4)
#define SET_PD_FLAG     (1 << 5)

union TimeFlag {
    struct {
        uint8_t flag500ms : 1;
        uint8_t flag1000ms : 1;
        uint8_t flagSetCol : 1;
        uint8_t flag24Hr12Hr : 1;
        uint8_t flagDmMd : 1;
        uint8_t flagAmPm : 1;
    } flag;
    uint8_t flags;
};

void SetTimeFlag(uint8_t flags);
BOOLEAN GetTimeFlag(uint8_t flags);
void ToggleTimeFlag(uint8_t flags);
void ResetTimeFlag(uint8_t flags);

//OS_EXT BOOLEAN F_500MS;
//OS_EXT BOOLEAN F_SET_COL;
//OS_EXT BOOLEAN flag_1000ms_on;
//OS_EXT BOOLEAN flag_1min_on;
//OS_EXT BOOLEAN Time_format; //=0,24HR. =1,12HR.
//OS_EXT BOOLEAN flag_DM_MD;  //=0,DM. =1,MD.

//OS_EXT uint8_t MinCtr;
//OS_EXT BOOLEAN F_1MIN;
//
//enum {
//    _24HR,
//    _12HR,
//};
//OS_EXT BOOLEAN F_HR; //= 1 is 12HR,= 0 is 24HR.
//
//enum {
//    _AM,
//    _PM,
//};
//OS_EXT BOOLEAN F_AM_PM;

struct TimeType *GetTime(void);
void TimeInit(void);
uint8_t TimeFormat(uint8_t hour);
BOOLEAN ClockRun(void);
uint8_t GetMaxDay(uint16_t Year, uint8_t Month);
void CalculateWeek(void);

void IncMin(void);
void DecMin(void);
void IncHour(void);
void DecHour(void);
void IncDay(void);
void DecDay(void);
void IncMonth(void);
void DecMonth(void);
void IncYear(void);
void DecYear(void);

#endif

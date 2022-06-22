#ifndef RTC_H
#define RTC_H

#include "type_define.h"

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

void RTC_ClearTimeFlag(uint8_t flags);
extern void RTC_SetTimeFlag(uint8_t flags);
extern bool RTC_GetTimeFlag(uint8_t flags);
extern void RTC_ToggleTimeFlag(uint8_t flags);

extern struct TimeType *RTC_GetTime(void);
extern void RTC_TimeInit(void);
extern bool RTC_ClockRun(void);
extern void RTC_CalculateWeek(void);

extern void RTC_IncMin(void);
extern void RTC_DecMin(void);
extern void RTC_IncHour(void);
extern void RTC_DecHour(void);
extern void RTC_IncDay(void);
extern void RTC_DecDay(void);
extern void RTC_IncMonth(void);
extern void RTC_DecMonth(void);
extern void RTC_IncYear(void);
extern void RTC_DecYear(void);

#endif

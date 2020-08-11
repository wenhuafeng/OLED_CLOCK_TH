

#ifndef _RTC_H_
#define _RTC_H_


typedef struct 
{
    INT8U      sec;
    INT8U      min;
    INT8U      hour;
    INT8U      day;
    INT8U      week;
    INT8U      month;
    INT16U       year;
}rtc_counter_value_t;

OS_EXT BOOLEAN F_500MS;
OS_EXT BOOLEAN F_500MS_1;
OS_EXT BOOLEAN F_SET_COL;
OS_EXT BOOLEAN flag_250ms_on;
OS_EXT BOOLEAN flag_500ms_on;
OS_EXT BOOLEAN flag_1000ms_on;
OS_EXT BOOLEAN flag_1min_on;
OS_EXT BOOLEAN Time_format;//=0,24HR. =1,12HR.
OS_EXT BOOLEAN flag_DM_MD; //=0,DM. =1,MD.

OS_EXT INT8U  MinCtr;
OS_EXT BOOLEAN  F_1MIN;


extern rtc_counter_value_t TIME;
extern BOOLEAN flag_500ms_on;
//extern BOOLEAN flag_500ms_on_1;
extern BOOLEAN flag_1000ms_on;
extern BOOLEAN flag_250ms_on;
extern BOOLEAN flag_1min_on;
extern BOOLEAN Time_format;
extern BOOLEAN flag_DM_MD;



//#ifdef _C24HR_TO_12HR
enum {
  _24HR,
  _12HR,
};
OS_EXT BOOLEAN  F_HR;                 //= 1 is 12HR,= 0 is 24HR.

enum {
  _AM,
  _PM,
};
OS_EXT BOOLEAN F_AM_PM;
//#endif



//==========================================================================================
void RTC_Init();
void TimeInit();
INT8U TimeFormat(INT8U hour);
void Time_Deal();
void Date_Deal();
INT8U Date_Day(INT16U Year,INT8U Month);
void Week_Deal(INT16U Year,INT8U Month,INT8U Day);

void MIN_INC (void);
void MIN_DEC (void);
void HOUR_INC (void);
void HOUR_DEC (void);
void DAY_INC (void);
void DAY_DEC (void);
void MONTH_INC (void);
void MONTH_DEC (void);
void YEAR_INC (void);
void YEAR_DEC (void);

BOOLEAN MIN_SET (BOOLEAN flag);
BOOLEAN HOUR_SET (BOOLEAN flag);
BOOLEAN DAY_SET (BOOLEAN flag);
BOOLEAN MONTH_SET (BOOLEAN flag);
BOOLEAN YEAR_SET (BOOLEAN flag);


#endif

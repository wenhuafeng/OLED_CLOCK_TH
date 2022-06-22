#include "common.h"
#include "type_define.h"
#include "key_func.h"
#include "display.h"
#include "oled_12832.h"
#include "gp_sub.h"
#include "si7021.h"
#include "rtc.h"

static uint16_t g_stopModeCtr;

static void StopModeCountDec(void)
{
    if (g_stopModeCtr) {
        g_stopModeCtr--;
    }
}

static void AllCtrRun(void)
{
    KEY_SetModeCountDec();
    DispChangeCtr();
    StopModeCountDec();
}

static void StopMode(void)
{
    if (g_stopModeCtr) {
        return;
    }

    OLED_Off();
}

void COMMON_SetDisplayTime(uint16_t time)
{
    g_stopModeCtr = time;
}

void COMMON_Init(void)
{
    DelayMs(1000);
    DelayUs(1000);
    SI7021_SampleTempHumi();
    RTC_TimeInit();
    OLED_Init();
    COMMON_SetDisplayTime(DISPLAY_10MIN_TIME);
}

void COMMON_Process(void)
{
    if (RTC_GetTimeFlag(SET_500MS_FLAG) || KEY_GetKeyFlag(KEY_FLAG)) {
        if (RTC_GetTimeFlag(SET_500MS_FLAG)) {
            RTC_ClearTimeFlag(SET_500MS_FLAG);
            KEY_IncHoldKeyCtr();
            AllCtrRun();
            StopMode();
        }

        if (KEY_GetKeyFlag(KEY_FLAG)) {
            KEY_ClearKeyFlag(KEY_FLAG);
            KEY_ScanKey();
            if (KEY_GetKeyFlag(PUSH_KEY_FLAG)) {
                KEY_PushKeyFunc();
            } else {
                KEY_ReleKeyFunc();
            }
        }
        Display();
    }

    if (RTC_GetTimeFlag(SET_1000MS_FLAG)) {
        RTC_SetTimeFlag(SET_1000MS_FLAG);
        RTC_ClockRun();
        SI7021_SampleTempHumi();
    }
}

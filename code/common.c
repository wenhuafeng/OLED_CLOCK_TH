#include "common.h"
#include "func_def.h"
#include "key_func.h"
#include "display.h"
#include "oled_12832.h"
#include "gp_sub.h"
#include "si7021.h"
#include "rtc.h"

static uint16_t g_stopModeCtr;

void SetDisplayTime(uint16_t time)
{
    g_stopModeCtr = time;
}

static void StopModeCountDec(void)
{
    if (g_stopModeCtr) {
        g_stopModeCtr--;
    }
}

static void AllCtrRun(void)
{
    SetModeCountDec();
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

void COMMON_Init(void)
{
    DelayMs(1000);
    DelayUs(1000);
    SI7021_SampleTempHumi();
    TimeInit();
    OLED_Init();
    SetDisplayTime(DISPLAY_10MIN_TIME);
}

void COMMON_Process(void)
{
    if (GetTimeFlag(SET_500MS_FLAG) || GetKeyFlag(KEY_FLAG)) {
        if (GetTimeFlag(SET_500MS_FLAG)) {
            ResetTimeFlag(SET_500MS_FLAG);
            IncHoldKeyCtr();
            AllCtrRun();
            StopMode();
        }

        if (GetKeyFlag(KEY_FLAG)) {
            ResetKeyFlag(KEY_FLAG);
            ScanKey();
            if (GetKeyFlag(PUSH_KEY_FLAG)) {
                PushKeyFunc();
            } else {
                ReleKeyFunc();
            }
        }
        Display();
    }

    if (GetTimeFlag(SET_1000MS_FLAG)) {
        ResetTimeFlag(SET_1000MS_FLAG);
        ClockRun();
        SI7021_SampleTempHumi();
    }
}

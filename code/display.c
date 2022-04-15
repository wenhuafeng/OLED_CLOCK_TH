#ifndef OS_MASTER_FILE
#define OS_GLOBALS
#include "includes.h"
#endif

BOOLEAN F_temp;
u8 DispChange;

void Display(void)
{
    if (F_temp == 0) {
        P11 = 0;
        OLED_DispClock();
        OLED_P8x16Str();
        P11 = 1;
    } else {
        P11 = 0;
        OLED_DispTempHumi();
        OLED_P8x16Str();
        P11 = 1;
    }
}

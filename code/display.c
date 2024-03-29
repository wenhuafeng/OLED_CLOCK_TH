#include "display.h"
#include "type_define.h"
#include "key_func.h"
#include "oled_12832.h"

static bool g_dispChange;

void DispChangeCtr(void)
{
    static uint8_t dispCtr = 0;

    dispCtr++;
    if (dispCtr > 4) {
        dispCtr = 0x00;
        g_dispChange = ~g_dispChange;
    }
}

void Display(void)
{
    if (KEY_GetSetModeCtr() != 0) {
        g_dispChange = 0;
    }

    if (g_dispChange == 0) {
        OLED_DispClock();
    } else {
        OLED_DispTempHumi();
    }
    OLED_P8x16Str();
}

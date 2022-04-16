#include "key_func.h"
#include "func_def.h"
#include "MC96F6432.h"
#include "gp_sub.h"
#include "rtc.h"
#include "common.h"
#include "main_sub.h"

#define KEY_PORT P0

#define ALL_KEY_MASK 0x38
#define KEY1_MASK    0x30
#define KEY2_MASK    0x28
#define KEY3_MASK    0x18

enum {
    NO_KEY,
    UP_KEY,
    DOWN_KEY,
    MODE_KEY,
    MAX_KEY,
};

enum {
    KEY0,
    KEY1 = DOWN_KEY,
    KEY2 = UP_KEY,
    KEY3 = MODE_KEY,
};

#define SET_MODE_TIME (10 * 2) // 10S
#define KEY_HOLD_TIME 2

static struct KeyType g_key;
static enum SetItemType g_setItem;
static uint8_t g_setModeCtr;
static uint8_t g_holdKeyCtr;

void IncHoldKeyCtr(void)
{
    g_holdKeyCtr++;
}

void SetKeyFlag(uint8_t keyFlag)
{
    struct KeyType *type = &g_key;

    type->keyFlag.flags |= keyFlag;
}

BOOLEAN GetKeyFlag(uint8_t flags)
{
    BOOLEAN ret;
    struct KeyType *type = &g_key;

    if ((type->keyFlag.flags & flags) == 0) {
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

void ResetKeyFlag(uint8_t flags)
{
    struct KeyType *type = &g_key;

    type->keyFlag.flags &= ~flags;
}

void SetItem(enum SetItemType item)
{
    g_setItem = item;
}

enum SetItemType GetItem(void)
{
    return g_setItem;
}

uint8_t GetSetModeCtr(void)
{
    return g_setModeCtr;
}

void SetModeCountDec(void)
{
    if (g_setModeCtr) {
        g_setModeCtr--;
        if (g_setModeCtr == 0x00) {
            SetItem(NORMAL_MODE);
        }
    }
}

void ScanKey(void)
{
    uint8_t i, j;
    struct KeyType *type = &g_key;

    type->key = NO_KEY;

    i = (KEY_PORT & ALL_KEY_MASK);
    if (i == ALL_KEY_MASK) {
        type->keyFlag.flag.pushKey = 0;
        type->keyFlag.flag.newKey  = 0;
        type->keyFlag.flag.holdKey = 0;
        type->keyFlag.flag.twoKey  = 0;
        goto normal_quit_scan_key;
    }

    DelayMs(2);

    j = (KEY_PORT & ALL_KEY_MASK);
    if ((j == ALL_KEY_MASK) || (i != j)) {
        type->keyFlag.flag.newKey  = 0;
        type->keyFlag.flag.holdKey = 0;
        type->keyFlag.flag.twoKey  = 0;
        goto normal_quit_scan_key;
    }

    DISABLE_KEY_INT();
    i = (KEY_PORT & ALL_KEY_MASK);
    switch (i) {
        case KEY1_MASK:
            type->key = KEY1;
            break;
        case KEY2_MASK:
            type->key = KEY2;
            break;
        case KEY3_MASK:
            type->key = KEY3;
            break;
        default:
            break;
    }

    if (type->key == NO_KEY) {
        type->key                  = NO_KEY;
        type->keyFlag.flag.newKey  = 0;
        type->keyFlag.flag.holdKey = 0;
        type->keyFlag.flag.twoKey  = 0;
        goto normal_quit_scan_key;
    } else {
        if (type->keyFlag.flag.pushKey == 0) {
            type->oldKey               = type->key;
            type->keyFlag.flag.newKey  = 1;
            type->keyFlag.flag.pushKey = 1;
        }
    }

normal_quit_scan_key:
    CLR_KEY_INT_FLAG();
    ENABLE_KEY_INT();
}

BOOLEAN HoldKeyCom(void)
{
    struct KeyType *type = &g_key;

    if (type->keyFlag.flag.newKey == 1) {
        g_holdKeyCtr = 0;
    } else {
        if (type->keyFlag.flag.holdKey == 0) {
            if (g_holdKeyCtr == KEY_HOLD_TIME) {
                type->keyFlag.flag.holdKey = 1;
                return 1;
            }
        }
    }

    return 0;
}

BOOLEAN SettingCom(void)
{
    struct KeyType *type = &g_key;

    if (type->keyFlag.flag.newKey == 1) {
        g_holdKeyCtr = 0;
        return 1;
    } else {
        if (type->keyFlag.flag.holdKey == 0) {
            if (g_holdKeyCtr == KEY_HOLD_TIME) {
                type->keyFlag.flag.holdKey = 1;
                return 1;
            }
        } else {
            return 1;
        }
    }

    return 0;
}

void PushKeyFunc(void)
{
    struct KeyType *type = &g_key;

    if ((type->key >= MAX_KEY) && (type->key == NO_KEY)) {
        return;
    }

    if (type->keyFlag.flag.newKey || type->keyFlag.flag.holdKey) {
        if (g_setItem != NORMAL_MODE) {
            g_setModeCtr = SET_MODE_TIME;
        }
    }

    switch (type->key) {
        case UP_KEY:
            if (g_setItem != NORMAL_MODE) {
                if (SettingCom() == 1) {
                    if (type->keyFlag.flag.newKey == 1) {
                        switch (g_setItem) {
                            case CLOCK_SET_HR:
                                ToggleTimeFlag(SET_HR_FLAG);
                                break;
                            default:
                                break;
                        }
                    }

                    switch (g_setItem) {
                        case CLOCK_SET_HOUR:
                            IncHour();
                            break;
                        case CLOCK_SET_MIN:
                            IncMin();
                            break;
                        case CLOCK_SET_YEAR:
                            IncYear();
                            break;
                        case CLOCK_SET_MONTH:
                            IncMonth();
                            break;
                        case CLOCK_SET_DAY:
                            IncDay();
                            break;
                        default:
                            break;
                    }
                    SetTimeFlag(SET_COL_FLAG);
                    CalculateWeek();
                }
            }
            break;
        case DOWN_KEY:
            if (g_setItem != NORMAL_MODE) {
                if (SettingCom() == 1) {
                    if (type->keyFlag.flag.newKey == 1) {
                        switch (g_setItem) {
                            case CLOCK_SET_HR:
                                ToggleTimeFlag(SET_HR_FLAG);
                                break;
                            default:
                                break;
                        }
                    }

                    switch (g_setItem) {
                        case CLOCK_SET_HOUR:
                            DecHour();
                            break;
                        case CLOCK_SET_MIN:
                            DecMin();
                            break;
                        case CLOCK_SET_YEAR:
                            DecYear();
                            break;
                        case CLOCK_SET_MONTH:
                            DecMonth();
                            break;
                        case CLOCK_SET_DAY:
                            DecDay();
                            break;
                        default:
                            break;
                    }
                    SetTimeFlag(SET_COL_FLAG);
                    CalculateWeek();
                }
            }
            break;
        case MODE_KEY:
            if (HoldKeyCom()) {
            }
            break;
        default:
            break;
    }

    type->keyFlag.flag.newKey = 0;
    SetDisplayTime(DISPLAY_10MIN_TIME);
}

void ReleKeyFunc(void)
{
    struct KeyType *type = &g_key;
    uint8_t key          = type->oldKey;

    type->oldKey = 0x00;
    if ((key >= MAX_KEY) && (key == NO_KEY)) {
        return;
    }

    switch (key) {
        case UP_KEY:
            break;
        case DOWN_KEY:
            break;
        case MODE_KEY:
            //set clock.
            if (g_setItem != NORMAL_MODE) {
                if ((g_setItem & CLOCK_SET) != CLOCK_SET) {
                    g_setItem = NORMAL_MODE;
                } else {
                    g_setModeCtr = SET_MODE_TIME;
                    g_setItem++;
                    if (g_setItem > CLOCK_SET_DAY) {
                        g_setItem    = NORMAL_MODE;
                        g_setModeCtr = 0x00;
                    }
                }
            } else {
                g_setItem    = CLOCK_SET_HOUR;
                g_setModeCtr = SET_MODE_TIME;
                Timer1_init();
            }
            break;
        default:
            break;
    }

    SetDisplayTime(DISPLAY_10MIN_TIME);
}

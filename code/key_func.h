#ifndef KEYFUNC_H
#define KEYFUNC_H

#include "func_def.h"

#define DISABLE_KEY_INT() \
    do {                  \
        IE &= ~0x20;      \
    } while (0)
#define ENABLE_KEY_INT() \
    do {                 \
        IE |= 0x20;      \
    } while (0)
#define CLR_KEY_INT_FLAG() \
    do {                   \
        EIFLAG0 = 0x00;    \
    } while (0)

#define KEY_FLAG      (1 << 0)
#define PUSH_KEY_FLAG (1 << 1)
#define NEW_KEY_FLAG  (1 << 2)
#define HOLD_KEY_FLAG (1 << 3)
#define TWO_KEY_FLAG  (1 << 4)

union KeyFlagType {
    struct {
        uint8_t key : 1;
        uint8_t pushKey : 1;
        uint8_t newKey : 1;
        uint8_t holdKey : 1;
        uint8_t twoKey : 1;
    } flag;
    uint8_t flags;
};

struct KeyType {
    union KeyFlagType keyFlag;
    uint8_t key;
    uint8_t oldKey;
};

enum SetItemType {
    NORMAL_MODE,
    CLOCK_SET       = 0x10,
    CLOCK_SET_HR    = 0x11,
    CLOCK_SET_HOUR  = 0x12,
    CLOCK_SET_MIN   = 0x13,
    CLOCK_SET_YEAR  = 0x14,
    CLOCK_SET_MONTH = 0x15,
    CLOCK_SET_DAY   = 0x16,
};

void IncHoldKeyCtr(void);

void SetKeyFlag(uint8_t keyFlag);
BOOLEAN GetKeyFlag(uint8_t flags);
void ResetKeyFlag(uint8_t flags);

void SetItem(enum SetItemType item);
enum SetItemType GetItem(void);
void SetModeCountDec(void);
uint8_t GetSetModeCtr(void);

void ScanKey(void);
BOOLEAN HoldKeyCom(void);
BOOLEAN SettingCom(void);

void PushKeyFunc(void);
void ReleKeyFunc(void);

#endif

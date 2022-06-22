#ifndef KEY_FUNC_H
#define KEY_FUNC_H

#include "type_define.h"

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
    CLOCK_SET,
    CLOCK_SET_HR,
    CLOCK_SET_HOUR,
    CLOCK_SET_MIN,
    CLOCK_SET_YEAR,
    CLOCK_SET_MONTH,
    CLOCK_SET_DAY,
};

extern void KEY_IncHoldKeyCtr(void);

extern void KEY_ClearKeyFlag(uint8_t flags);
extern bool KEY_GetKeyFlag(uint8_t flags);
extern void KEY_SetKeyFlag(uint8_t flags);

extern enum SetItemType KEY_GetItem(void);
extern void KEY_SetModeCountDec(void);
extern uint8_t KEY_GetSetModeCtr(void);

extern void KEY_ScanKey(void);
extern void KEY_PushKeyFunc(void);
extern void KEY_ReleKeyFunc(void);

#endif

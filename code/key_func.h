#ifndef KEYFUNC_H
#define KEYFUNC_H

#define KeyPort P0

enum {
    _NO_KEY_,
    _UP_KEY_,
    _DOWN_KEY_,
    _MODE_KEY_,
    _MAX_KEY_,
};

#define _ALL_KEY_MASK_ 0x38
#define _01_KEY_MASK_  0x30 //KO2 = 1
#define _02_KEY_MASK_  0x28
#define _03_KEY_MASK_  0x18

enum {
    _00_KEY_,
    _01_KEY_ = _DOWN_KEY_,
    _02_KEY_ = _UP_KEY_,
    _03_KEY_ = _MODE_KEY_,
};

OS_EXT BOOLEAN F_Key;
OS_EXT BOOLEAN F_PushKey;
OS_EXT BOOLEAN F_NewKey;
OS_EXT BOOLEAN F_HoldKey;
OS_EXT BOOLEAN F_TwoKey;
OS_EXT INT8U Key;
OS_EXT INT8U OldKey;

//Key
#define _HOLD_TIMER_KEY_ 2
OS_EXT INT8U HoldKeyCtr;

#define Dis_Key_Int() \
    do {              \
        IE &= ~0x20;  \
    } while (0)
#define En_Key_Int() \
    do {             \
        IE |= 0x20;  \
    } while (0)
#define Clr_Key_Int_Flag() \
    do {                   \
        EIFLAG0 = 0x00;    \
    } while (0)

void ScanKey(void);
BOOLEAN HoldKeyCom(void);
BOOLEAN SettingCom(void);

void PushKeyFunc(void);
void ReleKeyFunc(void);

#endif

#ifndef OS_MASTER_FILE
#define OS_GLOBALS
#include "includes.h"
#endif

void ScanKey(void)
{
    INT8U i, j;

    Key = _NO_KEY_;

    i = (KeyPort & _ALL_KEY_MASK_);
    if (i == _ALL_KEY_MASK_) {
        F_PushKey = 0;
        F_NewKey  = 0;
        F_HoldKey = 0;
        F_TwoKey  = 0;
        //TEST
        //P10 = 0;
        goto normal_quit_scan_key;
    }

    DelayMs(2);

    j = (KeyPort & _ALL_KEY_MASK_);
    if ((j == _ALL_KEY_MASK_) || (i != j)) {
        //F_PushKey = 0;
        F_NewKey  = 0;
        F_HoldKey = 0;
        F_TwoKey  = 0;
        goto normal_quit_scan_key;
    }

    Dis_Key_Int();
    i = (KeyPort & _ALL_KEY_MASK_);
    switch (i) {
        case _01_KEY_MASK_:
            Key = _01_KEY_;
            break;
        case _02_KEY_MASK_:
            Key = _02_KEY_;
            break;
        case _03_KEY_MASK_:
            Key = _03_KEY_;
            break;
        default:
            break;
    }

    if (Key == _NO_KEY_) {
        Key = _NO_KEY_;
        //F_PushKey = 0;
        F_NewKey  = 0;
        F_HoldKey = 0;
        F_TwoKey  = 0;
        goto normal_quit_scan_key;
    } else {
        if (F_PushKey == 0) {
            OldKey    = Key;
            F_NewKey  = 1;
            F_PushKey = 1;
            //TEST
            //P10 = 1;
        }
    }

normal_quit_scan_key:
    _nop_();
    Clr_Key_Int_Flag();
    En_Key_Int();
}

BOOLEAN HoldKeyCom(void)
{
    if (F_NewKey == 1) {
        HoldKeyCtr = 0;
    } else {
        if (F_HoldKey == 0) {
            if (HoldKeyCtr == _HOLD_TIMER_KEY_) {
                F_HoldKey = 1;
                return 1;
            }
        }
    }

    return 0;
}

BOOLEAN SettingCom(void)
{
    if (F_NewKey == 1) {
        HoldKeyCtr = 0;
        //KEY_TONE();
        return 1;
    } else {
        if (F_HoldKey == 0) {
            if (HoldKeyCtr == _HOLD_TIMER_KEY_) {
                F_HoldKey = 1;
                //Timer1_init();
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
    if (F_NewKey || F_HoldKey) {
        if (SetItem != _NORMAL_MODE_) {
            SetModeCtr = _SET_MODE_TIME_;
        }
    }

    switch (Key) {
        case _UP_KEY_:
            if (SetItem != _NORMAL_MODE_) {
                if (SettingCom() == 1) {
                    if (F_NewKey == 1) {
                        switch (SetItem) {
                            case _CLOCK_SET_HR_:
                                F_HR = ~F_HR;
                                break;
                            default:
                                break;
                        }
                    }

                    switch (SetItem) {
                        case _CLOCK_SET_HOUR_:
                            HOUR_INC();
                            break;
                        case _CLOCK_SET_MIN_:
                            MIN_INC();
                            break;
                        case _CLOCK_SET_YEAR_:
                            YEAR_INC();
                            break;
                        case _CLOCK_SET_MONTH_:
                            MONTH_INC();
                            break;
                        case _CLOCK_SET_DAY_:
                            DAY_INC();
                            break;
                        default:
                            break;
                    }
                    F_SET_COL = 1;
                    Week_Deal(TIME.year, TIME.month, TIME.day);
                }
            }
            break;
        case _DOWN_KEY_:
            if (SetItem != _NORMAL_MODE_) {
                if (SettingCom() == 1) {
                    if (F_NewKey == 1) {
                        switch (SetItem) {
                            case _CLOCK_SET_HR_:
                                F_HR = ~F_HR;
                                break;
                            default:
                                break;
                        }
                    }

                    switch (SetItem) {
                        case _CLOCK_SET_HOUR_:
                            HOUR_DEC();
                            break;
                        case _CLOCK_SET_MIN_:
                            MIN_DEC();
                            break;
                        case _CLOCK_SET_YEAR_:
                            YEAR_DEC();
                            break;
                        case _CLOCK_SET_MONTH_:
                            MONTH_DEC();
                            break;
                        case _CLOCK_SET_DAY_:
                            DAY_DEC();
                            break;
                        default:
                            break;
                    }
                    F_SET_COL = 1;
                    Week_Deal(TIME.year, TIME.month, TIME.day);
                }
            }
            break;
        case _MODE_KEY_:
            if (HoldKeyCom()) {
            }
            break;
        default:
            break;
    }

    F_NewKey = 0;
}

void ReleKeyFunc(void)
{
    INT8U tmp;

    tmp    = OldKey;
    OldKey = 0x00;

    switch (tmp) {
        case _UP_KEY_:
            break;
        case _DOWN_KEY_:
            break;
        case _MODE_KEY_:
            //set clock.
            if (SetItem != _NORMAL_MODE_) {
                if ((SetItem & _CLOCK_SET_) != _CLOCK_SET_) {
                    SetItem = _NORMAL_MODE_;
                } else {
                    SetModeCtr = _SET_MODE_TIME_;
                    SetItem++;
                    if (SetItem > _CLOCK_SET_DAY_) {
                        SetItem    = _NORMAL_MODE_;
                        SetModeCtr = 0x00;
                    }
                }
            } else {
                SetItem    = _CLOCK_SET_HOUR_;
                SetModeCtr = _SET_MODE_TIME_;
                Timer1_init();
            }
            break;
        default:
            break;
    }
}

#include "gp_sub.h"
#include "type_define.h"

#if defined(HEX_TO_DEC) && HEX_TO_DEC
static uint8_t g_dec[4];
#endif

#if defined(HEX_TO_ASC) && HEX_TO_ASC
uint8_t HexToAsc(uint8_t hex)
{
    if ((hex >= 0) && (hex <= 9)) {
        hex += 0x30;
    } else if ((hex >= 10) && (hex <= 15)) {
        hex += 0x37;
    } else {
        hex = 0xff;
    }

    return hex;
}
#endif

#if defined(ASC_TO_HEX) && ASC_TO_HEX
uint8_t AscToHex(uint8_t asc)
{
    if ((aChar >= 0x30) && (aChar <= 0x39))
        aChar -= 0x30;
    else if ((aChar >= 0x41) && (aChar <= 0x46))
        aChar -= 0x37;
    else if ((aChar >= 0x61) && (aChar <= 0x66))
        aChar -= 0x57;
    else
        aChar = 0xff;

    return aChar;
}
#endif

#if defined(DELAY_US) && DELAY_US
void DelayUs(uint16_t count)
{
    while (count--) ;
}
#endif

#if defined(DELAY_MS) && DELAY_MS
void DelayMs(uint16_t count) // ��ʾ��������
{
    /*
    // 8M
    uint8_t i,j;
    for(; count > 0; count--) {
        for(j = 21; j > 0; j--) {
            for(i = 90; i > 0; i--) {
                ;
            }
        }
    }
    */

    // 16M
    uint8_t i, j;
    for (; count > 0; count--) {
        for (j = 20; j > 0; j--) {
            for (i = 199; i > 0; i--) {
                ;
            }
        }
    }
}
#endif

#if defined(C24HR_TO_12HR) && C24HR_TO_12HR
uint8_t Hour24hrTo12hr(uint8_t hr)
{
    uint8_t val;

    if (hr <= 11) {
        F_AM_PM = _AM;
        if (hr == 0) {
            val = 12;
        } else {
            val = hr;
        }
    } else {
        F_AM_PM = _PM;
        if (hr == 12) {
            val = hr;
        } else {
            val = hr - 12;
        }
    }

    return val;
}
#endif

#if defined(C2F) && C2F
int16_t CelsiusToFahrenheit(int16_t c)
{
    return (c * 18 + 320);
}
#endif

#if defined(F2C) && F2C
int16_t F2C(int16_t temp)
{
    return ((temp - 320) / 18);
}
#endif

/********************************************************************/ /**
 * @brief:      Calculate weekday Function subprogram
 *              Weekday={(Year+12)+Day+(Year+12)/4+T[Mtab]-1}Mod7
 *              If Month<3 The Year Sub 1
 *
 * @param[in]:  Year,Month Offset(Mtab),Day
 *              1990<=Year<=2099, 1<=Month<=12, 1<=Day<=31
 *
 * @return:     WEEK(0=Sunday,1=Monday,...,6=Saturday)
 *********************************************************************/
#if defined(CALC_WEEK) && CALC_WEEK
void CalcWeek(void)
{
    uint16_t tmp1, tmp2, tmp3;
    uint8_t weekTable[13] = { 0x00, 0x00, 0x03, 0x02, 0x05, 0x00, 0x03, 0x05, 0x01, 0x04, 0x06, 0x02, 0x04 };

    tmp1 = DecToHex(D_YEAR);
    tmp2 = DecToHex(D_MONTH);
    tmp3 = DecToHex(D_DAY);

    D_WEEK = (tmp1 + tmp3 + (tmp1 / 4) + weekTable[tmp2] - 1) % 7;
}

#endif

#if defined(HPA_TO_INHG) && HPA_TO_INHG
uint16_t HpaToInhg(uint16_t hpa)
{
    return (hpa * 2953 / 1000);
}
#endif

#if defined(HEX_TO_DEC) && HEX_TO_DEC
void HexToDec(uint16_t hex)
{
    uint16_t i;

    i        = hex;
    g_dec[0] = 0;
    g_dec[1] = 0;
    g_dec[2] = 0;
    g_dec[3] = 0;

    while (i >= 1000) {
        i = i - 1000;
        g_dec[3]++;
    }

    while (i >= 100) {
        i = i - 100;
        g_dec[2]++;
    }

    while (i >= 10) {
        i = i - 10;
        g_dec[1]++;
    }

    g_dec[0] = i;
}
#endif

#if defined(DEC_TO_HEX) && DEC_TO_HEX
uint8_t DecToHex(uint8_t dec)
{
    uint8_t i;

    i = dec & 0xf0;
    i = i >> 4;

    dec = dec & 0x0f;

    while (i != 0) {
        i--;
        dec = dec + 0x0a;
    }

    return dec;
}
#endif

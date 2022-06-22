#ifndef GP_SUB_H
#define GP_SUB_H

#include "type_define.h"

#define HEX_TO_ASC    1
#define ASC_TO_HEX    0
#define DELAY_US      1
#define DELAY_MS      1
#define C24HR_TO_12HR 0
#define C2F           0
#define F2C           0
#define CALC_WEEK     0
#define HPA_TO_INHG   0
#define HEX_TO_DEC    0
#define DEC_TO_HEX    0

extern uint8_t HexToAsc(uint8_t hex);
extern uint8_t AscToHex(uint8_t asc);
extern void DelayUs(uint16_t count);
extern void DelayMs(uint16_t count);
extern uint8_t Hour24hrTo12hr(uint8_t hr);
extern int16_t CelsiusToFahrenheit(int16_t c);
extern void CalcWeek(void);
extern uint16_t HpaToInhg(uint16_t hpa);
extern void HexToDec(uint16_t hex);
extern uint8_t DecToHex(uint8_t dec);

#endif

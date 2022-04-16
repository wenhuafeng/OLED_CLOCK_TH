#ifndef GP_SUB_H
#define GP_SUB_H

#include "func_def.h"

#define HEX_TO_ASC 1
#define ASC_TO_HEX 0
#define DELAY_US 1
#define DELAY_MS 1
#define C24HR_TO_12HR 0
#define C2F 0
#define F2C 0
#define CALC_WEEK 0
#define HPA_TO_INHG 0
#define HEX_TO_DEC 0
#define DEC_TO_HEX 0

uint8_t HexToAsc(uint8_t hex);
uint8_t AscToHex(uint8_t asc);
void DelayUs(uint16_t count);
void DelayMs(uint16_t count);
uint8_t Hour24hrTo12hr(uint8_t hr);
int16_t CelsiusToFahrenheit(int16_t c);
void CalcWeek(void);
uint16_t HpaToInhg(uint16_t hpa);
void HexToDec(uint16_t hex);
uint8_t DecToHex(uint8_t dec);

#endif

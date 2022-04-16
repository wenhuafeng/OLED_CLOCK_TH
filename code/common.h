#ifndef COMMON_H
#define COMMON_H

#include "func_def.h"

#define DISPLAY_10MIN_TIME (10 * 60 * 2) // 10MIN

void SetDisplayTime(uint16_t time);
void COMMON_Init(void);
void COMMON_Process(void);

#endif

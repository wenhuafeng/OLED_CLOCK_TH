#ifndef COMMON_H
#define COMMON_H

#include "type_define.h"

#define DISPLAY_10MIN_TIME (10 * 60 * 2) // 10MIN

void COMMON_SetDisplayTime(uint16_t time);
void COMMON_Init(void);
void COMMON_Process(void);

#endif

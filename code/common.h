#ifndef COMMON_H
#define COMMON_H

#include "type_define.h"

#define DISPLAY_10MIN_TIME (10 * 60 * 2) // 10MIN

extern void COMMON_SetDisplayTime(uint16_t time);
extern void COMMON_Init(void);
extern void COMMON_Process(void);

#endif

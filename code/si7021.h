#ifndef SI7021_H
#define SI7021_H

#include "func_def.h"

void SI7021_SampleTempHumi(void);
int16_t SI7021_GetTemp(void);
uint16_t SI7021_GetHumi(void);

#endif

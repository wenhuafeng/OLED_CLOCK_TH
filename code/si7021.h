#ifndef SI7021_H
#define SI7021_H

#include "type_define.h"

extern void SI7021_SampleTempHumi(void);
extern int16_t SI7021_GetTemp(void);
extern uint16_t SI7021_GetHumi(void);

#endif

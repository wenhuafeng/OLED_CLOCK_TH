

#ifndef  OS_MASTER_FILE
#define  OS_GLOBALS
#include "includes.h"
#endif

BOOLEAN F_temp;
u8 DispChange;

void Display(void)
{
  if (F_temp == 0) {
    P11 = 0;
    OledDispClock();
    LCD_P8x16Str();
    P11 = 1;
  } else {
    P11 = 0;
    OledDispTH();
    LCD_P8x16Str();
    P11 = 1;
  }
}





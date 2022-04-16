//======================================================
// Main program routine
// Device name  : MC96F6332
// Package type : 32QFP
//======================================================

#include "MC96F6432.h"
#include "main_sub.h"
#include "common.h"

void main(void)
{
    cli();        // disable INT. during peripheral setting
    port_init();  // initialize ports
    clock_init(); // initialize operation clock
    LCD_init();   // initialize LCD
    //Timer1_init();
    WT_init(); // initialize Watch timer
    ExINT_init();
    sei(); // enable INT.

    // TODO: add your main code here
    COMMON_Init();

    do {
        COMMON_Process();
    } while (1);
}

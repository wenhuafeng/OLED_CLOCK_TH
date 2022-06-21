//======================================================
// Main program routine
// - Device name  : MC96F6332
// - Package type : 32QFP
//======================================================
// For XDATA variable : V1.041.00 ~
#define		MAIN	1

// Generated    : Sat, Jul 08, 2017 (16:55:58)
#include	"MC96F6432.h"
#include	"type_define.h"

void main()
{
	cli();          	// disable INT. during peripheral setting
	port_init();    	// initialize ports
	clock_init();   	// initialize operation clock
	ExINT_init();   	// initialize external interrupt
	LCD_init();     	// initialize LCD
	Timer1_init();  	// initialize Timer1
	WT_init();      	// initialize Watch timer
	sei();          	// enable INT.

	// TODO: add your main code here

	while(1);
}

//======================================================
// interrupt routines
//======================================================

void INT_Ext0_7() interrupt 5
{
	// External interrupt 0 ~ 7
	// TODO: add your code here
}

void INT_Timer1() interrupt 14
{
	// Timer1 interrupt
	// TODO: add your code here
}

void INT_WT() interrupt 20
{
	// Watch timer interrupt
	// TODO: add your code here
}

//======================================================
// peripheral setting routines
//======================================================

void ExINT_init()
{
	// initialize external interrupt
	EIPOL0H = 0x00; 	// edge : external INT.7~4
	EIPOL0L = 0xFC; 	// edge : external INT.3~0
	EIPOL1 = 0x55;  	// edge : external INT.12, 11, 10, 8
	IE |= 0x20;     	// Enable Ext.INT.0~7
}

void LCD_init()
{
	// initialize LCD
	// Frame freq. = 16.000000Hz
	LCDCRH = 0x00;  	// LCD control High
	LCDCRL = 0x00;  	// LCD control Low
	LCDCCR = 0x00;  	// LCD contrast
}

void Timer1_init()
{
	// initialize Timer1
	// 16bit timer, period = 125.000000mS
	T1CRH = 0x00;   	// timer setting High
	T1CRL = 0x40;   	// timer setting Low
	T1ADRH = 0x7A;  	// period count High
	T1ADRL = 0x11;  	// period count Low
	IE2 |= 0x04;    	// Enable Timer1 interrupt
	T1CRH |= 0x80;  	// enable counter
}

void WT_init()
{
	// initialize Watch timer
	WTCR = 0x88;    	// setting
	WTDR = 0x01;    	// set duty
	WTDR = 0x81;    	// clear WT
	IE3 |= 0x04;    	// Enable WT interrupt
}

void clock_init()
{
	// internal RC clock (16.000000MHz)
	OSCCR = 0x28;   	// Set Int. OSC
	SCCR = 0x00;    	// Use Int. OSC
}

void port_init()
{
	// initialize ports
	//   1 : SXIN     out
	//   2 : SXOUT    out
	//   3 : RESETB   in  key reset
	//   4 : P40      out
	//   5 : P41      out
	//   6 : P42      out
	//   7 : P33      out
	//   8 : P32      out
	//   9 : P31      out
	//  10 : P30      out
	//  11 : P27      out
	//  12 : P26      out
	//  13 : P22      out
	//  14 : P21      out
	//  15 : P20      out
	//  16 : P10      out
	//  17 : P11      out
	//  18 : P12      out
	//  19 : P13      out
	//  20 : P07      out
	//  21 : P06      out
	//  22 : EINT3    in
	//  23 : EINT2    in
	//  24 : EINT1    in
	//  25 : P02      out
	//  26 : P01      out
	//  27 : P00      out
	//  30 : P50      out
	//  31 : P51      out
	//  32 : P52      out
	P0IO = 0xC7;    	// direction
	P0PU = 0x38;    	// pullup
	P0OD = 0x00;    	// open drain
	P0DB = 0x00;    	// bit7~6(debounce clock), bit5~0=P07~02 debounce
	P0   = 0x00;    	// port initial value

	P1IO = 0xFF;    	// direction
	P1PU = 0x00;    	// pullup
	P1OD = 0x00;    	// open drain
	P15DB = 0x00;   	// debounce : P54, 52, 17, 16, 12, 11
	P1   = 0x00;    	// port initial value

	P2IO = 0xFF;    	// direction
	P2PU = 0x00;    	// pullup
	P2OD = 0x00;    	// open drain
	P2   = 0x00;    	// port initial value

	P3IO = 0xFF;    	// direction
	P3PU = 0x00;    	// pullup
	P3   = 0x00;    	// port initial value

	P4IO = 0xFF;    	// direction
	P4PU = 0x00;    	// pullup
	P4OD = 0x00;    	// open drain
	P4   = 0x00;    	// port initial value

	P5IO = 0xDF;    	// direction
	P5PU = 0x20;    	// pullup
	P5   = 0x00;    	// port initial value

	// Set port functions
	P0FSRH = 0x00;  	// P0 selection High
	P0FSRL = 0x00;  	// P0 selection Low
	P1FSRH = 0x00;  	// P1 selection High
	P1FSRL = 0x00;  	// P1 selection Low
	P2FSRH = 0x00;  	// P2 selection High
	P2FSRL = 0x00;  	// P2 selection Low
	P3FSR = 0x00;   	// P3 selection
	P4FSR = 0x00;   	// P4 selection
	P5FSR = 0x28;   	// P5 selection
}


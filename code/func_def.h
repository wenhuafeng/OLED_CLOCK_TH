
#ifndef _FUNC_DEF_
#define _FUNC_DEF_


#ifdef  OS_GLOBALS
#define  OS_EXT  extern
#else
#define  OS_EXT
#endif


//********************************************************************************
typedef bit          BOOLEAN;  
typedef unsigned char    INT8U, u8, U8, uint8, UINT8, BYTE;      /* Unsigned  8 bit quantity        */
typedef signed   char    INT8S, s8, S8, int8, INT8;          /* Signed    8 bit quantity        */
typedef unsigned int      INT16U, u16, U16, uint16, UINT16, WORD;   /* Unsigned 16 bit quantity        */
typedef signed   int      INT16S, s16, S16, int16, INT16;       /* Signed   16 bit quantity        */
typedef unsigned long    INT32U, u32, U32, uint32, UINT32, DWORD;   /* Unsigned 32 bit quantity        */
typedef signed   long    INT32S, s32, S32, int32, INT32;       /* Signed   32 bit quantity        */
typedef float        FP32;                     /* Single precision floating point */
//********************************************************************************


#define  _SET_MODE_TIME_        10*2    //10 sec quit Setting mode
enum{
  _NORMAL_MODE_,
  _CLOCK_SET_       = 0x10,
  _CLOCK_SET_HR_     = 0x11,
    _CLOCK_SET_HOUR_   = 0x12,
    _CLOCK_SET_MIN_     = 0x13,
    _CLOCK_SET_YEAR_  = 0x14,
    _CLOCK_SET_MONTH_  = 0x15,
    _CLOCK_SET_DAY_    = 0x16,
};
OS_EXT INT8U  SetItem;
OS_EXT INT8U  SetModeCtr;



//======================================================
// Function and global variables definition
//======================================================

void port_init();               // initialize ports
void clock_init();              // initialize operation clock
void LCD_init();                // initialize LCD
void WT_init();                 // initialize Watch timer
void Timer1_init();
void ExINT_init();

#endif

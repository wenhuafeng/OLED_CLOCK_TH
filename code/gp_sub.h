#ifndef GP_SUB_H
#define GP_SUB_H

#define _HEX_TO_ASC_
//#define _ASC_TO_HEX_
#define _DELAY_US
#define _DELAY_MS
//#define _C24HR_TO_12HR
//#define _C2F
//#define _F2C
//#define _CALC_WEEK
//#define _HPA2INHG
//#define _HEX2DEC
//#define _DEC2HEX

#ifdef _HEX2DEC
extern INT8U DEC[4]; //dec2hex use.
#endif

#ifdef _DEC2HEX
extern INT8U HEX; //hex2dec use.
#endif

INT8U HexToAsc(INT8U aHex);
INT8U AscToHex(INT8U aChar);

void KEY_TONE(void);
void DelayUs(INT16U count);
void DelayMs(INT16U t);
INT8U C24HR_TO_12HR(INT8U hr);
INT16S C2F(INT16S temp);
//void CALC_WEEK        (void);
INT16U HPA2INHG(INT16U i);
void HEX2DEC(INT16U Hp);
INT8U DEC2HEX(INT8U HEX_1);

#endif

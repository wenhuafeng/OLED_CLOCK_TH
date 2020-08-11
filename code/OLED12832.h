

#ifndef _LQOLED_H
#define _LQOLED_H

 
//#define byte  unsigned char
//#define word  unsigned int
//#define dword unsigned long 

 
//void OLED_Init(void);
//void OLED_CLS(void);
//void OLED_Fill(byte dat);
//void LCD_P6x8Str(byte x,byte y,byte ch[]);
//void LCD_P8x16Str(byte x,byte y,byte ch[]);
//void LCD_P14x16Ch(byte x,byte y,byte N);  
//void LCD_P16x16Ch(unsigned char x,unsigned char y,unsigned char c[2]);
//void OLED_PutString(unsigned char x, unsigned char y, unsigned char *s);    

//void QF_LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
//void QF_LCD_PutString(unsigned char x, unsigned char y, unsigned char *s);
//void QF_LCD_P16x16Ch(unsigned char x,unsigned char y,unsigned char c[2]);


void OLED_Clear(void);
void OLED_OFF(void);
void OLED_Init(void);

void LCD_P8x16Str(void);
void OledDispClock(void);
void OledDispTH(void);



#endif


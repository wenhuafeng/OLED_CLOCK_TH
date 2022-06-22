#include "oled_12832.h"
#include "type_define.h"
#include "MC96F6432.h"
#include "gp_sub.h"
#include "key_func.h"
#include "rtc.h"
#include "si7021.h"

//
// IC = SSD1306
// IIC
// 14PIN OLED
// 128*32
//

#define LCD_SCL   P06
#define LCD_SDA   P07
#define LCD_RESET P13

#define SDA_PORT   P0
#define SDA_NUMBER (1 << 7)
#define SDA_IN()              \
    do {                     \
        P0IO &= ~SDA_NUMBER; \
    } while (0)
#define SDA_OUT()            \
    do {                    \
        P0IO |= SDA_NUMBER; \
    } while (0)

#define SI7201_ADDR 0x78
#define BRIGHTNESS  250
#define DELAY_TIME  250

static uint8_t g_letterTable[32];

static code uint8_t g_weekTable[8][3] = {
    'S', 'u', 'n',
    'M', 'o', 'n',
    'T', 'u', 'e',
    'W', 'e', 'd',
    'T', 'h', 'u',
    'F', 'r', 'i',
    'S', 't', 'a',
};

static code uint8_t g_table8X16[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*" ",0*/
    0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00, 0x00, 0x00, /*"!",1*/
    0x00, 0x10, 0x0C, 0x06, 0x10, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*""",2*/
    0x40, 0xC0, 0x78, 0x40, 0xC0, 0x78, 0x40, 0x00, 0x04, 0x3F, 0x04, 0x04, 0x3F, 0x04, 0x04, 0x00, /*"#",3*/
    0x00, 0x70, 0x88, 0xFC, 0x08, 0x30, 0x00, 0x00, 0x00, 0x18, 0x20, 0xFF, 0x21, 0x1E, 0x00, 0x00, /*"$",4*/
    0xF0, 0x08, 0xF0, 0x00, 0xE0, 0x18, 0x00, 0x00, 0x00, 0x21, 0x1C, 0x03, 0x1E, 0x21, 0x1E, 0x00, /*"%",5*/
    0x00, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00, 0x00, 0x1E, 0x21, 0x23, 0x24, 0x19, 0x27, 0x21, 0x10, /*"&",6*/
    0x10, 0x16, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"'",7*/
    0x00, 0x00, 0x00, 0xE0, 0x18, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, /*"(",8*/
    0x00, 0x02, 0x04, 0x18, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, /*")",9*/
    0x40, 0x40, 0x80, 0xF0, 0x80, 0x40, 0x40, 0x00, 0x02, 0x02, 0x01, 0x0F, 0x01, 0x02, 0x02, 0x00, /*"*",10*/
    0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x01, 0x00, /*"+",11*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, /*",",12*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, /*"-",13*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, /*".",14*/
    0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 0x04, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, /*"/",15*/
    0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x10, 0x0F, 0x00, /*"0",16*/
    0x00, 0x10, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, /*"1",17*/
    0x00, 0x70, 0x08, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x30, 0x00, /*"2",18*/
    0x00, 0x30, 0x08, 0x88, 0x88, 0x48, 0x30, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"3",19*/
    0x00, 0x00, 0xC0, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x07, 0x04, 0x24, 0x24, 0x3F, 0x24, 0x00, /*"4",20*/
    0x00, 0xF8, 0x08, 0x88, 0x88, 0x08, 0x08, 0x00, 0x00, 0x19, 0x21, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"5",21*/
    0x00, 0xE0, 0x10, 0x88, 0x88, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"6",22*/
    0x00, 0x38, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, /*"7",23*/
    0x00, 0x70, 0x88, 0x08, 0x08, 0x88, 0x70, 0x00, 0x00, 0x1C, 0x22, 0x21, 0x21, 0x22, 0x1C, 0x00, /*"8",24*/
    0x00, 0xE0, 0x10, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x31, 0x22, 0x22, 0x11, 0x0F, 0x00, /*"9",25*/
    0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, /*":",26*/
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, /*";",27*/
    0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, /*"<",28*/
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, /*"=",29*/
    0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, /*">",30*/
    0x00, 0x70, 0x48, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x36, 0x01, 0x00, 0x00, /*"?",31*/
    0xC0, 0x30, 0xC8, 0x28, 0xE8, 0x10, 0xE0, 0x00, 0x07, 0x18, 0x27, 0x24, 0x23, 0x14, 0x0B, 0x00, /*"@",32*/
    0x00, 0x00, 0xC0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x3C, 0x23, 0x02, 0x02, 0x27, 0x38, 0x20, /*"A",33*/
    0x08, 0xF8, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"B",34*/
    0xC0, 0x30, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x07, 0x18, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, /*"C",35*/
    0x08, 0xF8, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, /*"D",36*/
    0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x23, 0x20, 0x18, 0x00, /*"E",37*/
    0x08, 0xF8, 0x88, 0x88, 0xE8, 0x08, 0x10, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00, /*"F",38*/
    0xC0, 0x30, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x07, 0x18, 0x20, 0x20, 0x22, 0x1E, 0x02, 0x00, /*"G",39*/
    0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x21, 0x3F, 0x20, /*"H",40*/
    0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, /*"I",41*/
    0x00, 0x00, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, 0x00, /*"J",42*/
    0x08, 0xF8, 0x88, 0xC0, 0x28, 0x18, 0x08, 0x00, 0x20, 0x3F, 0x20, 0x01, 0x26, 0x38, 0x20, 0x00, /*"K",43*/
    0x08, 0xF8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x00, /*"L",44*/
    0x08, 0xF8, 0xF8, 0x00, 0xF8, 0xF8, 0x08, 0x00, 0x20, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x20, 0x00, /*"M",45*/
    0x08, 0xF8, 0x30, 0xC0, 0x00, 0x08, 0xF8, 0x08, 0x20, 0x3F, 0x20, 0x00, 0x07, 0x18, 0x3F, 0x00, /*"N",46*/
    0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x10, 0x20, 0x20, 0x20, 0x10, 0x0F, 0x00, /*"O",47*/
    0x08, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x20, 0x3F, 0x21, 0x01, 0x01, 0x01, 0x00, 0x00, /*"P",48*/
    0xE0, 0x10, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x0F, 0x18, 0x24, 0x24, 0x38, 0x50, 0x4F, 0x00, /*"Q",49*/
    0x08, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x03, 0x0C, 0x30, 0x20, /*"R",50*/
    0x00, 0x70, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00, 0x38, 0x20, 0x21, 0x21, 0x22, 0x1C, 0x00, /*"S",51*/
    0x18, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x18, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, /*"t",52*/
    0x08, 0xF8, 0x08, 0x00, 0x00, 0x08, 0xF8, 0x08, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, /*"U",53*/
    0x08, 0x78, 0x88, 0x00, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x07, 0x38, 0x0E, 0x01, 0x00, 0x00, /*"V",54*/
    0xF8, 0x08, 0x00, 0xF8, 0x00, 0x08, 0xF8, 0x00, 0x03, 0x3C, 0x07, 0x00, 0x07, 0x3C, 0x03, 0x00, /*"W",55*/
    0x08, 0x18, 0x68, 0x80, 0x80, 0x68, 0x18, 0x08, 0x20, 0x30, 0x2C, 0x03, 0x03, 0x2C, 0x30, 0x20, /*"X",56*/
    0x08, 0x38, 0xC8, 0x00, 0xC8, 0x38, 0x08, 0x00, 0x00, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x00, 0x00, /*"Y",57*/
    0x10, 0x08, 0x08, 0x08, 0xC8, 0x38, 0x08, 0x00, 0x20, 0x38, 0x26, 0x21, 0x20, 0x20, 0x18, 0x00, /*"Z",58*/
    0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x00, /*"[",59*/
    0x00, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x38, 0xC0, 0x00, /*"\",60*/
    0x00, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00, /*"]",61*/
    0x00, 0x00, 0x04, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"^",62*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, /*"_",63*/
    0x00, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"`",64*/
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x19, 0x24, 0x22, 0x22, 0x22, 0x3F, 0x20, /*"a",65*/
    0x08, 0xF8, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x11, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"b",66*/
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x20, 0x11, 0x00, /*"c",67*/
    0x00, 0x00, 0x00, 0x80, 0x80, 0x88, 0xF8, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0x10, 0x3F, 0x20, /*"d",68*/
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x22, 0x13, 0x00, /*"e",69*/
    0x00, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x18, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, /*"f",70*/
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6B, 0x94, 0x94, 0x94, 0x93, 0x60, 0x00, /*"g",71*/
    0x08, 0xF8, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, /*"h",72*/
    0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, /*"i",73*/
    0x00, 0x00, 0x00, 0x80, 0x98, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x80, 0x80, 0x7F, 0x00, 0x00, /*"j",74*/
    0x08, 0xF8, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x24, 0x02, 0x2D, 0x30, 0x20, 0x00, /*"k",75*/
    0x00, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3F, 0x20, 0x20, 0x00, 0x00, /*"l",76*/
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x20, 0x3F, 0x20, 0x00, 0x3F, 0x20, 0x00, 0x3F, /*"m",77*/
    0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x3F, 0x21, 0x00, 0x00, 0x20, 0x3F, 0x20, /*"n",78*/
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x20, 0x1F, 0x00, /*"o",79*/
    0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xA1, 0x20, 0x20, 0x11, 0x0E, 0x00, /*"p",80*/
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x20, 0xA0, 0xFF, 0x80, /*"q",81*/
    0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x20, 0x20, 0x3F, 0x21, 0x20, 0x00, 0x01, 0x00, /*"r",82*/
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x33, 0x24, 0x24, 0x24, 0x24, 0x19, 0x00, /*"s",83*/
    0x00, 0x80, 0x80, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x00, 0x00, /*"t",84*/
    0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x1F, 0x20, 0x20, 0x20, 0x10, 0x3F, 0x20, /*"u",85*/
    0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x01, 0x0E, 0x30, 0x08, 0x06, 0x01, 0x00, /*"v",86*/
    0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 0x0F, 0x30, 0x0C, 0x03, 0x0C, 0x30, 0x0F, 0x00, /*"w",87*/
    0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x20, 0x31, 0x2E, 0x0E, 0x31, 0x20, 0x00, /*"x",88*/
    0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x81, 0x8E, 0x70, 0x18, 0x06, 0x01, 0x00, /*"y",89*/
    0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x21, 0x30, 0x2C, 0x22, 0x21, 0x30, 0x00, /*"z",90*/
    0x00, 0x00, 0x00, 0x00, 0x80, 0x7C, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x40, 0x40, /*"{",91*/
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, /*"|",92*/
    0x00, 0x02, 0x02, 0x7C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, /*"}",93*/
    0x00, 0x06, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*"~",94*/
};

static void I2C_Delay(uint8_t delay)
{
    while (delay-- != 0);
}

static void I2C_Start(void)
{
    LCD_SDA = HIGH;
    I2C_Delay(DELAY_TIME);
    LCD_SCL = HIGH;
    I2C_Delay(DELAY_TIME);
    LCD_SDA = LOW;
    I2C_Delay(DELAY_TIME);
    LCD_SCL = LOW;
    I2C_Delay(DELAY_TIME);
}

static void I2C_Stop(void)
{
    LCD_SDA = LOW;
    LCD_SCL = HIGH;
    I2C_Delay(DELAY_TIME);
    LCD_SDA = HIGH;
    I2C_Delay(DELAY_TIME);
    LCD_SCL = HIGH;
}

static bool I2C_SendByte(uint8_t send)
{
    bool ret = FALSE;
    uint8_t i;
    uint8_t read;

    for (i = 0; i < 8; i++) {
        LCD_SCL = LOW;
        if ((send & 0x80) == 0x80) {
            LCD_SDA = HIGH;
        } else {
            LCD_SDA = LOW;
        }
        I2C_Delay(DELAY_TIME);
        LCD_SCL = HIGH;
        I2C_Delay(DELAY_TIME);
        send <<= 1;
    }
    LCD_SCL = LOW;

    SDA_IN();
    I2C_Delay(DELAY_TIME);
    LCD_SCL = HIGH;
    I2C_Delay(DELAY_TIME);
    read = SDA_PORT;
    if ((read & SDA_NUMBER) == SDA_NUMBER) {
        ret = TRUE;
    }
    I2C_Delay(DELAY_TIME);
    LCD_SCL = LOW;
    SDA_OUT();
    I2C_Delay(DELAY_TIME);

    return ret;
}

static void OLED_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(SI7201_ADDR);
    I2C_SendByte(0x00);
    I2C_SendByte(cmd);
    I2C_Stop();
}

static void OLED_WriteData(uint8_t writeData)
{
    I2C_Start();
    I2C_SendByte(SI7201_ADDR);
    I2C_SendByte(0x40);
    I2C_SendByte(writeData);
    I2C_Stop();
}

static void OLED_SetXY(uint8_t x, uint8_t y)
{
    OLED_WriteCmd(0xb0 | y);
    OLED_WriteCmd(((0xf0 & x) >> 4) | 0x10);
    OLED_WriteCmd((0x0f & x) | 0x01);
}

void OLED_Off(void)
{
    OLED_WriteCmd(0xae);
}

void OLED_Init(void)
{
    LCD_RESET = HIGH;
    DelayMs(10);
    LCD_RESET = LOW;
    DelayMs(10);
    LCD_RESET = HIGH;
    DelayMs(10);

    OLED_WriteCmd(0xae);       // Turn off oled panel
    OLED_WriteCmd(0x00);       // Set low column address
    OLED_WriteCmd(0x10);       // Set high column address
    OLED_WriteCmd(0x40);       // Set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WriteCmd(0x81);       // Set contrast control register
    OLED_WriteCmd(BRIGHTNESS); // Set SEG Output Current BRIGHTNESS
    OLED_WriteCmd(0xa1);       // Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WriteCmd(0xc8);       // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WriteCmd(0xa6);       // Set normal display
    OLED_WriteCmd(0xa8);       // Set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3f);       // 1/64 duty
    OLED_WriteCmd(0xd3);       // Set display offset        Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WriteCmd(0x00);       // Not offset
    OLED_WriteCmd(0xd5);       // Set display clock divide ratio/oscillator frequency
    OLED_WriteCmd(0x80);       // Set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WriteCmd(0xd9);       // Set pre-charge period
    OLED_WriteCmd(0xf1);       // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WriteCmd(0xda);       // Set com pins hardware configuration
    OLED_WriteCmd(0x12);       //
    OLED_WriteCmd(0xdb);       // Set vcomh
    OLED_WriteCmd(0x40);       // Set VCOM Deselect Level
    OLED_WriteCmd(0x20);       // Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WriteCmd(0x02);       //
    OLED_WriteCmd(0x8d);       // Set Charge Pump enable/disable
    OLED_WriteCmd(0x14);       // Set(0x10) disable
    OLED_WriteCmd(0xa4);       // Disable Entire Display On (0xa4/0xa5)
    OLED_WriteCmd(0xa6);       // Disable Inverse Display On (0xa6/a7)
    OLED_WriteCmd(0xaf);       // Turn on oled panel
    OLED_SetXY(0, 0);
}

static void DisplayWrite(uint8_t x, uint8_t y, uint8_t index, uint8_t flags)
{
    uint8_t i;
    uint8_t t, z;

    OLED_SetXY(x, y);
    for (i = 0; i < 8; i++) {
        t = 0x00;
        z = g_table8X16[index + i];
        if (z & flags) {
            t |= 0x80;
        }
        flags >>= 1;
        if (z & flags) {
            t |= 0x20;
        }
        flags >>= 1;
        if (z & flags) {
            t |= 0x08;
        }
        flags >>= 1;
        if (z & flags) {
            t |= 0x02;
        }
        OLED_WriteData(t);
    }
}

void OLED_P8x16Str(void)
{
    uint8_t c, j;
    uint8_t x, y;

    x = 0x00;
    y = 0x00;
    j = 0x00;

    do {
        c = g_letterTable[j] - 32;

        DisplayWrite(x, y, c * 16, 0x08);
        DisplayWrite(x, y + 1, c * 16, 0x80);
        DisplayWrite(x, y + 2, c * 16 + 8, 0x08);
        DisplayWrite(x, y + 3, c * 16 + 8, 0x80);

        x += 8;
        j++;
        if (j == 16) {
            x = 0x00;
            y = 4;
        }
    } while (j < 32);
}

static void ClearDispBuffer(void)
{
    uint8_t i;

    for (i = 0; i < 32; i++) {
        g_letterTable[i] = ' ';
    }
}

static void DispYear(enum SetItemType item, struct TimeType *time)
{
    bool f_temp = FALSE;

    if (item == CLOCK_SET_YEAR) {
        if (RTC_GetTimeFlag(SET_COL_FLAG) || KEY_GetKeyFlag(HOLD_KEY_FLAG)) {
            f_temp = TRUE;
        }
    } else {
        f_temp = TRUE;
    }
    if (f_temp == TRUE) {
        g_letterTable[1] = '2';
        g_letterTable[2] = '0';
        g_letterTable[3] = HexToAsc(time->year % 100 / 10);
        g_letterTable[4] = HexToAsc(time->year % 100 % 10);
    }
}

static void DispMonth(enum SetItemType item, struct TimeType *time)
{
    bool f_temp = FALSE;

    g_letterTable[5] = '-';
    if (item == CLOCK_SET_MONTH) {
        if (RTC_GetTimeFlag(SET_COL_FLAG) || KEY_GetKeyFlag(HOLD_KEY_FLAG)) {
            f_temp = TRUE;
        }
    } else {
        f_temp = TRUE;
    }
    if (f_temp == TRUE) {
        g_letterTable[6] = HexToAsc(time->month / 10);
        g_letterTable[7] = HexToAsc(time->month % 10);
    }
}

static void DispDay(enum SetItemType item, struct TimeType *time)
{
    bool f_temp = FALSE;

    g_letterTable[8] = '-';
    if (item == CLOCK_SET_DAY) {
        if (RTC_GetTimeFlag(SET_COL_FLAG) || KEY_GetKeyFlag(HOLD_KEY_FLAG)) {
            f_temp = TRUE;
        }
    } else {
        f_temp = TRUE;
    }
    if (f_temp == TRUE) {
        g_letterTable[9]  = HexToAsc(time->day / 10);
        g_letterTable[10] = HexToAsc(time->day % 10);
    }
}

static void DispWeek(struct TimeType *time)
{
    g_letterTable[12] = g_weekTable[time->week][0];
    g_letterTable[13] = g_weekTable[time->week][1];
    g_letterTable[14] = g_weekTable[time->week][2];
}

static void DispHour(enum SetItemType item, struct TimeType *time)
{
    bool f_temp = FALSE;

    if (item == CLOCK_SET_HOUR) {
        if (RTC_GetTimeFlag(SET_COL_FLAG) || KEY_GetKeyFlag(HOLD_KEY_FLAG)) {
            f_temp = TRUE;
        }
    } else {
        f_temp = TRUE;
    }
    if (f_temp == TRUE) {
        g_letterTable[20] = HexToAsc(time->hour / 10);
        g_letterTable[21] = HexToAsc(time->hour % 10);
    }
}

static void DispMin(enum SetItemType item, struct TimeType *time)
{
    bool f_temp = FALSE;

    g_letterTable[22] = ':';
    if (item == CLOCK_SET_MIN) {
        if (RTC_GetTimeFlag(SET_COL_FLAG) || KEY_GetKeyFlag(HOLD_KEY_FLAG)) {
            f_temp = TRUE;
        }
    } else {
        f_temp = TRUE;
    }
    if (f_temp == TRUE) {
        g_letterTable[23] = HexToAsc(time->min / 10);
        g_letterTable[24] = HexToAsc(time->min % 10);
    }
}

static void DispSec(struct TimeType *time)
{
    g_letterTable[25] = ':';
    g_letterTable[26] = HexToAsc(time->sec / 10);
    g_letterTable[27] = HexToAsc(time->sec % 10);
}

void OLED_DispClock(void)
{
    enum SetItemType item = KEY_GetItem();
    struct TimeType *time = RTC_GetTime();

    ClearDispBuffer();
    DispYear(item, time);
    DispMonth(item, time);
    DispDay(item, time);
    DispWeek(time);
    DispHour(item, time);
    DispMin(item, time);
    DispSec(time);
}

static void DispTemperature(int16_t temp)
{
    uint8_t tempL;
    uint8_t tempH;
    uint16_t negative;

    if (temp < 0) {
        g_letterTable[4] = '-';
        negative = ~temp;
        negative++;
    } else {
        negative = temp;
    }

    tempH = negative / 100; // high
    tempL = negative % 100; // low

    g_letterTable[5]  = HexToAsc(tempH / 10);
    g_letterTable[6]  = HexToAsc(tempH % 10);
    g_letterTable[7]  = '.';
    g_letterTable[8]  = HexToAsc(tempL / 10);
    g_letterTable[9]  = HexToAsc(tempL % 10);
    g_letterTable[10] = 'C';
}

static void DispHumidity(uint16_t humi)
{
    uint8_t tempL;
    uint8_t tempH;

    tempH = humi / 100; // high
    tempL = humi % 100; // low

    g_letterTable[16 + 5]  = HexToAsc(tempH / 10);
    g_letterTable[16 + 6]  = HexToAsc(tempH % 10);
    g_letterTable[16 + 7]  = '.';
    g_letterTable[16 + 8]  = HexToAsc(tempL / 10);
    g_letterTable[16 + 9]  = HexToAsc(tempL % 10);
    g_letterTable[16 + 10] = '%';
}

void OLED_DispTempHumi(void)
{
    int16_t temp  = SI7021_GetTemp();
    uint16_t humi = SI7021_GetHumi();

    ClearDispBuffer();
    DispTemperature(temp);
    DispHumidity(humi);
}

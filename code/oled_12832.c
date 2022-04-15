#ifndef OS_MASTER_FILE
#define OS_GLOBALS
#include "includes.h"
#endif

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
#define SDAIN()              \
    do {                     \
        P0IO &= ~SDA_NUMBER; \
    } while (0)
#define SDAOUT()            \
    do {                    \
        P0IO |= SDA_NUMBER; \
    } while (0)

#define I2C_OLED   0x78
#define Brightness 250

u8 Tbl[32];
extern code u8 F8X16[];

void I2C_Delay(void)
{
    u8 i = 0;

    while (i--)
        ;
}

void I2C_Start(void)
{
    LCD_SDA = 1;
    I2C_Delay();
    LCD_SCL = 1;
    I2C_Delay();
    LCD_SDA = 0;
    I2C_Delay();
    LCD_SCL = 0;
    I2C_Delay();
}

void I2C_Stop(void)
{
    LCD_SDA = 0;
    LCD_SCL = 1;
    I2C_Delay();
    LCD_SDA = 1;
    I2C_Delay();
    LCD_SCL = 1;
}

BOOLEAN I2C_SendByte(u8 Byte)
{
    BOOLEAN F_temp = 0;
    u8 Cnt, j;

    for (Cnt = 0; Cnt < 8; Cnt++) {
        LCD_SCL = 0;
        if (Byte & 0x80) {
            LCD_SDA = 1;
        } else {
            LCD_SDA = 0;
        }
        I2C_Delay();
        LCD_SCL = 1;
        I2C_Delay();
        Byte = Byte << 1;
    }
    LCD_SCL = 0;

    SDAIN();
    I2C_Delay();
    LCD_SCL = 1;
    I2C_Delay();
    j = SDA_PORT;
    if ((j & SDA_NUMBER) == SDA_NUMBER) {
        F_temp = 1;
    }
    I2C_Delay();
    LCD_SCL = 0;
    SDAOUT();
    I2C_Delay();

    return F_temp;
}

void OLED_WriteCmd(u8 Cmd)
{
    I2C_Start();
    I2C_SendByte(I2C_OLED);
    I2C_SendByte(0x00);
    I2C_SendByte(Cmd);
    I2C_Stop();
}

void OLED_WriteData(u8 Data)
{
    I2C_Start();
    I2C_SendByte(I2C_OLED);
    I2C_SendByte(0x40);
    I2C_SendByte(Data);
    I2C_Stop();
}

void OLED_SetXY(u8 x, u8 y)
{
    OLED_WriteCmd(0xb0 | y);
    OLED_WriteCmd(((0xf0 & x) >> 4) | 0x10);
    OLED_WriteCmd((0x0f & x) | 0x01);
}

void OLED_OFF(void)
{
    OLED_WriteCmd(0xae);
}

void OLED_Init(void)
{
    LCD_RESET = 1;
    DelayMs(10);
    LCD_RESET = 0;
    DelayMs(10);
    LCD_RESET = 1;
    DelayMs(10);

    OLED_WriteCmd(0xae);       //--turn off oled panel
    OLED_WriteCmd(0x00);       //---set low column address
    OLED_WriteCmd(0x10);       //---set high column address
    OLED_WriteCmd(0x40);       //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WriteCmd(0x81);       //--set contrast control register
    OLED_WriteCmd(Brightness); // Set SEG Output Current Brightness
    OLED_WriteCmd(0xa1);       //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WriteCmd(0xc8);       //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WriteCmd(0xa6);       //--set normal display
    OLED_WriteCmd(0xa8);       //--set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3f);       //--1/64 duty
    OLED_WriteCmd(0xd3);       //-set display offset        Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WriteCmd(0x00);       //-not offset
    OLED_WriteCmd(0xd5);       //--set display clock divide ratio/oscillator frequency
    OLED_WriteCmd(0x80);       //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WriteCmd(0xd9);       //--set pre-charge period
    OLED_WriteCmd(0xf1);       //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WriteCmd(0xda);       //--set com pins hardware configuration
    OLED_WriteCmd(0x12);       //
    OLED_WriteCmd(0xdb);       //--set vcomh
    OLED_WriteCmd(0x40);       //Set VCOM Deselect Level
    OLED_WriteCmd(0x20);       //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WriteCmd(0x02);       //
    OLED_WriteCmd(0x8d);       //--set Charge Pump enable/disable
    OLED_WriteCmd(0x14);       //--set(0x10) disable
    OLED_WriteCmd(0xa4);       // Disable Entire Display On (0xa4/0xa5)
    OLED_WriteCmd(0xa6);       // Disable Inverse Display On (0xa6/a7)
    OLED_WriteCmd(0xaf);       //--turn on oled panel
    OLED_SetXY(0, 0);
}

void OLED_P8x16Str(void)
{
    u8 c, i, j;
    u8 z, T, x, y;

    x = 0x00;
    y = 0x00;
    j = 0x00;

    do {
        c = Tbl[j] - 32;
        OLED_SetXY(x, y);
        for (i = 0; i < 8; i++) {
            T = 0x00;
            z = F8X16[c * 16 + i];
            if (z & 0x08) {
                T |= 0x80;
            }
            if (z & 0x04) {
                T |= 0x20;
            }
            if (z & 0x02) {
                T |= 0x08;
            }
            if (z & 0x01) {
                T |= 0x02;
            }
            OLED_WriteData(T);
        }
        OLED_SetXY(x, y + 1);
        for (i = 0; i < 8; i++) {
            T = 0x00;
            z = F8X16[c * 16 + i];
            if (z & 0x80) {
                T |= 0x80;
            }
            if (z & 0x40) {
                T |= 0x20;
            }
            if (z & 0x20) {
                T |= 0x08;
            }
            if (z & 0x10) {
                T |= 0x02;
            }
            OLED_WriteData(T);
        }
        OLED_SetXY(x, y + 2);
        for (i = 0; i < 8; i++) {
            T = 0x00;
            z = F8X16[c * 16 + i + 8];
            if (z & 0x08) {
                T |= 0x80;
            }
            if (z & 0x04) {
                T |= 0x20;
            }
            if (z & 0x02) {
                T |= 0x08;
            }
            if (z & 0x01) {
                T |= 0x02;
            }
            OLED_WriteData(T);
        }
        OLED_SetXY(x, y + 3);
        for (i = 0; i < 8; i++) {
            T = 0x00;
            z = F8X16[c * 16 + i + 8];
            if (z & 0x80) {
                T |= 0x80;
            }
            if (z & 0x40) {
                T |= 0x20;
            }
            if (z & 0x20) {
                T |= 0x08;
            }
            if (z & 0x10) {
                T |= 0x02;
            }
            OLED_WriteData(T);
        }
        x += 8;
        j++;
        if (j == 16) {
            x = 0x00;
            y = 4;
        }
    } while (j < 32);
}

code u8 WeekTable[8][3] = {
    'S', 'u', 'n', 'M', 'o', 'n', 'T', 'u', 'e', 'W', 'e', 'd', 'T', 'h', 'u', 'F', 'r', 'i', 'S', 't', 'a',
};

void OLED_DispClock(void)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        Tbl[i] = ' ';
    }

    if (SetItem == _CLOCK_SET_YEAR_) {
        if (F_SET_COL || F_HoldKey) {
            Tbl[1] = '2';
            Tbl[2] = '0';
            Tbl[3] = HexToAsc(TIME.year % 100 / 10);
            Tbl[4] = HexToAsc(TIME.year % 100 % 10);
        }
    } else {
        Tbl[1] = '2';
        Tbl[2] = '0';
        Tbl[3] = HexToAsc(TIME.year % 100 / 10);
        Tbl[4] = HexToAsc(TIME.year % 100 % 10);
    }
    Tbl[5] = '-';
    if (SetItem == _CLOCK_SET_MONTH_) {
        if (F_SET_COL || F_HoldKey) {
            Tbl[6] = HexToAsc(TIME.month / 10);
            Tbl[7] = HexToAsc(TIME.month % 10);
        }
    } else {
        Tbl[6] = HexToAsc(TIME.month / 10);
        Tbl[7] = HexToAsc(TIME.month % 10);
    }
    Tbl[8] = '-';
    if (SetItem == _CLOCK_SET_DAY_) {
        if (F_SET_COL || F_HoldKey) {
            Tbl[9]  = HexToAsc(TIME.day / 10);
            Tbl[10] = HexToAsc(TIME.day % 10);
        }
    } else {
        Tbl[9]  = HexToAsc(TIME.day / 10);
        Tbl[10] = HexToAsc(TIME.day % 10);
    }
    Tbl[12] = WeekTable[TIME.week][0];
    Tbl[13] = WeekTable[TIME.week][1];
    Tbl[14] = WeekTable[TIME.week][2];

    if (SetItem == _CLOCK_SET_HOUR_) {
        if (F_SET_COL || F_HoldKey) {
            Tbl[20] = HexToAsc(TIME.hour / 10);
            Tbl[21] = HexToAsc(TIME.hour % 10);
        }
    } else {
        Tbl[20] = HexToAsc(TIME.hour / 10);
        Tbl[21] = HexToAsc(TIME.hour % 10);
    }
    Tbl[22] = ':';
    if (SetItem == _CLOCK_SET_MIN_) {
        if (F_SET_COL || F_HoldKey) {
            Tbl[23] = HexToAsc(TIME.min / 10);
            Tbl[24] = HexToAsc(TIME.min % 10);
        }
    } else {
        Tbl[23] = HexToAsc(TIME.min / 10);
        Tbl[24] = HexToAsc(TIME.min % 10);
    }

    Tbl[25] = ':';
    Tbl[26] = HexToAsc(TIME.sec / 10);
    Tbl[27] = HexToAsc(TIME.sec % 10);
}

void OLED_DispTempHumi(void)
{
    u8 i;
    u8 tmp0, tmp1;
    s8 tmp2, tmp3;

    for (i = 0; i < 32; i++) {
        Tbl[i] = ' ';
    }

    tmp3 = temp / 100; //high
    tmp2 = temp % 100; //low
    if (temp < 0) {
        Tbl[4] = '-';
        tmp3   = 0 - tmp3;
        tmp2   = 0 - tmp2;
    }
    Tbl[5]  = HexToAsc(tmp3 / 10);
    Tbl[6]  = HexToAsc(tmp3 % 10);
    Tbl[7]  = '.';
    Tbl[8]  = HexToAsc(tmp2 / 10);
    Tbl[9]  = HexToAsc(tmp2 % 10);
    Tbl[10] = 'C';

    tmp1         = humi / 100; //high
    tmp0         = humi % 100; //low
    Tbl[16 + 5]  = HexToAsc(tmp1 / 10);
    Tbl[16 + 6]  = HexToAsc(tmp1 % 10);
    Tbl[16 + 7]  = '.';
    Tbl[16 + 8]  = HexToAsc(tmp0 / 10);
    Tbl[16 + 9]  = HexToAsc(tmp0 % 10);
    Tbl[16 + 10] = '%';
}

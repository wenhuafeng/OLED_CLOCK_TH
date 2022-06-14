#include <intrins.h>
#include "MC96F6432.h"
#include "si7021.h"
#include "func_def.h"
#include "gp_sub.h"
#include "key_func.h"

#define I2C_DELAY_TIME 250

#define HSB 0
#define LSB 1

#define BIT_HIGH 1
#define BIT_LOW  0

#define WRITE_CMD  0x80
#define READ_CDM   0x81
#define SALVE_ADDR 0x80

#define HUMI_HOLD_MASTER 0xE5
#define TEMP_HOLD_MASTER 0xE3

#define HUMI_NOHOLD_MASTER 0xF5
#define TEMP_NOHOLD_MASTER 0xF3

#define SI7021_SCL        P11
#define SI7021_SDA        P10
#define SI7021_SDA_PORT   P1
#define SI7021_SDA_NUMBER (1 << 0)

#define SI7021_SDAIN()              \
    do {                            \
        P1IO &= ~SI7021_SDA_NUMBER; \
    } while (0)
#define SI7021_SDAOUT()            \
    do {                           \
        P1IO |= SI7021_SDA_NUMBER; \
    } while (0)

#define SI7021_SCL_HIGH()      \
    do {                       \
        SI7021_SCL = BIT_HIGH; \
    } while (0)
#define SI7021_SCL_LOW()      \
    do {                      \
        SI7021_SCL = BIT_LOW; \
    } while (0)

#define SI7021_SDA_HIGH()      \
    do {                       \
        SI7021_SDA = BIT_HIGH; \
    } while (0)
#define SI7021_SDA_LOW()      \
    do {                      \
        SI7021_SDA = BIT_LOW; \
    } while (0)

union union16 {
    unsigned int uint;
    unsigned char uchar[2];
};

union union32 {
    long _long;
    unsigned int uint[2];
    unsigned char uchar[4];
};

struct Si7021Type {
    int16_t temp;
    uint16_t humi;
    uint8_t crc;
};
static struct Si7021Type g_si7021;

static void I2C_DelayUs(uint8_t delay)
{
    while (delay-- != 0) {
        _nop_();
    }
}

static void I2C_Start(void)
{
    SI7021_SDA_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SDA_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
}

static void I2C_Stop(void)
{
    SI7021_SDA_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SDA_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
}

static BOOLEAN I2C_SendByte(uint8_t send_data)
{
    uint8_t i;
    BOOLEAN ret = 0;
    uint8_t delay;
    uint8_t read;

    for (i = 0; i < 8; i++) {
        SI7021_SCL_LOW();
        if ((send_data << i) & 0x80) {
            SI7021_SDA_HIGH();
        } else {
            SI7021_SDA_LOW();
        }
        I2C_DelayUs(I2C_DELAY_TIME);
        SI7021_SCL_HIGH();
        I2C_DelayUs(I2C_DELAY_TIME);
    }

    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_LOW();
    SI7021_SDA_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);

    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);

    SI7021_SDAIN();
    delay = 200;
    while (delay--) {
        I2C_DelayUs(I2C_DELAY_TIME);
        read = SI7021_SDA_PORT;
        if ((read & SI7021_SDA_NUMBER) == 0x00) {
            ret = 1;
            break;
        }
    }
    SI7021_SDAOUT();

    if (delay == 0) {
        ret = 0;
    }

    SI7021_SCL_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);

    return ret;
}

static uint8_t I2C_ReadByte(void)
{
    uint8_t value = 0;
    uint8_t i;
    uint8_t read;

    SI7021_SDAIN();
    for (i = 0; i < 8; i++) {
        SI7021_SCL_HIGH();
        I2C_DelayUs(I2C_DELAY_TIME);
        value <<= 1;
        read = SI7021_SDA_PORT;
        if (read & SI7021_SDA_NUMBER) {
            value += 1;
        }
        SI7021_SCL_LOW();
    }
    SI7021_SDAOUT();

    return value;
}

static void I2C_Ack(void)
{
    SI7021_SDA_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_LOW();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SDA_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
}

static void I2C_Nack(void)
{
    SI7021_SDA_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_HIGH();
    I2C_DelayUs(I2C_DELAY_TIME);
    SI7021_SCL_LOW();
}

static void SI7021_Measure(uint8_t model, union union16 *value)
{
    uint16_t tmp;
    FP32 buff;
    uint8_t crc8;

    I2C_Start();
    if (0 == I2C_SendByte(SALVE_ADDR)) { // slave addr
        value->uint = 0xAABB;
        return;
    }
    if (0 == I2C_SendByte(model)) { // measure cmd
        value->uint = 0x1234;
        return;
    }

    DelayMs(100);

    I2C_Start();
    if (0 == I2C_SendByte(SALVE_ADDR + 1)) {
        value->uint = 0x3456;
        return;
    }

    value->uchar[HSB] = I2C_ReadByte();
    I2C_Ack();
    value->uchar[LSB] = I2C_ReadByte();
    I2C_Ack();
    crc8 = I2C_ReadByte();
    I2C_Nack();
    I2C_Stop();

    tmp = (value->uchar[HSB] << 8) | value->uchar[LSB];
    if (model != TEMP_HOLD_MASTER) {
        buff = tmp * 125.0;
        buff = buff / 65536 - 6;
        g_si7021.humi = buff * 100;
    } else {
        buff = tmp * 175.72;
        buff = buff / 65536 - 46.85;
        g_si7021.temp = buff * 100;
    }
}

void SI7021_SampleTempHumi(void)
{
    static uint8_t count = 0;

    count++;
    if (count > 10) {
        count = 0x00;
        if (KEY_GetSetModeCtr() == 0) {
            SI7021_Measure(TEMP_HOLD_MASTER, (union union16 *)(&g_si7021.temp));
            DelayMs(1);
            SI7021_Measure(HUMI_HOLD_MASTER, (union union16 *)(&g_si7021.humi));
        }
    }
}

int16_t SI7021_GetTemp(void)
{
    return g_si7021.temp;
}

uint16_t SI7021_GetHumi(void)
{
    return g_si7021.humi;
}

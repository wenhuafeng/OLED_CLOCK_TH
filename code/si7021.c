#include <intrins.h>
#include "MC96F6432.h"
#include "si7021.h"
#include "type_define.h"
#include "gp_sub.h"
#include "key_func.h"

#define I2C_DELAY_TIME 250

#define HSB 0
#define LSB 1

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

#define SI7021_SDA_IN()             \
    do {                            \
        P1IO &= ~SI7021_SDA_NUMBER; \
    } while (0)
#define SI7021_SDA_OUT()           \
    do {                           \
        P1IO |= SI7021_SDA_NUMBER; \
    } while (0)

#define SI7021_SCL_HIGH()  \
    do {                   \
        SI7021_SCL = HIGH; \
    } while (0)
#define SI7021_SCL_LOW()  \
    do {                  \
        SI7021_SCL = LOW; \
    } while (0)

#define SI7021_SDA_HIGH()  \
    do {                   \
        SI7021_SDA = HIGH; \
    } while (0)
#define SI7021_SDA_LOW()  \
    do {                  \
        SI7021_SDA = LOW; \
    } while (0)

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

static bool I2C_SendByte(uint8_t send)
{
    uint8_t i;
    bool ret = FALSE;
    uint8_t delay;
    uint8_t read;

    for (i = 0; i < 8; i++) {
        SI7021_SCL_LOW();
        if ((send << i) & 0x80) {
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

    SI7021_SDA_IN();
    delay = 200;
    while (delay != 0x00) {
        I2C_DelayUs(I2C_DELAY_TIME);
        read = SI7021_SDA_PORT;
        if ((read & SI7021_SDA_NUMBER) == 0x00) {
            ret = TRUE;
            break;
        }
        delay--;
    }
    SI7021_SDA_OUT();
    if (delay == 0) {
        ret = FALSE;
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

    SI7021_SDA_IN();
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
    SI7021_SDA_OUT();

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

static bool SI7021_Measure(uint8_t type)
{
    uint8_t read[2];
    uint8_t crc8;
    uint16_t temp;
    FP32 buffer;

    I2C_Start();
    if (I2C_SendByte(SALVE_ADDR) == FALSE) { // slave addr
        return FALSE;
    }
    if (I2C_SendByte(type) == FALSE) { // measure cmd
        return FALSE;
    }

    DelayMs(100);
    I2C_Start();
    if (I2C_SendByte(SALVE_ADDR + 1) == FALSE) {
        return FALSE;
    }

    read[HSB] = I2C_ReadByte();
    I2C_Ack();
    read[LSB] = I2C_ReadByte();
    I2C_Ack();
    crc8 = I2C_ReadByte();
    I2C_Nack();
    I2C_Stop();

    temp = (uint16_t)(read[HSB] << 8) | read[LSB];
    if (type != TEMP_HOLD_MASTER) {
        buffer          = temp * 125.0;
        buffer          = buffer / 65536 - 6;
        g_si7021.humi = buffer * 100;
    } else {
        buffer          = temp * 175.72;
        buffer          = buffer / 65536 - 46.85;
        g_si7021.temp = buffer * 100;
    }

    return TRUE;
}

void SI7021_SampleTempHumi(void)
{
    bool f_temp;
    static uint8_t count = 0;

    count++;
    if (count > 9) {
        count = 0x00;
        if (KEY_GetSetModeCtr() == 0) {
            f_temp = SI7021_Measure(TEMP_HOLD_MASTER);
            if (f_temp == FALSE) {
                return;
            }

            DelayMs(1);
            f_temp = SI7021_Measure(HUMI_HOLD_MASTER);
            if (f_temp == FALSE) {
                return;
            }
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

#ifndef SI7021_H
#define SI7021_H

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

typedef struct _si7021_value_ {
    u16 temp;
    u16 humi;
    u8 crc;
} si7021_value_ST, *p_si7021_value_ST;

extern si7021_value_ST gv_si7021_value;

typedef union union16_ {
    unsigned int uint;
    unsigned char uchar[2];
} union16;
typedef union union32_ {
    long _long;
    unsigned int uint[2];
    unsigned char uchar[4];
} union32;

extern s16 temp;
extern u16 humi;

void measure_si7021(u8 model, union16 *value);

#endif

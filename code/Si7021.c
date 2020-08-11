

#ifndef  OS_MASTER_FILE
#define  OS_GLOBALS
#include "includes.h"
#endif


si7021_value_ST gv_si7021_value;
s16 temp;
u16 humi;//0%~100%


void delay_x_us (void)//(1+2)*60=180???? 180us
{
  u8 x=250;
  for (; x>0; x--) {
    _nop_();
  }
}

//????: start_i2c
//????: i2c ??????
//????:
// ? ? ?:
void start_i2c (void)
{
  SI7021_SDA_HIGH();
  delay_x_us();
  SI7021_SCL_HIGH();
  delay_x_us();
  SI7021_SDA_LOW();
  delay_x_us();
  SI7021_SCL_LOW();
  delay_x_us();
}

//????: stop_i2c
//????: i2c ??????
//????:
// ? ? ?:
void stop_i2c ( void )
{
  SI7021_SDA_LOW();
  delay_x_us();
  SI7021_SCL_HIGH();
  delay_x_us();
  SI7021_SDA_HIGH();
  delay_x_us();
}

//????: send_1byte
//????: ??????si7021??
//????:
// ? ? ?:
BOOLEAN send_1byte (u8 send_data)
{
  u8 bit_cnt;
  BOOLEAN  b_ack=0;
  u8 i=200;
  u8 j;
  
  for(bit_cnt=0; bit_cnt<8; bit_cnt++)
  {
    SI7021_SCL_LOW();
    if ((send_data << bit_cnt) & 0x80) {
      SI7021_SDA_HIGH();
    } else {
      SI7021_SDA_LOW();
    }
    delay_x_us();
    SI7021_SCL_HIGH();
    delay_x_us();
  }

  delay_x_us();
  SI7021_SCL_LOW();
  SI7021_SDA_HIGH();
  delay_x_us();
  
  delay_x_us();
  SI7021_SCL_HIGH();
  delay_x_us();
  
  SI7021_SDAIN();
  i = 200;
  while (i--)
  {
    delay_x_us();
    j = SI7021_SDA_PORT;
    if ((j & SI7021_SDA_NUMBER) == 0x00) {
      b_ack = 1;
      break;
    }
  }
  SI7021_SDAOUT();
  
  if (i == 0) b_ack = 0;
  
  SI7021_SCL_LOW();
  delay_x_us();
  
  return b_ack;
}

//????: read_1byte
//????: ?si7021??????
//????:
// ? ? ?:
//
u8 read_1byte(void)
{
  u8 read_value=0;
  u8 bit_cnt;
  u8 j;

  SI7021_SDAIN();
  for (bit_cnt=0; bit_cnt<8; bit_cnt++)
  {
    SI7021_SCL_HIGH();
    delay_x_us();
    read_value <<= 1;
    j = SI7021_SDA_PORT;
    if (j & SI7021_SDA_NUMBER) {
     read_value +=1;
    }
    SI7021_SCL_LOW();
  }
  SI7021_SDAOUT();
  
  return (read_value);
}

//????:master_i2c_ack
//????:MCU?????
//????:
// ? ? ?:
void master_i2c_ack (void)
{
  SI7021_SDA_LOW();
  delay_x_us();
  SI7021_SCL_LOW();
  delay_x_us();      
  SI7021_SCL_HIGH();
  delay_x_us();
  SI7021_SCL_LOW();    
  delay_x_us(); 
  SI7021_SDA_HIGH();
  delay_x_us();
}

//????:master_i2c_noack
//????:MCU???????????
//????:
// ? ? ?:
void master_i2c_noack (void)
{
  SI7021_SDA_HIGH();
  delay_x_us();
  SI7021_SCL_HIGH();
  delay_x_us();
  SI7021_SCL_LOW();
}

//????:measure_si7021
//????:HOLD MASTER???????? 
//????:
// ? ? ?:
void measure_si7021 (u8 model,union16 *value)
{
  //u16 i;
  u16 tmp;
  FP32 buff;
  //u32 buf;
  u8 crc8;
  
  //?????
  start_i2c();
  if (0 == send_1byte(SALVE_ADDR))//?slave addr
  {
    value->uint = 0xAABB;
    return;
  }
  
  if (0 == send_1byte(model))//measure cmd
  {
    value->uint = 0x1234;
    return;
  }
  //?????????10~12ms ?? 6~10ms  
  //i = 600;  //600*180=108ms  ?????
  //while (i--) {
  //  delay_x_us();  //???????????(??????,??????)
  //}
  DelayMs(100);
  
  start_i2c();         //???????
  if (0==send_1byte(SALVE_ADDR+1)) //???
  {
    value->uint = 0x3456;
    return;
  }

  value->uchar[HSB] = read_1byte();//??????????
  //mcu??
  master_i2c_ack();
  value->uchar[LSB] = read_1byte();//????????????
  //mcu???
  master_i2c_ack();
  crc8 = read_1byte();
  master_i2c_noack();
  
  //mcu??->read_1byte()?????->mcu??

  //?????
  //value->uint = 0x7890;
  stop_i2c();
  
  tmp = (value->uchar[HSB] << 8) | value->uchar[LSB];
  if (model != TEMP_HOLD_MASTER) {
    buff = tmp*125.0;
    buff = buff/65536-6;
    humi = buff*100;
    _nop_();
  } else {
    buff = tmp*175.72;
    buff = buff/65536-46.85;
    temp = buff*100;
    _nop_();
  }
}


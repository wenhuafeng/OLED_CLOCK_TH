#!/usr/bin/python

import os
import sys
import datetime

BUILD_LOG_FILE    = "build_log.txt"
BUILD_COMMAND     = 'D:\\Keil_v5\\UV4\\UV4.exe -sg -j0 -b OLED_CLOCK_TH.uvproj -o ./build_log.txt'

COPY_OUT_FILE     = 'copy output\\OLED_CLOCK_TH.hex run'
OUT_FILE_NAME     = 'OLED_CLOCK_TH.hex'

CHECKSUM_ALL      = 'checksum.exe OLED_CLOCK_TH.hex ALL'
CHECKSUM_CHECKSUM = 'checksum.exe OLED_CLOCK_TH.hex CHECKSUM'
CHECKSUM_CRC16    = 'checksum.exe OLED_CLOCK_TH.hex CRC16'
CHECKSUM_CRC32    = 'checksum.exe OLED_CLOCK_TH.hex CRC32'
CHECKSUM_SHA256   = 'checksum.exe OLED_CLOCK_TH.hex SHA256'

# 编译项目
os.system(BUILD_COMMAND)

# 输出编译log文件
with open(BUILD_LOG_FILE, "r") as f:
    data = f.read()
    print(data)

# 复制输出文件到run目录
os.system(COPY_OUT_FILE)

# 切换目录到run
path = "./run"
os.chdir(path)

# 输出文件需要的信息
os.system(CHECKSUM_ALL)
#os.system(CHECKSUM_CHECKSUM)
#os.system(CHECKSUM_CRC16)
#os.system(CHECKSUM_CRC32)
#os.system(CHECKSUM_SHA256)
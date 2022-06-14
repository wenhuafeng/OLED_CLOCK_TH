#!/usr/bin/python

import os
import datetime
import shutil

BUILD_LOG_FILE    = "build_log.txt"
BUILD_COMMAND     = 'D:\\Keil_v5\\UV4\\UV4.exe -sg -j0 -b OLED_CLOCK_TH.uvproj -o ./build_log.txt'

COPY_OUT_FILE     = 'copy output\\OLED_CLOCK_TH.hex run'
OUT_FILE_NAME     = 'OLED_CLOCK_TH.hex'

CHECKSUM_ALL      = 'checksum.exe OLED_CLOCK_TH.hex ALL'
CHECKSUM_CHECKSUM = 'checksum.exe OLED_CLOCK_TH.hex CHECKSUM'
CHECKSUM_CRC16    = 'checksum.exe OLED_CLOCK_TH.hex CRC16'
CHECKSUM_CRC32    = 'checksum.exe OLED_CLOCK_TH.hex CRC32'
CHECKSUM_SHA256   = 'checksum.exe OLED_CLOCK_TH.hex SHA256'

def rm_output():
    # 删除output文件夹
    name = "output"
    path = "./" + name
    if os.path.exists(path):
        shutil.rmtree(path)

def build():
    # 编译项目
    os.system(BUILD_COMMAND)

def log_file_print():
    # 输出编译log文件
    with open(BUILD_LOG_FILE, "r") as f:
        data = f.read()
        print(data)

def copy_hex_file():
    # 复制输出文件到run目录
    os.system(COPY_OUT_FILE)

def checksum_hex_file():
    # 切换目录到run
    path = "./run"
    os.chdir(path)

    # 输出文件需要的信息
    os.system(CHECKSUM_ALL)
    #os.system(CHECKSUM_CHECKSUM)
    #os.system(CHECKSUM_CRC16)
    #os.system(CHECKSUM_CRC32)
    #os.system(CHECKSUM_SHA256)

def main():
    start = datetime.datetime.now()

    rm_output()
    build()
    log_file_print()
    copy_hex_file()
    checksum_hex_file()

    end = datetime.datetime.now()
    print('run time: %s second' %(end - start))

if __name__ == "__main__":
    main()
//用于crc校验，采用crc8算法
//从串口接收到的数据，最后三个字节依次分别为CRC_SUM、0xFF、0xFF

#ifndef _CRC_H_
#define _CRC_H_

#include "hardware.h"

//校验接收信息是否完整，size专指去掉了结束字符(0xFF、0xFF)的大小
uchar CRC_is_ok(const uchar *ptr,  uint size);

#endif
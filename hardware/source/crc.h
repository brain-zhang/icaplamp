//����crcУ�飬����crc8�㷨
//�Ӵ��ڽ��յ������ݣ���������ֽ����ηֱ�ΪCRC_SUM��0xFF��0xFF

#ifndef _CRC_H_
#define _CRC_H_

#include "hardware.h"

//У�������Ϣ�Ƿ�������sizeרָȥ���˽����ַ�(0xFF��0xFF)�Ĵ�С
uchar CRC_is_ok(const uchar *ptr,  uint size);

#endif
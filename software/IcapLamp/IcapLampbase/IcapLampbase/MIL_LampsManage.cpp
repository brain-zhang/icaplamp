#include "MIL_LampsManage.h"


MIL_LampsManage::MIL_LampsManage(void)
{
}


MIL_LampsManage::~MIL_LampsManage(void)
{
}

MIL_Lamp MIL_LampsManage::GetLampByid(int id)
{
	//����
	return MIL_Lamp(1, "192.168.1.1");
}

MIL_Lamp MIL_LampsManage::GetLampByip(const string &ip)
{
	//����
	return MIL_Lamp(1, "192.168.1.1");
}
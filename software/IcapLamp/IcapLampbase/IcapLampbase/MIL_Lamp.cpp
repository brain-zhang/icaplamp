#include "MIL_Exception.h"
#include "MIL_Database.h"
#include "MIL_Lamp.h"

MIL_Lamp::MIL_Lamp(int lamp_id):_id(lamp_id)
{
	stringstream ss;
	ss<<" lamp_id:"<<lamp_id;

	MILDB_Instance.DBLamp_GetipByid(_id, _ip);
	if(!_ip.size())
	{
		//抛出指定id矿灯不存在的异常
		throw MIL_Exception()<<err_no(MILEXCE_DB_NOTHAVE_LIGHT)<<err_str(MILEXCE_DB_NOTHAVE_LIGHT_STR + ss.str());
	}
}

MIL_Lamp::~MIL_Lamp(void)
{
}

void MIL_Lamp::Serialize()
{

}

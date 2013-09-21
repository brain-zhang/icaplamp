/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   16:19
	filename: 	MIL_LampManages.h
	author:		张斌
	
	purpose:	添加/删除/获取信息 矿灯控制接口
*********************************************************************/
#ifndef MIL_LampsManage_h__
#define MIL_LampsManage_h__

#include "MIL_Lamp.h"
#include "MIL_BaseObject.h"

class MIL_LampsManage:
	public MIL_BaseObject
{
public:
	MIL_LampsManage(void);
	~MIL_LampsManage(void);

public:
	/*! \brief 根据矿灯编号获取矿灯对象
	 *	\param id 矿灯编号
	 *
	 *	如果数据库中没有此编号矿灯，抛出异常
	 */
	MIL_Lamp GetLampByid(int id);

	/*! \brief 根据矿灯IP获取矿灯对象
	 *	\param ip 矿灯IP
	 *
	 *	如果数据库中没有此IP矿灯，抛出异常
	 */
	MIL_Lamp GetLampByip(const string &ip);
};

#endif // MIL_LampManage_h__
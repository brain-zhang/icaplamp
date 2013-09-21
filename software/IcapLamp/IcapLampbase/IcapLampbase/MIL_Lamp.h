/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   12:09
	filename: 	MIL_Lamp.h
	author:		张斌
	
	purpose:	矿灯封转类，功能很简单，实现同工人的一对一关系
*********************************************************************/
#ifndef MIL_Lamp_h__
#define MIL_Lamp_h__

#include <iostream>
#include <string>
#include "MIL_BaseObject.h"

using namespace std;

class MIL_Lamp :
	public MIL_BaseObject
{
public:
	MIL_Lamp(){};
	/*! \brief 输入所有矿灯信息来构造对象
	 *	\param	lamp_id	矿灯编号
	 *	\param	ip		矿灯ip
	 *
	 *	此构造方式仅用于录入新矿灯场景
	 */
	MIL_Lamp(int lamp_id, const string &ip):_id(lamp_id),_ip(ip){};

	/*! \brief 根据矿灯编号构造对象
	 *	\param	lamp_id	矿灯编号
	 *
	 *	这种构造方式用于平时的查询或者临时对象使用
	 *	如果此编号的矿灯还没有录入数据库的话，会抛出一个异常
	 */
	MIL_Lamp(int lamp_id);

	~MIL_Lamp(void);

public:
	string Getip(){return _ip;}
	int Getid(){return _id;}
	void Serialize();

private:
	string _ip;	/*!< \brief 矿灯的ip地址，此信息唯一 */
	int _id; /*!< \brief 矿灯编号 */
};

#endif // MIL_Lamp_h__
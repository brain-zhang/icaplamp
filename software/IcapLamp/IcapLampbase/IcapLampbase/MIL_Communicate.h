/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:41
	filename: 	MIL_Communicate.h
	author:		张斌
	
	purpose:	用于上下位机的通讯
*********************************************************************/
#ifndef MIL_Communicate_h__
#define MIL_Communicate_h__

#include <iostream>
#include <string>
#include "MIL_BaseObject.h"

using namespace std;

class MIL_Communicate :
	public MIL_BaseObject
{
public:
	MIL_Communicate(void);
	~MIL_Communicate(void);

public:
	/*! \brief 通知下位机
	 *
	 * 向下位机发送信息 
	 * \param ip 发送对象的ip地址
	 * \param message 发送内容
	 */
	void SendMessage(string ip, string message);
};

#endif // MIL_Communicate_h__
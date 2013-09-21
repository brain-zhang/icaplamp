/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   16:19
	filename: 	MIL_LampManages.h
	author:		�ű�
	
	purpose:	���/ɾ��/��ȡ��Ϣ ��ƿ��ƽӿ�
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
	/*! \brief ���ݿ�Ʊ�Ż�ȡ��ƶ���
	 *	\param id ��Ʊ��
	 *
	 *	������ݿ���û�д˱�ſ�ƣ��׳��쳣
	 */
	MIL_Lamp GetLampByid(int id);

	/*! \brief ���ݿ��IP��ȡ��ƶ���
	 *	\param ip ���IP
	 *
	 *	������ݿ���û�д�IP��ƣ��׳��쳣
	 */
	MIL_Lamp GetLampByip(const string &ip);
};

#endif // MIL_LampManage_h__
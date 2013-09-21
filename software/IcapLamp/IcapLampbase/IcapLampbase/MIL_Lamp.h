/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   12:09
	filename: 	MIL_Lamp.h
	author:		�ű�
	
	purpose:	��Ʒ�ת�࣬���ܼܺ򵥣�ʵ��ͬ���˵�һ��һ��ϵ
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
	/*! \brief �������п����Ϣ���������
	 *	\param	lamp_id	��Ʊ��
	 *	\param	ip		���ip
	 *
	 *	�˹��췽ʽ������¼���¿�Ƴ���
	 */
	MIL_Lamp(int lamp_id, const string &ip):_id(lamp_id),_ip(ip){};

	/*! \brief ���ݿ�Ʊ�Ź������
	 *	\param	lamp_id	��Ʊ��
	 *
	 *	���ֹ��췽ʽ����ƽʱ�Ĳ�ѯ������ʱ����ʹ��
	 *	����˱�ŵĿ�ƻ�û��¼�����ݿ�Ļ������׳�һ���쳣
	 */
	MIL_Lamp(int lamp_id);

	~MIL_Lamp(void);

public:
	string Getip(){return _ip;}
	int Getid(){return _id;}
	void Serialize();

private:
	string _ip;	/*!< \brief ��Ƶ�ip��ַ������ϢΨһ */
	int _id; /*!< \brief ��Ʊ�� */
};

#endif // MIL_Lamp_h__
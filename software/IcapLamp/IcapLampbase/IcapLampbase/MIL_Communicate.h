/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:41
	filename: 	MIL_Communicate.h
	author:		�ű�
	
	purpose:	��������λ����ͨѶ
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
	/*! \brief ֪ͨ��λ��
	 *
	 * ����λ��������Ϣ 
	 * \param ip ���Ͷ����ip��ַ
	 * \param message ��������
	 */
	void SendMessage(string ip, string message);
};

#endif // MIL_Communicate_h__
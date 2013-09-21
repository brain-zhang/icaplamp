/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   12:40
	filename: 	MIL_Worker.h
	author:		�ű�
	
	purpose:	����������Ϊ��װ
*********************************************************************/
#ifndef MIL_Worker_h__
#define MIL_Worker_h__

#include <iostream>
#include <string>

#define BOOST_DATE_TIME_SOURCE
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "MIL_Communicate.h"
#include "MIL_BaseObject.h"
#include "MIL_Lamp.h"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace boost::local_time;

class MIL_Worker :
	public MIL_BaseObject
{
public:
	MIL_Worker(){};
	MIL_Worker(int worker_id, const string &name, int lamp_id);

	/*! \brief ����worker_id����woker����
	 *
	 *	������ݿ��в����ڴ�worker�Ļ���ֱ���׳��쳣
	 */
	MIL_Worker(int worker_id);
	~MIL_Worker(void);

public:
	/*! \brief ��˹��˷�����Ϣ
	 * \param message ��������
	 */
	void SendMessage(const string &message);

	/*! \brief ��ȡ���˹���
	 *
	 */
	int Getid() const {return _worker_id;};

	/*! \brief ��ȡ���˳��еĿ��
	 *
	 */
	MIL_Lamp GetLamp();

	/*! \brief ��������Ϣд�����ݿ�
	 *
	 */
	void Serialize();

public:
	/*! \brief ����״̬˵��
	 *
	 */
	enum WORKER_STATE{
		MINE_UP_STATE = 0,				/*!< \brief Ĭ�Ϲ���״̬�ھ��� */
		MINE_DOWN_STATE = 1,			/*!< \brief ����״̬ */
		MINE_GOING_UP_STATE = 2,		/*!< \brief �Ӿ���ͨ�����׵��Ͼ�����ʱ��Ϊ׼���Ͼ�״̬ */
		MINE_GOING_DOWN_STATE = 3,		/*!< \brief �Ӿ���ͨ�����ڵ��¾�����ʱ��Ϊ׼���¾�״̬ */
	};

private:
	int _worker_id, _group_id;
	MIL_Communicate _communicator;	/*!< \brief ͨѶ�ӿھۺ� */
	int _lamp_id;					/*!< \brief ���п�� */
	string _name;
	struct  worker_state_t
	{
		int state;					/*!< \brief ����״̬��Ĭ���ھ��� */
		int direction;				/*!< \brief ������ˣ��ھ��£���¼�������һ�ξ����ķ�վ��� */
		ptime recording_time;		/*!< \brief ��¼�˹���״̬ʱ��ʱ�� */
	}_state;						
};

#endif // MIL_Worker_h__
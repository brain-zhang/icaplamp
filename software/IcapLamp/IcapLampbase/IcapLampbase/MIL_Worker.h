/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   12:40
	filename: 	MIL_Worker.h
	author:		张斌
	
	purpose:	单个工人行为封装
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

	/*! \brief 根据worker_id构造woker对象
	 *
	 *	如果数据库中不存在此worker的话，直接抛出异常
	 */
	MIL_Worker(int worker_id);
	~MIL_Worker(void);

public:
	/*! \brief 向此工人发送消息
	 * \param message 发送内容
	 */
	void SendMessage(const string &message);

	/*! \brief 获取工人工号
	 *
	 */
	int Getid() const {return _worker_id;};

	/*! \brief 获取工人持有的矿灯
	 *
	 */
	MIL_Lamp GetLamp();

	/*! \brief 将工人信息写入数据库
	 *
	 */
	void Serialize();

public:
	/*! \brief 工人状态说明
	 *
	 */
	enum WORKER_STATE{
		MINE_UP_STATE = 0,				/*!< \brief 默认工人状态在井上 */
		MINE_DOWN_STATE = 1,			/*!< \brief 井下状态 */
		MINE_GOING_UP_STATE = 2,		/*!< \brief 从井下通过井底的上井监测点时，为准备上井状态 */
		MINE_GOING_DOWN_STATE = 3,		/*!< \brief 从井上通过井口的下井监测点时，为准备下井状态 */
	};

private:
	int _worker_id, _group_id;
	MIL_Communicate _communicator;	/*!< \brief 通讯接口聚合 */
	int _lamp_id;					/*!< \brief 持有矿灯 */
	string _name;
	struct  worker_state_t
	{
		int state;					/*!< \brief 工人状态，默认在井上 */
		int direction;				/*!< \brief 如果工人，在井下，记录工人最后一次经过的分站编号 */
		ptime recording_time;		/*!< \brief 记录此工人状态时的时间 */
	}_state;						
};

#endif // MIL_Worker_h__
/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   14:57
	filename: 	MIL_Database.h
	author:		张斌
	
	purpose:	封装系统所有的数据库操作，要求使用此类最好用单件，本系统中统一采用boost::pool::singleton_default
*********************************************************************/
#ifndef MIL_Database_h__
#define MIL_Database_h__

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <soci.h> //使用soci封装库
#include <sqlite3/soci-sqlite3.h>

//使用boost::pool::singleton,boost::smart_ptr
#include <boost/smart_ptr.hpp>
#include <boost/pool/detail/singleton.hpp>

using namespace std;
using namespace soci;
using boost::details::pool::singleton_default;

class MIL_Database
{
public:
	MIL_Database(void);
	~MIL_Database(void);

public:
	/*! \brief 记录发送的消息
	 *	\param set<int> 工人工号
	 *	\param message 消息内容
	 */
	void DBMessage_SetMessage(const set<int> &worker_ids_set, string message);
	
	/*! \brief 记录发送的消息(按组发送)
	 *	\param set<string> 组号
	 *	\param message 消息内容
	 */
	void DBMessage_SetMessage(int group, string message);

	//============================================================================

	/*! \brief 从LampDevice table中根据矿灯ip地址获取对应的矿灯编号
	 *	\param ip 矿灯ip地址
	 *
	 */
	void DBLamp_GetipByid(int id, string &ip);

	//============================================================================

	/*! \brief 从Worker table中获取指定worker_id持有的lamp_id
	 *	\param worker_id 工人工号
	 *
	 */
	int DBWorker_GetDeviceidByWorkerid(int worker_id);

	/*! \brief 从Worker table中获取指定工号的工人姓名
	 *	\param	worker_id
	 *
	 */
	string DBWorker_GetNameByWorkerid(int worker_id);

	/*! \brief 从Worker table中获取指定工号的工人信息
	 *	\param	worker_id
	 *
	 */
	void DBWorker_GetInfoByWorkerid(int worker_id, string &name, int &lamp_id, int &group_id);

	/*! \brief 写入Worker信息
	 *	\param	device_id	持有的矿灯id
	 *	\param	name		工人所属组别
	 */
	void DBWorker_SetWorkerInfo(int worker_id, int device_id, string name, int group_id);

	/*! \brief 从WorkerState table获取worker的状态
	 *
	 */
	void DBWorker_GetWorkerState(int worker_id, int &state, int &direction, string &recording_time);

	/*! \brief 设置worker状态
	 *
	 */
	void DBWorker_SetWorkerState(int worker_id, int state, int direction, const string recording_time);

	/*! \brief 获取所有工人id
	 *
	 */
	void DBWorker_GetAllWorkersId(vector<int> &worker_ids);

private:
	/*! \brief 为了配合单件类使用，在实际操作数据库的时候，必须先调用此函数初始化!!!
	 *
	 */
	void _init(void);

private:
	const size_t _pool_size;
	shared_ptr<connection_pool> _ppool;
	bool _is_init;
};

//单件类引用，直接使用MILDB_Instance作为访问入口
typedef singleton_default<MIL_Database> MILDB;
#define MILDB_Instance MILDB::instance()

#endif // MIL_Database_h__
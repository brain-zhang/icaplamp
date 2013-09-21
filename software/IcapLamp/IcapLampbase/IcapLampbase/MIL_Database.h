/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   14:57
	filename: 	MIL_Database.h
	author:		�ű�
	
	purpose:	��װϵͳ���е����ݿ������Ҫ��ʹ�ô�������õ�������ϵͳ��ͳһ����boost::pool::singleton_default
*********************************************************************/
#ifndef MIL_Database_h__
#define MIL_Database_h__

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <soci.h> //ʹ��soci��װ��
#include <sqlite3/soci-sqlite3.h>

//ʹ��boost::pool::singleton,boost::smart_ptr
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
	/*! \brief ��¼���͵���Ϣ
	 *	\param set<int> ���˹���
	 *	\param message ��Ϣ����
	 */
	void DBMessage_SetMessage(const set<int> &worker_ids_set, string message);
	
	/*! \brief ��¼���͵���Ϣ(���鷢��)
	 *	\param set<string> ���
	 *	\param message ��Ϣ����
	 */
	void DBMessage_SetMessage(int group, string message);

	//============================================================================

	/*! \brief ��LampDevice table�и��ݿ��ip��ַ��ȡ��Ӧ�Ŀ�Ʊ��
	 *	\param ip ���ip��ַ
	 *
	 */
	void DBLamp_GetipByid(int id, string &ip);

	//============================================================================

	/*! \brief ��Worker table�л�ȡָ��worker_id���е�lamp_id
	 *	\param worker_id ���˹���
	 *
	 */
	int DBWorker_GetDeviceidByWorkerid(int worker_id);

	/*! \brief ��Worker table�л�ȡָ�����ŵĹ�������
	 *	\param	worker_id
	 *
	 */
	string DBWorker_GetNameByWorkerid(int worker_id);

	/*! \brief ��Worker table�л�ȡָ�����ŵĹ�����Ϣ
	 *	\param	worker_id
	 *
	 */
	void DBWorker_GetInfoByWorkerid(int worker_id, string &name, int &lamp_id, int &group_id);

	/*! \brief д��Worker��Ϣ
	 *	\param	device_id	���еĿ��id
	 *	\param	name		�����������
	 */
	void DBWorker_SetWorkerInfo(int worker_id, int device_id, string name, int group_id);

	/*! \brief ��WorkerState table��ȡworker��״̬
	 *
	 */
	void DBWorker_GetWorkerState(int worker_id, int &state, int &direction, string &recording_time);

	/*! \brief ����worker״̬
	 *
	 */
	void DBWorker_SetWorkerState(int worker_id, int state, int direction, const string recording_time);

	/*! \brief ��ȡ���й���id
	 *
	 */
	void DBWorker_GetAllWorkersId(vector<int> &worker_ids);

private:
	/*! \brief Ϊ����ϵ�����ʹ�ã���ʵ�ʲ������ݿ��ʱ�򣬱����ȵ��ô˺�����ʼ��!!!
	 *
	 */
	void _init(void);

private:
	const size_t _pool_size;
	shared_ptr<connection_pool> _ppool;
	bool _is_init;
};

//���������ã�ֱ��ʹ��MILDB_Instance��Ϊ�������
typedef singleton_default<MIL_Database> MILDB;
#define MILDB_Instance MILDB::instance()

#endif // MIL_Database_h__
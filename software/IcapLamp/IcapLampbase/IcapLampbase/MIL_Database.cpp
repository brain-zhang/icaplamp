#include "MIL_Exception.h"
#include "MIL_Database.h"


MIL_Database::MIL_Database(void):_pool_size(100),_is_init(false)
{

}


MIL_Database::~MIL_Database(void)
{
}

void MIL_Database::_init()
{
	if(!_is_init)
	{
		try
		{
			_ppool = shared_ptr<connection_pool> (new connection_pool(_pool_size));
			for (size_t i = 0; i != _pool_size; ++i)
			{
				session &sql = _ppool->at(i);

				sql.open(sqlite3, "d:\\IcapLamp.db");
			}
		}
		catch(const exception &e)
		{
			cerr<<"Err:"<<e.what()<<endl;
		}
	}
}

void MIL_Database::DBMessage_SetMessage(int group, string message)
{
	//好吧，我承认很丑
	//如果有什么好办法能实现一个构造函数抛出异常的三方库封装类的单件模式，还得是线程安全的
	//比这个更简单的我暂时想不到了

	_init();
}

void MIL_Database::DBMessage_SetMessage(const set<int> &worker_ids_set, string message)
{
	_init();
}

void MIL_Database::DBLamp_GetipByid(int id, string &ip)
{
	_init();
	session s(*_ppool);
	s<<"select ip from LampDevice where _ID = :id", into(ip), use(id);
}

int MIL_Database::DBWorker_GetDeviceidByWorkerid(int worker_id)
{
	_init();
	session s(*_ppool);
	int lamp_id = 0;
	s<<"select device_id from Worker where _ID = :_id", into(lamp_id), use(worker_id);
	return lamp_id;
}

void MIL_Database::DBWorker_GetInfoByWorkerid(int worker_id, string &name, int &lamp_id, int &group_id)
{
	_init();
	session s(*_ppool);
	s<<"select device_id, name, group_id from Worker where _ID = :id", 
		use(worker_id), into(lamp_id), into(name), into(group_id);
}

void MIL_Database::DBWorker_SetWorkerInfo(int worker_id, int device_id, string name, int group_id)
{
	_init();
	session s(*_ppool);
	s<<"insert into Worker(_ID, device_id, name, group_id) values(:id, :d_id, :name, :g_id)", 
		use(worker_id), use(device_id), use(name), use(group_id);
}

string MIL_Database::DBWorker_GetNameByWorkerid(int worker_id)
{
	_init();
	session s(*_ppool);
	string name;
	s<<"select name from Worker where worker_id = :id", into(name), use(worker_id);
	return name;
}

void MIL_Database::DBWorker_GetWorkerState(int worker_id, int &state, int &direction, string &recording_time)
{
	_init();
	session s(*_ppool);
	s<<"select state, direction, recording_time from WorkerState where worker_id = :id", 
		into(state), into(direction), into(recording_time), use(worker_id);
}

void MIL_Database::DBWorker_GetAllWorkersId(vector<int> &worker_ids)
{
	_init();
	session s(*_ppool);
	rowset<int> rs = (s.prepare << "select _ID from Worker");

	for (rowset<int>::const_iterator it = rs.begin(); it != rs.end(); ++it)
	{
		worker_ids.push_back(*it);
	}
}

void MIL_Database::DBWorker_SetWorkerState(int worker_id, int state, int direction, const string recording_time)
{
	_init();
	session s(*_ppool);
	try
	{
		s<<"update WorkerState set state = :state, direction = :direction, recording_time = :recording_time where worker_id = :id",
			use(state), use(direction), use(recording_time), use(worker_id);
	}
	catch (const exception &e)
	{
		//如果没有此worker的记录，则插入一条新纪录
		s<<"insert into WorkerState(worker_id, state, recording_time) values(:worker_id, :state, :recording_time)", 
			use(worker_id), use(state), use(recording_time);
	}
}
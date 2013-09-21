#include <sstream>

//使用datetime库
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>

#include "MIL_Database.h"
#include "MIL_Exception.h"
#include "MIL_Lamp.h"
#include "MIL_Worker.h"

MIL_Worker::MIL_Worker(int worker_id, const string &name, int lamp_id):
						_worker_id(worker_id), _group_id(-1), _name(name), _lamp_id(lamp_id)
{
	//设置工人此时状态	
	_state.state = MINE_UP_STATE;
	_state.recording_time = second_clock::local_time();
}

MIL_Worker::MIL_Worker(int worker_id):_worker_id(worker_id)
{
	stringstream ss;
	ss<<" worker_id:"<<worker_id;

	MILDB_Instance.DBWorker_GetInfoByWorkerid(worker_id, _name, _lamp_id, _group_id);
	if(!_name.size())
	{
		throw MIL_Exception()<<err_no(MILEXCE_DB_NOTHAVE_WORKER)<<err_str(MILEXCE_DB_NOTHAVE_WORKER_STR + ss.str());
	}

	//从数据库中获取工人此时状态
	string recording_time;
	int state = MINE_UP_STATE;
	int direction = -1;
	MILDB_Instance.DBWorker_GetWorkerState(_worker_id, state, direction, recording_time);
	_state.state = state;
	_state.direction = direction;
	_state.recording_time = time_from_string(recording_time);
}

MIL_Worker::~MIL_Worker(void)
{
}

void MIL_Worker::SendMessage(const string &message)
{
	string ip = MIL_Lamp(_lamp_id).Getip();
	_communicator.SendMessage(ip, message);
}

MIL_Lamp MIL_Worker::GetLamp()
{
	return MIL_Lamp(_lamp_id);
}

void MIL_Worker::Serialize()
{
	try
	{
		MILDB_Instance.DBWorker_SetWorkerInfo(_worker_id, _lamp_id, _name, _group_id);
		MILDB_Instance.DBWorker_SetWorkerState(_worker_id, _state.state, _state.direction, to_simple_string(_state.recording_time));
	}
	catch(const std::exception &e)
	{
		throw MIL_Exception()<<err_no(MILEXCE_DB_WORKER_REPEAT)<<err_str(MILEXCE_DB_WORKER_REPEAT_STR);
	}
}
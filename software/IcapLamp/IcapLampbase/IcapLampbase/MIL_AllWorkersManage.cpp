#include "MIL_Database.h"
#include "MIL_Worker.h"
#include "MIL_AllWorkersManage.h"


MIL_AllWorkersManage::MIL_AllWorkersManage(void)
{
}


MIL_AllWorkersManage::~MIL_AllWorkersManage(void)
{
}

void MIL_AllWorkersManage::load()
{
	vector<int> worker_ids;
	MILDB_Instance.DBWorker_GetAllWorkersId(worker_ids);

	for (vector<int>::const_iterator it = worker_ids.begin(); it != worker_ids.end(); it++)
	{
		all_workers_manager.AddWorker(MIL_Worker(*it));
	}
}
#include <algorithm>
#include <iterator>
#include "MIL_WorkersManage.h"
#include "MIL_Database.h"


void MIL_WorkersManage::AddWorker(const MIL_Worker &worker)
{
	_workers_set.insert(worker);
}

void MIL_WorkersManage::AddWorker(const set<MIL_Worker, Comp> &workers_set)
{
	for (set<MIL_Worker, Comp>::const_iterator iter = workers_set.begin(); iter != workers_set.end(); ++iter)
	{
		AddWorker(*iter);
	}
}

void MIL_WorkersManage::RemoveWorker(const MIL_Worker &worker)
{
	_workers_set.erase(worker);
}

void MIL_WorkersManage::RemoveWorker(const set<MIL_Worker, Comp> &workers_set)
{
	for (set<MIL_Worker, Comp>::const_iterator iter = workers_set.begin(); iter != workers_set.end(); ++iter)
	{
		RemoveWorker(*iter);
	}
}

void MIL_WorkersManage::BroadCast(const string &message)
{
	/*
	set<MIL_Worker> worker_ids_intersection;

	//先求交集
	set_intersection(workers_set.begin(), workers_set.end(), _workers_set.begin(), _workers_set.end(), 
		insert_iterator<set<MIL_Worker> >(worker_ids_intersection, worker_ids_intersection.begin()));
	*/

	for (set<MIL_Worker, Comp>::iterator iter = _workers_set.begin(); iter != _workers_set.end(); iter++)
	{
		MIL_Worker(*iter).SendMessage(message);
	}
}

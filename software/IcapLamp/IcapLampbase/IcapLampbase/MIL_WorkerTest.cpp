/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   11:27
	filename: 	MIL_WorkerTest.cpp
	author:		张斌
	
	purpose:	用于测试类MIL_Worker
*********************************************************************/
#include <vector>

#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include "MIL_Exception.h"
#include "MIL_Worker.h"
#include "MIL_Database.h"
#include "MIL_AllWorkersManage.h"

using namespace boost;

//开始测试套件
BOOST_AUTO_TEST_SUITE(s_MIL_Worker)

//测试用例1
BOOST_AUTO_TEST_CASE(t_MIL_Worker)
{
	try
	{
		MIL_Worker worker(1);
		MIL_AllWorkersManage all_worker_manager;
		all_worker_manager.load();

		//BOOST_CHECK_NO_THROW(worker.Serialize());
		//BOOST_CHECK_NO_THROW(worker.SendMessage(string("hello")), boost::exception);
	}
	catch(const std::exception &e)
	{
		cout<<MILEXEC_WHAT(e)<<endl;
	}

/*
	set<int> worker_ids;
	worker_ids.insert(1);
	worker_ids.insert(2);
	worker_ids.insert(3);

	set<int> worker_broad_ids;
	worker_broad_ids.insert(2);
	worker_broad_ids.insert(4);

	MIL_WorkersManage downhole_workers(worker_ids);
	downhole_workers.BroadCast("careful safe working", worker_broad_ids);
*/
}

BOOST_AUTO_TEST_SUITE_END()
//结束测试套件
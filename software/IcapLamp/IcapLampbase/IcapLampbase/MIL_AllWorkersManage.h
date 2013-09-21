/********************************************************************
	created:	2012/10/04
	created:	4:10:2012   14:48
	filename: 	MIL_AllWorkersManage.h
	author:		张斌
	
	purpose:	实现本系统所有工人的状态管理
*********************************************************************/
#ifndef MIL_AllWorkersManage_h__
#define MIL_AllWorkersManage_h__

#include <vector>

#include "MIL_WorkersManage.h"
#include "mil_baseobject.h"

using namespace std;

/*! \brief 所有工人的管理，最主要的接口实现
 *
 */
class MIL_AllWorkersManage :
	public MIL_BaseObject
{
public:
	MIL_AllWorkersManage(void);
	virtual ~MIL_AllWorkersManage(void);

public:
	/*! \brief 载入当前系统中所有工人信息，每次系统启动时都要调用此函数初始化
	 *
	 *  如果修改了数据库工人信息(录入新工人，删除工人等)，务必调用此函数重新初始化
	 */
	void load();

	/*! \brief 更新worker状态
	 *
	 */
	void UpdateWorkerState(int workerid, int state, int direction);

private:
	MIL_WorkersManage all_workers_manager;
};

#endif // MIL_AllWorkersManage_h__

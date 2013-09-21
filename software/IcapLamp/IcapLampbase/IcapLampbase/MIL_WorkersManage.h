/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   14:13
	filename: 	MIL_WorkersManage.h
	author:		张斌
	
	purpose:	
*********************************************************************/
#ifndef MIL_WorkersManage_h__
#define MIL_WorkersManage_h__

#include <iostream>
#include <string>
#include <set>
#include <map>
#include "MIL_Worker.h"
#include "MIL_BaseObject.h"

using namespace std;

/*! \brief 用于提供对关联容器的支持
	*
	*/
inline bool worker_compare(const MIL_Worker &lhs, const MIL_Worker &rhs)
{
	return lhs.Getid()< rhs.Getid();
}

/*! \brief 工人集合管理类
 *
 * 对外提供工人的通讯、定位、考勤等管理接口
 */
class MIL_WorkersManage :
	public MIL_BaseObject
{
public:
	/*!< \brief 类型别名 */
	typedef bool (*Comp)(const MIL_Worker &lhs, const MIL_Worker &rhs);


	MIL_WorkersManage(void):_workers_set(worker_compare){};

	/*! \brief 通过工人集合构建管理对象
	 *	\param workers_set 工人工号集合
	 */
	MIL_WorkersManage(set<MIL_Worker, Comp> &workers_set):_workers_set(workers_set){};
	~MIL_WorkersManage(void){};

public:
	/*! \brief 向管理集合中增加一个工人
	 *	\param worker_id 工人工号
	 */
	void AddWorker(const MIL_Worker &worker);

	/*! \brief 向管理集合中增加多个工人(函数重载)
	 */
	void AddWorker(const set<MIL_Worker, Comp> &workers_set);

	/*! \brief 从管理集合中移除一个工人
	 *	\param worker_id 工人工号
	 */
	void RemoveWorker(const MIL_Worker &worker);

	/*! \brief 从管理集合中移除多个工人(函数重载)
	 */
	void RemoveWorker(const set<MIL_Worker, Comp> &workers_set);

	/*! \brief 从管理集合中返回一个工人
	 *
	 */
	//MIL_Worker *GetWorker(int worker_id);

	/*! \brief 向管理集合中的工人发送信息
	 *	\param message 发送内容
	 */
	void BroadCast(const string &message);

private:
	set<MIL_Worker, Comp> _workers_set;
};

#endif // MIL_WorkersManage_h__

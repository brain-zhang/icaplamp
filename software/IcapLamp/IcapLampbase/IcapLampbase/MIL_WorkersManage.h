/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   14:13
	filename: 	MIL_WorkersManage.h
	author:		�ű�
	
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

/*! \brief �����ṩ�Թ���������֧��
	*
	*/
inline bool worker_compare(const MIL_Worker &lhs, const MIL_Worker &rhs)
{
	return lhs.Getid()< rhs.Getid();
}

/*! \brief ���˼��Ϲ�����
 *
 * �����ṩ���˵�ͨѶ����λ�����ڵȹ���ӿ�
 */
class MIL_WorkersManage :
	public MIL_BaseObject
{
public:
	/*!< \brief ���ͱ��� */
	typedef bool (*Comp)(const MIL_Worker &lhs, const MIL_Worker &rhs);


	MIL_WorkersManage(void):_workers_set(worker_compare){};

	/*! \brief ͨ�����˼��Ϲ����������
	 *	\param workers_set ���˹��ż���
	 */
	MIL_WorkersManage(set<MIL_Worker, Comp> &workers_set):_workers_set(workers_set){};
	~MIL_WorkersManage(void){};

public:
	/*! \brief �������������һ������
	 *	\param worker_id ���˹���
	 */
	void AddWorker(const MIL_Worker &worker);

	/*! \brief ������������Ӷ������(��������)
	 */
	void AddWorker(const set<MIL_Worker, Comp> &workers_set);

	/*! \brief �ӹ��������Ƴ�һ������
	 *	\param worker_id ���˹���
	 */
	void RemoveWorker(const MIL_Worker &worker);

	/*! \brief �ӹ��������Ƴ��������(��������)
	 */
	void RemoveWorker(const set<MIL_Worker, Comp> &workers_set);

	/*! \brief �ӹ������з���һ������
	 *
	 */
	//MIL_Worker *GetWorker(int worker_id);

	/*! \brief ��������еĹ��˷�����Ϣ
	 *	\param message ��������
	 */
	void BroadCast(const string &message);

private:
	set<MIL_Worker, Comp> _workers_set;
};

#endif // MIL_WorkersManage_h__

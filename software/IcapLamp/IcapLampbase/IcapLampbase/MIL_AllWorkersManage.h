/********************************************************************
	created:	2012/10/04
	created:	4:10:2012   14:48
	filename: 	MIL_AllWorkersManage.h
	author:		�ű�
	
	purpose:	ʵ�ֱ�ϵͳ���й��˵�״̬����
*********************************************************************/
#ifndef MIL_AllWorkersManage_h__
#define MIL_AllWorkersManage_h__

#include <vector>

#include "MIL_WorkersManage.h"
#include "mil_baseobject.h"

using namespace std;

/*! \brief ���й��˵Ĺ�������Ҫ�Ľӿ�ʵ��
 *
 */
class MIL_AllWorkersManage :
	public MIL_BaseObject
{
public:
	MIL_AllWorkersManage(void);
	virtual ~MIL_AllWorkersManage(void);

public:
	/*! \brief ���뵱ǰϵͳ�����й�����Ϣ��ÿ��ϵͳ����ʱ��Ҫ���ô˺�����ʼ��
	 *
	 *  ����޸������ݿ⹤����Ϣ(¼���¹��ˣ�ɾ�����˵�)����ص��ô˺������³�ʼ��
	 */
	void load();

	/*! \brief ����worker״̬
	 *
	 */
	void UpdateWorkerState(int workerid, int state, int direction);

private:
	MIL_WorkersManage all_workers_manager;
};

#endif // MIL_AllWorkersManage_h__

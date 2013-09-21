/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:17
	filename: 	MIL_BaseObject.h
	author:		�ű�
	
	purpose:	��Ŀ���࣬�ṩ���л�����־����
*********************************************************************/
#ifndef MIL_BaseObject_h__
#define MIL_BaseObject_h__

class MIL_BaseObject
{
protected:
	MIL_BaseObject(void);
	virtual ~MIL_BaseObject(void);

protected:
	/*! \brief ��ӡ��־����
	 *
	 * ������ʵ����־��ӡ����
	 */
	virtual void ReportEvent();

	/*! \brief ���л�����
	 *
	 * ������������ݿ�ӿ�ʵ�����л�
	 */
	virtual void Serialize();
};

#endif // MIL_BaseObject_h__
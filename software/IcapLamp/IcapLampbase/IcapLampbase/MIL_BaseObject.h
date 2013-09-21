/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:17
	filename: 	MIL_BaseObject.h
	author:		张斌
	
	purpose:	项目基类，提供序列化及日志功能
*********************************************************************/
#ifndef MIL_BaseObject_h__
#define MIL_BaseObject_h__

class MIL_BaseObject
{
protected:
	MIL_BaseObject(void);
	virtual ~MIL_BaseObject(void);

protected:
	/*! \brief 打印日志功能
	 *
	 * 派生类实现日志打印功能
	 */
	virtual void ReportEvent();

	/*! \brief 序列化功能
	 *
	 * 派生类调用数据库接口实现序列化
	 */
	virtual void Serialize();
};

#endif // MIL_BaseObject_h__
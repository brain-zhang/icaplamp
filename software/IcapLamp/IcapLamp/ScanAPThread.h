#pragma once

#include <iostream>
#include "Easyssh.h"

#define WM_STATUS_MESSAGE WM_USER + 1000

//用于在后台，定时扫描所有设备是否可以访问
//能访问的设备在数据库中做标记

class CSCanAPThread : public CWinThread
{
	DECLARE_DYNCREATE(CSCanAPThread)

public:	
	CSCanAPThread();
	CSCanAPThread(std::string ip);           // 动态创建所使用的受保护的构造函数
	virtual ~CSCanAPThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int CSCanAPThread::Run();

private:
	void DaemonDeviceScan(void);

private:
	UINT m_scanTimeInterval;
	CEasyssh m_ssh;
	bool m_bIsConnect;
	std::string m_strIP;

protected:
	DECLARE_MESSAGE_MAP()
};



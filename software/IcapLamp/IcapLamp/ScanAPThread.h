#pragma once

#include <iostream>
#include "Easyssh.h"

#define WM_STATUS_MESSAGE WM_USER + 1000

//�����ں�̨����ʱɨ�������豸�Ƿ���Է���
//�ܷ��ʵ��豸�����ݿ��������

class CSCanAPThread : public CWinThread
{
	DECLARE_DYNCREATE(CSCanAPThread)

public:	
	CSCanAPThread();
	CSCanAPThread(std::string ip);           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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



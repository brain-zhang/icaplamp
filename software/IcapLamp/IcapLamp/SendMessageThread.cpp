// SendMessageThread.cpp : 实现文件
//

#include "stdafx.h"

#include <iostream>

#include "SendMessageThread.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Model.h"


// CSendMessageThread

IMPLEMENT_DYNCREATE(CSendMessageThread, CWinThread)

CSendMessageThread::CSendMessageThread()
{
}

CSendMessageThread::~CSendMessageThread()
{
}

BOOL CSendMessageThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CSendMessageThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

//线程持续运行并相应消息
int CSendMessageThread::Run()
{
	MSG   msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
		case WM_SENDMESSAGEDAEMON:
			SendMessageDaemon(m_message);
			break;
		default:
			break;
		}
	}
	return   CWinThread::Run();
} 

void CSendMessageThread::SetMessage(char *message)
{
	m_message = std::string(message);
}

//向所有设备发送消息
void CSendMessageThread::SendMessageDaemon(std::string &message)
{
	/*
	CModel *pmodel = CSingleton<CModel>::instance();
	std::vector<std::string> vec_ip_pool = pmodel->GetDeviceIPList();

	for (std::vector<std::string>::iterator iter = vec_ip_pool.begin(); iter != vec_ip_pool.end(); iter++)
	{
		m_sockSendMessage.SendMessage(*iter, m_message);
	}*/
	m_sockSendMessage.SendMessage("192.168.1.19", m_message);
}

BEGIN_MESSAGE_MAP(CSendMessageThread, CWinThread)
END_MESSAGE_MAP()


// CSendMessageThread 消息处理程序
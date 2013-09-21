// DaemonThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <iostream>
#include "Resource.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Util.h"
#include "ScanAPThread.h"


// CSCanAPThread

IMPLEMENT_DYNCREATE(CSCanAPThread, CWinThread)

CSCanAPThread::CSCanAPThread()
{
}

CSCanAPThread::CSCanAPThread(std::string ip):m_scanTimeInterval(5000), m_ssh(ip), m_strIP(ip)
{
}

CSCanAPThread::~CSCanAPThread()
{
}

BOOL CSCanAPThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	::SetTimer(NULL, NULL, m_scanTimeInterval, NULL);
	m_bIsConnect = false;
	return TRUE;
}

int CSCanAPThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

int CSCanAPThread::Run()
{
	if(!m_bIsConnect)
	{
		m_ssh.ConnectAP();
		m_bIsConnect = true;
	}

	MSG   msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		CString trace_msg;
		trace_msg.Format(_T("GetMessage.........%d\n"), msg.message);
		TRACE(trace_msg);
		switch(msg.message)
		{
		case WM_TIMER:
			DaemonDeviceScan();
			
			if (m_bIsConnect)
			{
				//�������ڷ�����Ϣ������״̬��
				::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_STATUS_MESSAGE, (WPARAM)m_strIP.c_str(), 0);
			}
			else
			{
				::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_STATUS_MESSAGE, (WPARAM)("Error"), (WPARAM)m_strIP.c_str());
			}
			break;
		case WM_QUIT:
			m_ssh.DisconnectAP();
			ExitThread(0);
			break;
		default:
			break;
		}
	}
	return   CWinThread::Run();
} 

void CSCanAPThread::DaemonDeviceScan()
{
	//ÿ��һ��ʱ�䣬ɨ��ָ�����豸����ȡip
	std::string str_stalist = m_ssh.GetDeviceMAC();

	if(str_stalist.c_str() == std::string("Error"))
	{
		m_bIsConnect = false;
		m_ssh.ConnectAP();
		return;
	}
	else
	{
		m_bIsConnect = true;
	}

	//�ȷָ��ȡMAC
	std::vector<std::string> vec_list;
	CUtil::split(str_stalist, vec_list, "\n");

	if (vec_list.size() < 2)
	{
		return;
	}

	//ȥ��������Ϣ
	vec_list.erase(vec_list.begin());
	vec_list.erase(vec_list.end() - 1);
	
	int i = 0;
	for (std::vector<std::string>::iterator iter = vec_list.begin(); iter != vec_list.end(); iter++)
	{
		if (i % 4 == 0)
		{
			//д�����ݿ�
			CModel *pmodel = CSingleton<CModel>::instance();
			std::string mac((*iter), 0, 17);

			//��ȡdeviceID
			int deviceID = pmodel->GetDeviceIDbyMAC(mac);

			if (deviceID > 0)
			{
				//��ȡAPID
				int APID = pmodel->GetAPIDbyIP(m_strIP);

				Field definition_tbMonitor[] = 
				{
					Field(FIELD_KEY),
					Field("DeviceID", type_int),
					Field("APID", type_int),
					Field(DEFINITION_END),
				};
				Table tbMonitor(pmodel->MGetDatabase(), std::string("DirectionMonitor"), definition_tbMonitor);
				tbMonitor.open();
				Record record(tbMonitor.fields());
				record.setInteger("DeviceID", deviceID);
				record.setInteger("APID", APID);
				tbMonitor.addRecord(&record);
			}
		}
		i++;
	}
}


BEGIN_MESSAGE_MAP(CSCanAPThread, CWinThread)
END_MESSAGE_MAP()


// CSCanAPThread ��Ϣ�������

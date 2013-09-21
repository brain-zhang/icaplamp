#include <winsock2.h>

#include "stdafx.h"
#include "SockSendMessage.h"

CSockSendMessage::CSockSendMessage(void)
{
	LPWSADATA lpmyWSAData;
	WORD   VersionReqd;
	VersionReqd = WSA_MAKEWORD(MAJOR_VERSION,MINOR_VERSION);
	lpmyWSAData = (LPWSADATA)malloc(sizeof(WSADATA));
	int Status = WSAStartup(VersionReqd, lpmyWSAData);
	if (Status != 0)
	{
		AfxMessageBox(_T("��������ʧ��!"));
	}
}

CSockSendMessage::~CSockSendMessage(void)
{
	WSACleanup();
}

int CSockSendMessage::SendMessage(string ip, string content) 
{
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERVER_PORT);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	//����socket�ӿ�
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(s == INVALID_SOCKET)
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		return 1;
	}

	/*
	int nTimeout = 5000;  
	//���÷��ͳ�ʱΪ1000ms  
	::setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeout, sizeof(int));

	//���ý��ճ�ʱΪ1000ms  
	::setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeout, sizeof(int));
	
	//���÷�������ʽ����
	unsigned long ul = 1;
	::ioctlsocket(s,   FIONBIO,   (unsigned   long*)&ul);
	*/
	TRACE(_T("Send Message\n"));
	if(::connect(s, (sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
	{
		string strErr = string("����ʧ��");
		strErr += ip;
		AfxMessageBox(CString(strErr.c_str()));
		return 1;
	}
	Sleep(100);
	::send(s, content.c_str(), content.length(), 0);
	::closesocket(s);
	return 0;
}
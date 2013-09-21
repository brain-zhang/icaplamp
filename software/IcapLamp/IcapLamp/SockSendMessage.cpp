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
		AfxMessageBox(_T("建立连接失败!"));
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

	//建立socket接口
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(s == INVALID_SOCKET)
	{
		AfxMessageBox(_T("发送失败！"));
		return 1;
	}

	/*
	int nTimeout = 5000;  
	//设置发送超时为1000ms  
	::setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeout, sizeof(int));

	//设置接收超时为1000ms  
	::setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeout, sizeof(int));
	
	//设置非阻塞方式连接
	unsigned long ul = 1;
	::ioctlsocket(s,   FIONBIO,   (unsigned   long*)&ul);
	*/
	TRACE(_T("Send Message\n"));
	if(::connect(s, (sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
	{
		string strErr = string("连接失败");
		strErr += ip;
		AfxMessageBox(CString(strErr.c_str()));
		return 1;
	}
	Sleep(100);
	::send(s, content.c_str(), content.length(), 0);
	::closesocket(s);
	return 0;
}
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include "NetUtil.h"

bool CNetUtil::Ping(const char *pIPAddr)
{
	HANDLE iHwnd;
	iHwnd = IcmpCreateFile();
	IPAddr pAddr;
	pAddr = (IPAddr)(inet_addr(pIPAddr));
	icmp_echo_reply pData;
	IcmpSendEcho(iHwnd,pAddr,NULL,0,NULL,(LPVOID)&pData,sizeof(icmp_echo_reply),0);
	IcmpCloseHandle(iHwnd);
	return !pData.Status;
}

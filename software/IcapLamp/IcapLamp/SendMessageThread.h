#include <iostream>
#include "SockSendMessage.h"

#define WM_SENDMESSAGEDAEMON	WM_USER + 1

// CSendMessageThread

class CSendMessageThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendMessageThread)

public:
	CSendMessageThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSendMessageThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int CSendMessageThread::Run();
	void SetMessage(char *message);

private:
	void SendMessageDaemon(std::string &message);	

private:
	CSockSendMessage m_sockSendMessage;
	std::string m_message;

protected:
	DECLARE_MESSAGE_MAP()
};



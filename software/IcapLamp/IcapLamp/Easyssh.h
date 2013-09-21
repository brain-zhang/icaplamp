#pragma once

#include <iostream>
#include "libssh2/libssh2_config.h"
#include "libssh2/libssh2.h"

// CEasyssh

class CEasyssh : public CWnd
{
	DECLARE_DYNAMIC(CEasyssh)

public:
	CEasyssh();
	CEasyssh(const std::string &str_ip);
	virtual ~CEasyssh();
	void DisconnectAP();
	void ConnectAP(const std::string str_usr = std::string("ubnt"), std::string str_passwd = std::string("ubnt"));
	std::string GetDeviceMAC(void);

protected:
	DECLARE_MESSAGE_MAP()

private:
	int waitsocket(int socket_fd, LIBSSH2_SESSION *session);

private:
	LIBSSH2_SESSION *m_ssh_session;
	bool m_bIsErr;
	std::string m_strIP;
	int m_sock;
	
};



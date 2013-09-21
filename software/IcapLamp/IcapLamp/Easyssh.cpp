// Easyssh.cpp : 实现文件
//

#include "stdafx.h"
#include "IcapLamp.h"
#include "Easyssh.h"


// CEasyssh

IMPLEMENT_DYNAMIC(CEasyssh, CWnd)

CEasyssh::CEasyssh()
{

}

CEasyssh::CEasyssh(const std::string &str_ip):m_bIsErr(false), m_strIP(str_ip)
{

}

CEasyssh::~CEasyssh()
{

}


BEGIN_MESSAGE_MAP(CEasyssh, CWnd)
END_MESSAGE_MAP()



// CEasyssh 消息处理程序


void CEasyssh::DisconnectAP()
{
	libssh2_session_disconnect(m_ssh_session, "Normal Shutdown, Thank you for playing");
	libssh2_session_free(m_ssh_session);
	closesocket(m_sock);
	libssh2_exit();
}

void CEasyssh::ConnectAP(const std::string str_usr, std::string str_passwd)
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,0), &wsadata);

	int rc = libssh2_init(0);

	if (rc != 0) {
		TRACE(_T("libssh2_init Error\n"));
		m_bIsErr = true;
		return;
	}

	unsigned long hostaddr = inet_addr(m_strIP.c_str());

	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(m_sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
		TRACE(_T("connect socket Error\n"));
		m_bIsErr = true;
		return;
	}

	/* Create a session instance */ 
	m_ssh_session = libssh2_session_init();
	if (!m_ssh_session)
	{
		TRACE(_T("libssh2_session_init Error\n"));
		m_bIsErr = true;
		return;
	}

    /* tell libssh2 we want it all done non-blocking */ 
    libssh2_session_set_blocking(m_ssh_session, 0);

 
    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */ 
    while ((rc = libssh2_session_handshake(m_ssh_session, m_sock)) == LIBSSH2_ERROR_EAGAIN);
    if (rc) {
		TRACE(_T("libssh2_session_handshake Error\n"));
        m_bIsErr = true;
        return;
    }
 
    LIBSSH2_KNOWNHOSTS *nh = libssh2_knownhost_init(m_ssh_session);

    if(!nh) {
        /* eeek, do cleanup here */ 
		TRACE(_T("libssh2_knownhost_init Error\n"));
		m_bIsErr = true;
        return;
    }
 
    /* read all hosts from here */ 
    libssh2_knownhost_readfile(nh, "known_hosts", LIBSSH2_KNOWNHOST_FILE_OPENSSH);
 
    /* store all known hosts to here */ 
    libssh2_knownhost_writefile(nh, "dumpfile",LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    libssh2_knownhost_free(nh);

	while ((rc = libssh2_userauth_password(m_ssh_session, str_usr.c_str(), str_passwd.c_str())) == LIBSSH2_ERROR_EAGAIN);
	if (rc) {
		TRACE(_T("libssh2_userauth_password Error\n"));
		m_bIsErr = true;
		libssh2_session_disconnect(m_ssh_session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(m_ssh_session);
		closesocket(m_sock);
		libssh2_exit();
	}
	m_bIsErr = false;
}

//获取此分站范围内的设备IP
std::string CEasyssh::GetDeviceMAC(void)
{
	if (m_bIsErr)
	{
		//自动重连
		ConnectAP();
		return std::string("Error");
	}

	std::string str_ip_pool = "";

	char *exitsignal=(char *)"none";
	int rc = 0;
	const char *commandline = "/usr/www/wstalist ath0";
	/* Exec non-blocking on the remove host */ 
	LIBSSH2_CHANNEL *channel;

    while( (channel = libssh2_channel_open_session(m_ssh_session)) == NULL && 
			libssh2_session_last_error(m_ssh_session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN)
    {
        waitsocket(m_sock, m_ssh_session);
    }
    if(channel == NULL)
    {
		TRACE(_T("libssh2_channel_open_session Error\n"));
		return std::string("Error");
    }
    while( (rc = libssh2_channel_exec(channel, commandline)) == LIBSSH2_ERROR_EAGAIN)
    {
        waitsocket(m_sock, m_ssh_session);
    }
    if(rc != 0)
    {
		return std::string("Error");
    }
    for( ;; )
    {
        /* loop until we block */ 
        int rc;
        do
        {
            char buffer[0x4000];
            rc = libssh2_channel_read(channel, buffer, sizeof(buffer));

            if(rc > 0)
            {
				std::string str_temp(buffer, 0, rc);
				str_ip_pool += str_temp;
            }
        }
        while( rc > 0 );
 
        /* this is due to blocking that would occur otherwise so we loop on
           this condition */ 
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(m_sock, m_ssh_session);
        }
        else
            break;
    }

    while( (rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN )
        waitsocket(m_sock, m_ssh_session);
 
    if( rc == 0 )
    {
        libssh2_channel_get_exit_status(channel);
        libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
    }
 
    libssh2_channel_free(channel);
    channel = NULL;
	return str_ip_pool;
}

int CEasyssh::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */ 
	dir = libssh2_session_block_directions(session);


	if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}
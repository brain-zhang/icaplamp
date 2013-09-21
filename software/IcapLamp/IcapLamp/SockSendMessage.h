#pragma once

#include <iostream>
using namespace std;

class CSockSendMessage
{

#define MAJOR_VERSION 1
#define MINOR_VERSION 2
#define WSA_MAKEWORD(x,y)  ((y)*256+(x))
#define SERVER_PORT 8899

public:
	CSockSendMessage(void);
	~CSockSendMessage(void);

	int SendMessage(string ip, string content);
};

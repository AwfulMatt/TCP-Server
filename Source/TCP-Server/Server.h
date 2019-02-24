#pragma once
#include "pch.h"

struct Connection_t {
	SOCKET cSocket;
	sockaddr_in addrInfo;
};

class Server
{
	public:
		Server();
};
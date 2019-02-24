#include "pch.h"

int main()
{
	/* Activate winsocket */
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);
	int wsDataResult = WSAStartup(version, &wsData);
	if (wsDataResult != 0)
	{
		std::cout << "Windows library failed to initialize." << std::endl;
		return 0;
	}

	Server * server = new Server;
}
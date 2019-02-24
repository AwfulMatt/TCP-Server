#include "pch.h"

#define PORT 9977

Server::Server()
{
	/* 
	Connections stores all connections made as we need a way to 
	track how many connections there is and allows the server to 
	send bytes to them. 
	*/
	std::vector<Connection_t *> connections;

	/* Creates the socket to listen to the port */
	SOCKET socketListen = socket(AF_INET, SOCK_STREAM, 0);
	if (socketListen == INVALID_SOCKET)
	{
		std::cout << "Error : Could not create socket!" << std::endl;
		return;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	/* Bind the socket to port 9977 */
	bind(socketListen, (sockaddr*)&address, sizeof(address));

	/* Tell the socket to start listening for connections, SOMAXCONN allows max bytes to be sent from the connection! */
	listen(socketListen, SOMAXCONN);

	std::cout << "Server initialized successfully!" << std::endl;
	std::cout << "Awaiting client connection..." << std::endl;

	/* Client info will store ip and port of connected client! */
	sockaddr_in clientInfo;
	int clientSize = sizeof(clientInfo);

	/* While loop waits for connections to be made, everytime a connection is found it executes whats in the loop */
	SOCKET clientSocket;
	while ((clientSocket = accept(socketListen, (sockaddr *)&clientInfo, &clientSize)) != 0)
	{
		Connection_t * connection = new Connection_t{ clientSocket, clientInfo };
		connections.push_back(connection);
	}
}
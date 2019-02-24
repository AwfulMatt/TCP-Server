#include "pch.h"

#define PORT 54000

void handleConnection(Connection_t * connection);
int findConnection(Connection_t * conn);

std::vector<Connection_t *> connections;

Server::Server()
{
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

	std::cout << std::time(0) << "Server initialized successfully!" << std::endl;
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

		std::thread connectionThread(handleConnection, connection);
		connectionThread.detach();

		clientSocket = NULL;
	}
}

void handleConnection(Connection_t * connection)
{
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&connection->addrInfo, sizeof(connection->addrInfo), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &connection->addrInfo.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(connection->addrInfo.sin_port) << std::endl;
	}

	char receiveBuffer[200];

	while (true)
	{
		ZeroMemory(receiveBuffer, 200);

		/* Waits here for client to send another message! */
		int bytesReceived = recv(connection->cSocket, receiveBuffer, 100, 0);

		/* Connection died! */
		if (bytesReceived == 0 || bytesReceived == SOCKET_ERROR)
		{
			std::cout << host << " has disconnected!" << std::endl;
			connections.erase(connections.begin() + findConnection(connection));
			break;
		}

		/* Print to server what the client wrote! */
		std::cout << host << " : " << receiveBuffer << std::endl;

		/* Tell all clients what the other client wrote! */
		for (int i = 0; i < connections.size(); i++)
		{
			/* Don't send the message to the client it already knows! */
			if (connections[i] == connection) return;
			send(connections[i]->cSocket, receiveBuffer, sizeof(receiveBuffer), 0);
		}
	}
}

int findConnection(Connection_t * conn)
{
	int found = 0;
	for (int i = 0; i < connections.size(); i++)
	{
		//Two addresses match!
		if (connections[i] == conn)
		{
			found = i;
			break;
		}
	}

	return found;
}
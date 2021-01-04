#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main() {


	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		WSACleanup();
		return;
	}

	//Bind the an ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Not Accepted!"<< endl;
		WSACleanup();
		return;
	}

	//Clients remote name
	char host[NI_MAXHOST]; 
	//Service (port) the client is connected on
	char service[NI_MAXSERV]; 


	ZeroMemory(host, NI_MAXHOST); 
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "Connected on port: " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "Connected on port: " << ntohs(client.sin_port) << endl;
	}
	//Close listening socket
	closesocket(listening);
	//While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);
		//Wait for client to send data
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;

		break;
		}

		cout << string(buf, 0, bytesRecieved) << endl;

		//Echo message back to client
		//send(clientSocket, buf, bytesRecieved + 1, 0);
		
	}
	//Close the socket
	closesocket(clientSocket);

	//Cleanup winsock
	WSACleanup();
}
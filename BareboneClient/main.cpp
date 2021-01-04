#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main() {
	//Ipadress of the server
	string ipAddress = "127.0.0.1"; 
	//Listening port of the server
	int port = 54000;				
	
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "Can't initialize winsock! Error # " << wsok << endl;
		return;
	}

	//Create Socket
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Error # " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	

	//Connect to server
	int connResult = connect(clientSock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, ERR # " << WSAGetLastError() << endl;
		closesocket(clientSock);
		WSACleanup();
	}

	//Do-while loop to send and recieve data
	char buf[4096];
	string userInput;

	do {
		//Ask user for input
		cout << "> ";
		getline(cin, userInput);

		//Check if something is typed
		if (userInput.size() > 0) 
		{
			//Send the text to the server
			int sendResult = send(clientSock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) 
			{
				//Wait for response
				ZeroMemory(buf, 4096);
				int bytesRecieved = recv(clientSock, buf, 4096, 0);
				if (bytesRecieved > 0) 
				{
					//Echo response to console
					cout << "SERVER> " << string(buf, 0, bytesRecieved) << endl;
				}
			}
		
		}
		
	} while (userInput.size() > 0);

	//Close down everything
	closesocket(clientSock);
	WSACleanup();
}
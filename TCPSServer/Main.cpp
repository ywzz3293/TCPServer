#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")


using namespace std;

void main() {
	//init winsocket
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0)
	{
		cerr << "cannot init winsock" << endl;
		return;
	}

	//create winsock
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "cannot create winsock" << endl;
		return;
	}

	//Bind ip address and port to socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;	//could alsso ussse inet__pton...

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//wait for connection
	sockaddr_in client;
	int clienSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clienSize);
	//if(clientSocket) error

	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXHOST]; //sservice(port) client is connect on

	ZeroMemory(host, NI_MAXHOST); //same as meme_set(host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" <<
			ntohs(client.sin_port) << endl;
	}

	//close listening ocket
	closesocket(listening);

	//while loop:acceptt and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);
		
		//wait for client to send data
		int byteRecevied = recv(clientSocket, buf, 4096, 0);
		if (byteRecevied == SOCKET_ERROR)
		{
			cerr << "Error in recv()" << endl;
		}

		if (byteRecevied == 0)
		{
			cout << "client diconnected" << endl;
			break;
		}


		//echo message back to client
		send(clientSocket, buf, byteRecevied + 1, 0);

	}


	//close socket
	closesocket(clientSocket);

	//shudown winsock
	WSACleanup();


}
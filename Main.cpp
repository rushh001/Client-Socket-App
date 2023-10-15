#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

	cout << "\n Welcome to client socket app";

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "\n FAILED : WSAStartup failed.";
		return 1;
	}
	cout << "\n SUCCESS : Winsock library initialised";
	/*
	* Request for an socket from socket api.
	* Pass the arguments as per your type of socket requirement to socket function.
	* 1st parameter is to tell the address family that is going to be used
	*   1)AF_INET -> IPv4 address
	*   2)AF_INET6 -> IPv6 address
	* 2nd paramter is to tell what type of socket is needed
	*   1)SOCK_STREAM (Stream Sockets) uses TCP protocol
	*   2)SOCK_DGRAM (Datagram Sockets) uses UDP protocol
	* 3rd parameter is to tell the protocol to be used.
	*/
	SOCKET   listeningSocket;
	listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Could not create socket.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	cout << "\n SUCCESS : Socket created";
	/*
	* Store the details of the Port used along with the IP address family
	* in an SOCKADDR_IN object.
	*/
	const int PORT = 5757;  //Port number at which server is running. 
	SOCKADDR_IN   serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	// Pass the IP Address of the server.
	// Use inet_pton to convert the IP address from human-readable to binary format
	if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
		cerr << "\n FAILED : Invalid IP address.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	/*
	* Connect to the server
	*/
	int connect_result = connect(listeningSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (connect_result == SOCKET_ERROR) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Connect failed.";
		closesocket(listeningSocket);
		WSACleanup();
		return 1;
	}
	cout << "\n SUCCESS : Connected with server";
	//Send message
	std::string message = "Wassup! My friend!";
	if (send(listeningSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Send failed.";
	}
	else {
		cout << "\n SUCCESS : Sent to server: ";
	}
	//Receive message
	char buffer[1024];
	int bytesReceived = recv(listeningSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived == SOCKET_ERROR) {
		cerr << "\n FAILED : " << WSAGetLastError() << ", Receive failed.";
	}
	else {
		buffer[bytesReceived] = '\0';
		cout << "\n SUCCESS : Received from server: " << buffer;
	}
	//close socket
	closesocket(listeningSocket);
	WSACleanup();

	return 0;
}
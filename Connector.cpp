#include "Connector.h"

Connector::Connector() {
	sprintf_s<20>(IP, "127.0.0.1");
	port = 87;
	
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	bIsConnected = false;
	loopThread = nullptr;
}

bool Connector::Connect() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Failed. Error Code: " << WSAGetLastError() << "\n";
		return false;
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cerr << "Counld not create socket: " << WSAGetLastError() << "\n";
		return false;
	}

	int flag = connect(sock, (struct sockaddr*) &server, sizeof(server));
	bIsConnected = flag == 0;
	
	if (recvCallback == nullptr) {
		bIsConnected = false;
		return false;
	}
	loopThread = new std::thread(&Connector::ReceiveLoop, this);

	return bIsConnected;
}

void Connector::Close() {
	if (bIsConnected) {
		bIsConnected = false;

		shutdown(sock, 2);

		if (loopThread->joinable()) {
			loopThread->join();
			delete loopThread;
			loopThread = nullptr;

			closesocket(sock);
			WSACleanup();
		}
	}
}

void Connector::SetRecvCallback(ReceivedCallback func) {
	recvCallback = func;
}

void Connector::ReceiveLoop() {
	char server_reply[PACKAGE_SIZE];
	char message[PACKAGE_SIZE];
	while (bIsConnected) {
		int recvSize = recv(sock, server_reply, PACKAGE_SIZE, 0);
		if (recvSize > 0) {
			recvCallback(server_reply);
		}
		else if (recvSize == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			if (errorCode != WSAEWOULDBLOCK) break;
		}
		memset(message, 0, PACKAGE_SIZE);
		sprintf(message, "good\0");
		int flag = send(sock, message, PACKAGE_SIZE, 0);
		if (flag == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			if (errorCode != WSAEWOULDBLOCK) break;
		}
	}

	closesocket(sock);
	WSACleanup();
}

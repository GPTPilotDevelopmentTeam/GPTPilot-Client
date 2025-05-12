#include <winsock.h>
#include <iostream>
#include <thread>
#include <condition_variable>

#pragma comment (lib, "ws2_32.lib")

#define PACKAGE_SIZE	2048

typedef void (*ReceivedCallback) (const char* info);

class Connector {
public:
	Connector();
	bool Connect();
	void Close();
	void SetRecvCallback(ReceivedCallback func);

private:
	char IP[20];
	unsigned short port;

	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;

	bool bIsConnected;
	ReceivedCallback recvCallback;

	std::thread* loopThread;
	void ReceiveLoop();
};

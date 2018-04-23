// net.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "net.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

typedef struct
{
	WSAOVERLAPPED  overlap;
	WSABUF         Buffer;
	char           szMessage[SEND_MSSAGE_MAXLEN];
	DWORD          NumberOfBytesRecvd;
	DWORD          Flags;
}MY_IO_OPERATION_DATA;

struct stServerData
{
	HANDLE completionPort;
	NetCallBack func;
};

DWORD WINAPI WorkerThread(LPVOID);		//服务端工作线程
DWORD WINAPI AcceptConnectThread(LPVOID param);	//服务端接收连接线程
DWORD WINAPI ClientWorkThread(LPVOID param);	//客户端和服务端通信的线程

bool g_bServer = false;
SOCKET g_serverSocket = -1;
SOCKET g_clientSocket = -1;

void InitNet(unsigned port, NetCallBack func, bool bServer, char* pAddr)
{
	g_bServer = bServer;
	if (bServer)
	{
		WSADATA                 wsaData;
		SOCKET                  sListen, sClient;
		SOCKADDR_IN             local, client;
		DWORD                   i, dwThreadId;
		int                     iaddrSize = sizeof(SOCKADDR_IN);
		HANDLE                  CompletionPort = INVALID_HANDLE_VALUE;
		SYSTEM_INFO             systeminfo;
		MY_IO_OPERATION_DATA* lpPerIOData = NULL;

		// 初始化网络
		WSAStartup(0x0202, &wsaData);

		// 创建完成端口
		CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		// 创建工作线程
		GetSystemInfo(&systeminfo);
		stServerData data;
		data.completionPort = CompletionPort;
		data.func = func;
		for (i = 0; i < systeminfo.dwNumberOfProcessors; i++)
		{
			CreateThread(NULL, 0, WorkerThread, (void*)&data, 0, &dwThreadId);
		}

		// 创建服务器线程
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// 绑定
		local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		local.sin_family = AF_INET;
		local.sin_port = htons(port);
		bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

		// 监听 
		listen(sListen, 3);

		//在一个线程中不停的accept
		g_serverSocket = sListen;
		CreateThread(0, 0, AcceptConnectThread,(void*)CompletionPort , 0, 0);
	}
	else
	{
		//初始化
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			cout << "初始化失败" << endl;
			return ;
		}
		//HANDLE  
		//创建套接字
		SOCKET sock;
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			cout << "创建套接字失败" << endl;
			return ;
		}
		g_clientSocket = sock;
		//连接
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(sockaddr_in));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(pAddr);

		//连接服务器
		if (connect(sock, (sockaddr*)&serverAddr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			cout << "连接服务器失败" << endl;
			return ;
		}

		//创建一个线程用来和服务器通信
		CreateThread(NULL, 0, ClientWorkThread, func, 0, 0);
	}
}

DWORD WINAPI  WorkerThread(LPVOID param)
{
	stServerData data(*(stServerData*)param);
	HANDLE                  CompletionPort = data.completionPort;
	NetCallBack				func = data.func;
	DWORD                   dwBytesTransferred;
	SOCKET                  sClient;
	MY_IO_OPERATION_DATA* lpPerIOData = NULL;

	while (TRUE)
	{
		GetQueuedCompletionStatus(
			CompletionPort,
			&dwBytesTransferred,
			(PULONG_PTR)&sClient,
			(LPOVERLAPPED *)&lpPerIOData,
			INFINITE);
		if (dwBytesTransferred == 0xFFFFFFFF)
		{
			return 0;
		}
		if (dwBytesTransferred == 0)
		{
			// 断开连接
			closesocket(sClient);
			delete lpPerIOData;
		}
		else
		{
			func(sClient, lpPerIOData->szMessage, dwBytesTransferred);

			// 激活一个WSARecv
			memset(lpPerIOData, 0, sizeof(MY_IO_OPERATION_DATA));
			lpPerIOData->Buffer.len = SEND_MSSAGE_MAXLEN;
			lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
			WSARecv(sClient,
				&lpPerIOData->Buffer,
				1,
				&lpPerIOData->NumberOfBytesRecvd,
				&lpPerIOData->Flags,
				&lpPerIOData->overlap,
				NULL);
		}
	}
	return 0;
}

DWORD WINAPI AcceptConnectThread(LPVOID param)
{
	HANDLE CompletionPort = (HANDLE)param;
	sockaddr_in clientAddr;
	int len = sizeof(sockaddr_in);
	while (TRUE)
	{
		// 接收连接
		SOCKET sClient = accept(g_serverSocket, (struct sockaddr *)&clientAddr, &len);


		//把新连接的socket加到完成端口中
		CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)sClient, 0);

		//激活一个WSARecv
		MY_IO_OPERATION_DATA* lpPerIOData = new MY_IO_OPERATION_DATA;
		memset(lpPerIOData, 0, sizeof(MY_IO_OPERATION_DATA));
		lpPerIOData->Buffer.len = SEND_MSSAGE_MAXLEN;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
		WSARecv(sClient,
			&lpPerIOData->Buffer,
			1,
			&lpPerIOData->NumberOfBytesRecvd,
			&lpPerIOData->Flags,
			&lpPerIOData->overlap,
			NULL);
	}
	return 0;
}


DWORD WINAPI ClientWorkThread(LPVOID param)
{
	NetCallBack func = (NetCallBack)param;
	char buf[SEND_MSSAGE_MAXLEN] = { 0 };
	while (true)
	{
		int num;
		if ((num = recv(g_clientSocket, buf, SEND_MSSAGE_MAXLEN, 0)) == SOCKET_ERROR)
		{
			cout << "和服务器断开连接" << endl;
			return 0;
		}
		func(0, buf, num);
	}
	return 0;
}

//发送消息给客户端
void SendToClient(DWORD id, void* param, int len)
{
	if (!g_bServer)
	{
		return;
	}
	send(id, (char*)param, len, 0);
}

//发送消息给服务器
void SendToServer(void* param, int len)
{
	if (g_bServer)
	{
		return;
	}
	send(g_clientSocket, (char*)param, len, 0);
}
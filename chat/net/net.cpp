// net.cpp : ���� DLL Ӧ�ó���ĵ���������
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

DWORD WINAPI WorkerThread(LPVOID);		//����˹����߳�
DWORD WINAPI AcceptConnectThread(LPVOID param);	//����˽��������߳�
DWORD WINAPI ClientWorkThread(LPVOID param);	//�ͻ��˺ͷ����ͨ�ŵ��߳�

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

		// ��ʼ������
		WSAStartup(0x0202, &wsaData);

		// ������ɶ˿�
		CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		// ���������߳�
		GetSystemInfo(&systeminfo);
		stServerData data;
		data.completionPort = CompletionPort;
		data.func = func;
		for (i = 0; i < systeminfo.dwNumberOfProcessors; i++)
		{
			CreateThread(NULL, 0, WorkerThread, (void*)&data, 0, &dwThreadId);
		}

		// �����������߳�
		sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// ��
		local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		local.sin_family = AF_INET;
		local.sin_port = htons(port);
		bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

		// ���� 
		listen(sListen, 3);

		//��һ���߳��в�ͣ��accept
		g_serverSocket = sListen;
		CreateThread(0, 0, AcceptConnectThread,(void*)CompletionPort , 0, 0);
	}
	else
	{
		//��ʼ��
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			cout << "��ʼ��ʧ��" << endl;
			return ;
		}
		//HANDLE  
		//�����׽���
		SOCKET sock;
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			cout << "�����׽���ʧ��" << endl;
			return ;
		}
		g_clientSocket = sock;
		//����
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(sockaddr_in));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(pAddr);

		//���ӷ�����
		if (connect(sock, (sockaddr*)&serverAddr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			cout << "���ӷ�����ʧ��" << endl;
			return ;
		}

		//����һ���߳������ͷ�����ͨ��
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
			// �Ͽ�����
			closesocket(sClient);
			delete lpPerIOData;
		}
		else
		{
			func(sClient, lpPerIOData->szMessage, dwBytesTransferred);

			// ����һ��WSARecv
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
		// ��������
		SOCKET sClient = accept(g_serverSocket, (struct sockaddr *)&clientAddr, &len);


		//�������ӵ�socket�ӵ���ɶ˿���
		CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)sClient, 0);

		//����һ��WSARecv
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
			cout << "�ͷ������Ͽ�����" << endl;
			return 0;
		}
		func(0, buf, num);
	}
	return 0;
}

//������Ϣ���ͻ���
void SendToClient(DWORD id, void* param, int len)
{
	if (!g_bServer)
	{
		return;
	}
	send(id, (char*)param, len, 0);
}

//������Ϣ��������
void SendToServer(void* param, int len)
{
	if (g_bServer)
	{
		return;
	}
	send(g_clientSocket, (char*)param, len, 0);
}
// chat.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "../net/net.h"
using namespace std;
#pragma comment(lib,"../x64/debug/net.lib")

void FuncTest(DWORD id, void* param, int len)
{
	char * buf = (char*)param;
	buf[len] = '\0';
	cout << "服务器发来的消息：" << buf<< endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitNet(2222, FuncTest, false, "127.0.0.1");
	while (true)
	{
		cout << "输入要发送的消息:"<<endl;
		char buf[111];
		cin >> buf;
		SendToServer(buf, strlen(buf));
		Sleep(100);
	}
	return 0;
}


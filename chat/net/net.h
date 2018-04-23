#ifndef _NET_H_
#define _NET_H_
#include <windows.h>

#define SEND_MSSAGE_MAXLEN 2048
#define NET_API __declspec(dllexport)
typedef void(*NetCallBack)(DWORD id, void* param, int len);

//初始化网络，服务器和客户端都是用同一个
NET_API void InitNet(unsigned port, NetCallBack func, bool bServer = true, char* pAddr = NULL);

//发送消息给客户端
NET_API void SendToClient(DWORD id, void* param, int len);

//发送消息给服务器
NET_API void SendToServer(void* param, int len);

#endif
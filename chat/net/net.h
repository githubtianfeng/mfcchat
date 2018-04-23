#ifndef _NET_H_
#define _NET_H_
#include <windows.h>

#define SEND_MSSAGE_MAXLEN 2048
#define NET_API __declspec(dllexport)
typedef void(*NetCallBack)(DWORD id, void* param, int len);

//��ʼ�����磬�������Ϳͻ��˶�����ͬһ��
NET_API void InitNet(unsigned port, NetCallBack func, bool bServer = true, char* pAddr = NULL);

//������Ϣ���ͻ���
NET_API void SendToClient(DWORD id, void* param, int len);

//������Ϣ��������
NET_API void SendToServer(void* param, int len);

#endif
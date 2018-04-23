#pragma once
#include "cData.h"
class cServerMain
{
public:
	cServerMain();
	~cServerMain();

	//注册
	void Register(DWORD id,stMsgRegister st);

	//登陆
	void Login(DWORD id, stMsgLogin st);

	//查找
	void Search(DWORD id, stMsgSearch st);

	//请求加好友
	void QueryMakeFriend(DWORD id, stMsgQueryMakeFriend st);

	//回复是否同意被添加好友
	void MakeFriend(DWORD id, stMsgMakeFirend st);

	//发送消息
	void SendMsg(DWORD id, stMsgSendMsg st);

	//发送文件 
	void SendFile(DWORD id, stMsgSendFile st);
	
	//读取客户端最新版本号
	void ReadLastVersion();

	//请求版本号
	void QueryVersion(DWORD id);

	//更新
	void UpdateFileVersion(DWORD id, stMsgUpdata st);
private:
	cData* m_pUserInfoManager;	//用户信息管理器
};


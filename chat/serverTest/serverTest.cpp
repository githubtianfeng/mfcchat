// serverTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cServerMain.h"
#include "../sqlnet/sqlnet.h"
#pragma comment(lib,"../x64/debug/sqlnet.lib")

cServerMain* g_pServerMain = NULL;

void ServerNetCallBack(DWORD id, void* param, int len)
{
	char* buf = (char*)param;
	stMsgHead head;
	memcpy(&head, buf, sizeof(stMsgHead));
	switch (head.msgType)
	{
	case msg_accountExist:
	{
		stMsgRegister msg;
		if (sizeof(stMsgRegister) != len)
		{
			cout << "消息错误，大小不对" << endl;
			return;
		}
		memcpy(&msg, buf, sizeof(stMsgRegister));
		//g_pServerMain->QueryAccountExist(msg);
		break;
	}
	case msg_register:
	{
		stMsgRegister msg;
		memcpy(&msg, buf, sizeof(stMsgRegister));
		g_pServerMain->Register(id,msg);
		break;
	}
		break;
	case msg_login:
	{
		stMsgLogin msg;
		memcpy(&msg, buf, sizeof(stMsgLogin));
		g_pServerMain->Login(id,msg);
		break;
	}
	case msg_search:
	{
		stMsgSearch msg;
		memcpy(&msg, buf, sizeof(stMsgSearch));
		g_pServerMain->Search(id,msg);
	}
		break;
	case msg_makeQueryFriend:
	{
		stMsgQueryMakeFriend msg;
		memcpy(&msg, buf, sizeof(stMsgQueryMakeFriend));
		g_pServerMain->QueryMakeFriend(id,msg);
		break;
	}
	case msg_makeFriend:
	{
		stMsgMakeFirend msg;
		memcpy(&msg, buf, sizeof(stMsgMakeFirend));
		g_pServerMain->MakeFriend(id,msg);
		break;
	}
	case msg_sendMsg:
	{
		stMsgSendMsg msg;
		memcpy(&msg, buf, sizeof(stMsgSendMsg));
		g_pServerMain->SendMsg(id,msg);
		break;
	}
	case msg_sendFile:
	{
		stMsgSendFile msg;
		memcpy(&msg, buf, sizeof(stMsgSendFile));
		g_pServerMain->SendFile(id,msg);
		break;
	}
	case msg_version:
	{
		if (sizeof(stMsgVer) != len)
		{
			return;
		}
		g_pServerMain->QueryVersion(id);
	}
		break;
	case msg_UpdateFile:
	{
		if (sizeof(stMsgUpdata) != len)
		{
			return;
		}
		stMsgUpdata msg;
		memcpy(&msg, buf, len);
		g_pServerMain->UpdateFileVersion(id,msg);
	}
		break;
	default:
		break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	g_pServerMain = new cServerMain;
	InitNet(2222, ServerNetCallBack);

/*	cMySql sql;
	if (!sql.connect("127.0.0.1", 3306, "userinfodatabase", "root", "123456"))
	{
		cout << "连接数据库失败" << endl;
		return 0;
	}

	if (!sql.sqlExecute("select * from userinfo;"))
	{
		cout << "执行sql语句失败" << endl;
		return 0;
	}
	list<list<string>> resList = sql.getExecuteResults();*/
	/*for (list<list<string>>::iterator it = resList.begin(); it != resList.end();
		++it)
	{
		for (list<string>::iterator itString = it->begin(); itString != it->end();++itString)
		{
			cout << *itString << "\t";
		}
		cout << endl;
	}*/

	while (1)
	{
		char buf[111];
		cin >> buf;
	}
	return 0;
}


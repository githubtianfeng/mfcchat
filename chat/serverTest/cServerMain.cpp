#include "stdafx.h"
#include "cServerMain.h"
#include <list>
using namespace std;

int g_curVer = 0;	//保存客户端的最新版本

cServerMain::cServerMain()
{
	m_pUserInfoManager = new cData;
	//读取客户端当前最新版本号
	ReadLastVersion();
}


cServerMain::~cServerMain()
{
	delete m_pUserInfoManager;
}

void cServerMain::Register(DWORD id,stMsgRegister st)
{
	//注册
	OP_ERR_TYPE err = m_pUserInfoManager->AddUser(st);
	/*if (err == noerr)
	{
		//注册成功

	}
	else if (err == account_exist)
	{
		//帐号已存在
	}
	else if (err == neekname_exist)
	{
		//昵称已存在
	}
	else
	{
		//未知错误
	}
	*/

	//只管把处理结果发送给客户端
	stMsgResult results;
	results.errType = err;
	results.head.msgType = msg_registerRusult;
	SendToClient(id, &results, sizeof(stMsgResult));
}

void cServerMain::Login(DWORD id, stMsgLogin st)
{
	OP_ERR_TYPE err = m_pUserInfoManager->Login(st);
	if (err == noerr)
	{
		//登陆成功
		//发消息给客户端（自己的基本信息、好友信息，通知好友我上线了）。。。
		stUserInfo* pUserInfo = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
		if (pUserInfo != NULL)
		{
			pUserInfo->id = id;
		}
	}
	else if (err == account_notexist)
	{
		//帐号不存在
	}
	else if (err == password_err)
	{
		//密码错误
	}
	stUserInfo* pUserinfo = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
	
	if (pUserinfo == NULL)
	{
		err = account_notexist;
	}

	stMsgLoginResult msg;
	msg.head.msgType = msg_loginResult;
	msg.errType = err;
	if (err == noerr)
	{
		strcpy(msg.szAccount, pUserinfo->szAccount);
		strcpy(msg.szNeekNsame, pUserinfo->szNeekName);
	}
	SendToClient(id, &msg, sizeof(stMsgResult));

	//同步相关消息

	for (list<stFriendInfo>::iterator it = pUserinfo->listFriends.begin(); it != pUserinfo->listFriends.end();++it)
	{
		stMsgMakeFirend msg;
		msg.bAgree = 1;
		msg.head.msgType = msg_makeFriend;
		strcpy(msg.szAccount, it->szAccount);
		SendToClient(id, &msg, sizeof(stMsgMakeFirend));
		Sleep(10);
	}
}

void cServerMain::Search(DWORD id, stMsgSearch st)
{
	//因为查找结果个数具有不确定性，所以肯定不能一次表示出有多少个要发送给客户端
	//循环发送
	list<stSearchUserInfo> listInfo = m_pUserInfoManager->Search(st);

	//把所有搜索到的信息发送给客户端
	for (list<stSearchUserInfo>::iterator it = listInfo.begin(); it != listInfo.end();++it)
	{
		stMsgSearchResult msg;
		msg.head.msgType= msg_searchResult;
		msg.searchUserInfo = *it;
		SendToClient(id, &msg, sizeof(stMsgSearchResult));
		Sleep(10);
	}
}

void cServerMain::QueryMakeFriend(DWORD id, stMsgQueryMakeFriend st)
{
	OP_ERR_TYPE err = noerr;
	stUserInfo* pUserInfo = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
	if (pUserInfo == NULL)
	{
		err = account_notexist;
		//需要发送消息给请求方
	}
	else if (pUserInfo->online == 0)
	{
		err = err_noOnLine;
		//如果不在线，需要发送消息给请求方
	}
	else
	{
		//没有错误
		err = noerr;
	}
	
	if (err == noerr)
	{
		//发送消息给被请求方
		stMsgQueryMakeFriend msg;
		msg.head.msgType = msg_makeQueryFriend;

		stUserInfo* pResUser = m_pUserInfoManager->GetUserInfoById(id);
		if (pResUser == NULL)
		{
			//有问题
			return;
		}

		strcpy(msg.szAccount, pResUser->szAccount);
		SendToClient(pUserInfo->id, &msg, sizeof(stMsgQueryMakeFriend));
	}
	else
	{
		//发送消息给请求方
		stMsgQueryMakeFriendResult res;
		res.head.msgType = msg_QueryFriendResul;
		res.errType = err;
		SendToClient(id, &res, sizeof(stMsgQueryMakeFriendResult));
	}
}

void cServerMain::MakeFriend(DWORD id, stMsgMakeFirend st)
{
	stUserInfo* pUserInfo = m_pUserInfoManager->GetUserInfoById(id);	 //被请求加好友的人

	if (st.bAgree == 1)
	{
		stUserInfo* pUserDes = pUserInfo;//被请求人
		stUserInfo* pUserRes = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);//请求的人
		//同意的
		m_pUserInfoManager->AddFriend(pUserDes,pUserRes);//在服务器修改相关数据


		//通知两个客户端告诉他们添加对方为好友成功
		stMsgMakeFirend msg;
		msg.bAgree = 1;
		msg.head.msgType = msg_makeFriend;
		strcpy(msg.szAccount, pUserDes->szAccount);
		SendToClient(pUserRes->id, &msg, sizeof(msg));

		strcpy(msg.szAccount, pUserRes->szAccount);
		SendToClient(pUserDes->id, &msg, sizeof(msg));
		return;
	}

	stMsgMakeFirend msg;
	msg.head.msgType = msg_makeFriend;
	msg.bAgree = st.bAgree;
	strcpy(msg.szAccount, pUserInfo->szAccount);


	stUserInfo* pUserInfoRes = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);//最开始请求加好友的那个人
	if (pUserInfoRes->online == 0)
	{
		return;
	}
	SendToClient(pUserInfoRes->id, &msg, sizeof(stMsgMakeFirend));
}

void cServerMain::SendMsg(DWORD id, stMsgSendMsg st)
{
	stUserInfo* pUserSend = m_pUserInfoManager->GetUserInfoById(id);
	stUserInfo* pUserRecv = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
	if (pUserRecv == NULL || pUserSend == NULL)
	{
		return;
	}

	strcpy(st.szAccount, pUserSend->szAccount);
	SendToClient(pUserRecv->id, &st, sizeof(stMsgSendMsg));
}

void cServerMain::SendFile(DWORD id, stMsgSendFile st)
{
	stMsgSendFile msg;
	memcpy(&msg, &st, sizeof(msg));
	stUserInfo* pUserDesInfo = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
	stUserInfo* pUserResInfo = m_pUserInfoManager->GetUserInfoById(id);
	if (pUserDesInfo == NULL || pUserDesInfo == NULL)
	{
		return;
	}
	strcpy(msg.szAccount, pUserResInfo->szAccount);
	SendToClient(pUserDesInfo->id, &msg, sizeof(stMsgSendFile));
}

void cServerMain::ReadLastVersion()
{
	FILE* pFile = fopen("serverVer.txt", "r");
	if (pFile == NULL)
	{
		return;
	}

	char buf[102] = { 0 };
	fgets(buf, 100, pFile);
	g_curVer = atoi(buf);

	fclose(pFile);
}

void cServerMain::QueryVersion(DWORD id)
{
	stMsgVer msg;
	msg.head.msgType = msg_version;
	msg.ver = g_curVer;
	SendToClient(id, &msg, sizeof(msg));
}

void cServerMain::UpdateFileVersion(DWORD id, stMsgUpdata st)
{
	//确定要更新哪些文件（1.md5比较、2.针对你是什么版本确定更新哪些文件）
	//这里测试，假如就是更新.exe
	//把这个发送给客户端

	FILE* pFile = fopen("client/chatClient.exe", "rb");
	if (pFile == NULL)
	{
		return ;
	}
	fseek(pFile, 0, SEEK_END);
	st.allSize =  ftell(pFile);
	
	fseek(pFile, 0, SEEK_SET);

	char buf[1024] = { 0 };
	int ret = 0;
	ret = fread(buf, 1, 1024, pFile);

	while (ret)
	{
		//发送读取到的文件 信息给对方
		//要获得是哪个
		stMsgUpdata msg;
		msg.curVersion = g_curVer;
		msg.head.msgType = msg_UpdateFile;
		strcpy(msg.szFileName, "chatClient.exe");
		memcpy(msg.szFile, buf, ret);
		msg.allSize = st.allSize;
		msg.size = ret;
		msg.bFinish = 0;
		ret = fread(buf, 1, 1024, pFile);
		if (ret == 0)
		{
			msg.bFinish = 1;
		}

		SendToClient(id, &msg, sizeof(stMsgUpdata));
		Sleep(20);
	}
	fclose(pFile);
}
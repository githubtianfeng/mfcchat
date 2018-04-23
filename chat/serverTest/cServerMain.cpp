#include "stdafx.h"
#include "cServerMain.h"
#include <list>
using namespace std;

int g_curVer = 0;	//����ͻ��˵����°汾

cServerMain::cServerMain()
{
	m_pUserInfoManager = new cData;
	//��ȡ�ͻ��˵�ǰ���°汾��
	ReadLastVersion();
}


cServerMain::~cServerMain()
{
	delete m_pUserInfoManager;
}

void cServerMain::Register(DWORD id,stMsgRegister st)
{
	//ע��
	OP_ERR_TYPE err = m_pUserInfoManager->AddUser(st);
	/*if (err == noerr)
	{
		//ע��ɹ�

	}
	else if (err == account_exist)
	{
		//�ʺ��Ѵ���
	}
	else if (err == neekname_exist)
	{
		//�ǳ��Ѵ���
	}
	else
	{
		//δ֪����
	}
	*/

	//ֻ�ܰѴ��������͸��ͻ���
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
		//��½�ɹ�
		//����Ϣ���ͻ��ˣ��Լ��Ļ�����Ϣ��������Ϣ��֪ͨ�����������ˣ�������
		stUserInfo* pUserInfo = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);
		if (pUserInfo != NULL)
		{
			pUserInfo->id = id;
		}
	}
	else if (err == account_notexist)
	{
		//�ʺŲ�����
	}
	else if (err == password_err)
	{
		//�������
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

	//ͬ�������Ϣ

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
	//��Ϊ���ҽ���������в�ȷ���ԣ����Կ϶�����һ�α�ʾ���ж��ٸ�Ҫ���͸��ͻ���
	//ѭ������
	list<stSearchUserInfo> listInfo = m_pUserInfoManager->Search(st);

	//����������������Ϣ���͸��ͻ���
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
		//��Ҫ������Ϣ������
	}
	else if (pUserInfo->online == 0)
	{
		err = err_noOnLine;
		//��������ߣ���Ҫ������Ϣ������
	}
	else
	{
		//û�д���
		err = noerr;
	}
	
	if (err == noerr)
	{
		//������Ϣ��������
		stMsgQueryMakeFriend msg;
		msg.head.msgType = msg_makeQueryFriend;

		stUserInfo* pResUser = m_pUserInfoManager->GetUserInfoById(id);
		if (pResUser == NULL)
		{
			//������
			return;
		}

		strcpy(msg.szAccount, pResUser->szAccount);
		SendToClient(pUserInfo->id, &msg, sizeof(stMsgQueryMakeFriend));
	}
	else
	{
		//������Ϣ������
		stMsgQueryMakeFriendResult res;
		res.head.msgType = msg_QueryFriendResul;
		res.errType = err;
		SendToClient(id, &res, sizeof(stMsgQueryMakeFriendResult));
	}
}

void cServerMain::MakeFriend(DWORD id, stMsgMakeFirend st)
{
	stUserInfo* pUserInfo = m_pUserInfoManager->GetUserInfoById(id);	 //������Ӻ��ѵ���

	if (st.bAgree == 1)
	{
		stUserInfo* pUserDes = pUserInfo;//��������
		stUserInfo* pUserRes = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);//�������
		//ͬ���
		m_pUserInfoManager->AddFriend(pUserDes,pUserRes);//�ڷ������޸��������


		//֪ͨ�����ͻ��˸���������ӶԷ�Ϊ���ѳɹ�
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


	stUserInfo* pUserInfoRes = m_pUserInfoManager->GetUserInfoByAccount(st.szAccount);//�ʼ����Ӻ��ѵ��Ǹ���
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
	//ȷ��Ҫ������Щ�ļ���1.md5�Ƚϡ�2.�������ʲô�汾ȷ��������Щ�ļ���
	//������ԣ�������Ǹ���.exe
	//��������͸��ͻ���

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
		//���Ͷ�ȡ�����ļ� ��Ϣ���Է�
		//Ҫ������ĸ�
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
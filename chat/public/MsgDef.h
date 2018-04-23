#ifndef _MSGDEF_H
#define _MSGDEF_H
#include <List>
using namespace std;

#define MIN_INFO_LEN  6		//�ʺš����롢�ǳ���̳���
#define MAX_ACCOUNT_LEN	10	//�ʺŵ���󳤶�
#define MAX_PASSWORD_LEN 20	//�������󳤶�
#define MAX_NEEKNAME_LEN 20	//�ǳ���󳤶�
#define CARDNO_LEN	18	//���֤����
#define MAX_MSG_LEN 200	//��Ϣ����󳤶�

enum MSG_TYPE
{
	msg_accountExist,	//ѯ�ʷ������ʺ��Ƿ����
	msg_register,		//ע��
	msg_registerRusult,	//ע����
	msg_login,			//��½
	msg_loginResult,	//��½��� 
	msg_sendMsg,		//����Ϣ
	msg_search,			//����
	msg_searchResult,	//������� 
	msg_makeQueryFriend,		//��Ӻ�������
	msg_makeFriend,//��Ӻ��ѽ��
	msg_QueryFriendResul,	//������Ӻ��ѽ��
	msg_sendFile,		//�����ļ�
	msg_version,			//�汾��
	msg_UpdateFile,		//���°汾
};

enum OP_ERR_TYPE
{
	noerr,	//�޴���
	account_exist,	//�ʺ��Ѵ���
	neekname_exist,	//�ǳ��Ѵ���
	account_notexist,	//�ʺŲ�����
	password_err,	//�������
	err_noOnLine,	//�û�������
};


struct stMsgHead
{
	MSG_TYPE msgType;
};

//ע����Ϣ
struct stMsgRegister
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szPassword[MAX_PASSWORD_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
	char szCardNo[CARDNO_LEN+1];
};

struct stMsgResult
{
	stMsgHead head;
	OP_ERR_TYPE errType;	//��������
};


//��½��Ϣ
struct stMsgLogin
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szPassword[MAX_PASSWORD_LEN];
};

//��½�����Ϣ
struct stMsgLoginResult
{
	stMsgHead head;
	OP_ERR_TYPE errType;
	char szAccount[MAX_ACCOUNT_LEN];
	char szNeekNsame[MAX_NEEKNAME_LEN];
};

//������Ϣ
struct stMsgSearch
{
	stMsgHead head;
	char szName[MAX_NEEKNAME_LEN];
};

//������Ϣ
struct stFriendInfo
{
	char szAccount[MAX_ACCOUNT_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
};

//�û�������Ϣ�ṹ��
struct stUserInfo
{
	char szAccount[MAX_ACCOUNT_LEN];
	char szPassword[MAX_PASSWORD_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
	char szCardNo[CARDNO_LEN + 1];
	int online;	//�Ƿ�����
	list<stFriendInfo> listFriends;
	DWORD id;//�û�ID�ţ�ÿ�ε�½���᲻һ����ʵ����socket
};

//�����û�����Ϣ
struct stSearchUserInfo
{
	char szAccout[MAX_ACCOUNT_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
	char bOnLine;	//�Ƿ�����
};


//���������Ϣ
struct stMsgSearchResult
{
	stMsgHead head;
	stSearchUserInfo searchUserInfo;
};

//����Ӻ�����Ϣ
struct stMsgQueryMakeFriend
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];	//������
};

struct stMsgQueryMakeFriendResult
{
	stMsgHead head;
	OP_ERR_TYPE errType;
};

//�Ӻ�����Ϣ
struct stMsgMakeFirend
{
	stMsgHead head;
	char bAgree;	//�Ƿ�ͬ��
	char szAccount[MAX_ACCOUNT_LEN];	//����Ϊ����
};

//������Ϣ
struct stMsgSendMsg
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szMsg[MAX_MSG_LEN];
};

//�����ļ���Ϣ
struct stMsgSendFile
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szFileName[MAX_PATH];
	char szFile[1024];
	int size;
	char bFinish;	//�Ƿ������
};

//�汾����Ϣ
struct stMsgVer
{
	stMsgHead head;
	int ver;
};

//������Ϣ
struct stMsgUpdata
{
	stMsgHead head;
	int curVersion;	//��ǰ�汾��
	char szFileName[50];	//�ļ���
	char szFile[1024];	//�ļ� 
	int size;
	int bFinish;
	DWORD allSize;	//�ļ� �ܴ�С
};

#endif


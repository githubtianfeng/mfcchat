#ifndef _MSGDEF_H
#define _MSGDEF_H
#include <List>
using namespace std;

#define MIN_INFO_LEN  6		//帐号、密码、昵称最短长度
#define MAX_ACCOUNT_LEN	10	//帐号的最大长度
#define MAX_PASSWORD_LEN 20	//密码的最大长度
#define MAX_NEEKNAME_LEN 20	//昵称最大长度
#define CARDNO_LEN	18	//身份证长度
#define MAX_MSG_LEN 200	//消息的最大长度

enum MSG_TYPE
{
	msg_accountExist,	//询问服务器帐号是否存在
	msg_register,		//注册
	msg_registerRusult,	//注册结果
	msg_login,			//登陆
	msg_loginResult,	//登陆结果 
	msg_sendMsg,		//发消息
	msg_search,			//搜索
	msg_searchResult,	//搜索结果 
	msg_makeQueryFriend,		//添加好友请求
	msg_makeFriend,//添加好友结果
	msg_QueryFriendResul,	//请求添加好友结果
	msg_sendFile,		//发送文件
	msg_version,			//版本号
	msg_UpdateFile,		//更新版本
};

enum OP_ERR_TYPE
{
	noerr,	//无错误
	account_exist,	//帐号已存在
	neekname_exist,	//昵称已存在
	account_notexist,	//帐号不存在
	password_err,	//密码错误
	err_noOnLine,	//用户不在线
};


struct stMsgHead
{
	MSG_TYPE msgType;
};

//注册消息
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
	OP_ERR_TYPE errType;	//错误类型
};


//登陆消息
struct stMsgLogin
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szPassword[MAX_PASSWORD_LEN];
};

//登陆结果消息
struct stMsgLoginResult
{
	stMsgHead head;
	OP_ERR_TYPE errType;
	char szAccount[MAX_ACCOUNT_LEN];
	char szNeekNsame[MAX_NEEKNAME_LEN];
};

//搜索消息
struct stMsgSearch
{
	stMsgHead head;
	char szName[MAX_NEEKNAME_LEN];
};

//好友信息
struct stFriendInfo
{
	char szAccount[MAX_ACCOUNT_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
};

//用户基本信息结构体
struct stUserInfo
{
	char szAccount[MAX_ACCOUNT_LEN];
	char szPassword[MAX_PASSWORD_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
	char szCardNo[CARDNO_LEN + 1];
	int online;	//是否在线
	list<stFriendInfo> listFriends;
	DWORD id;//用户ID号，每次登陆都会不一样，实际上socket
};

//查找用户的信息
struct stSearchUserInfo
{
	char szAccout[MAX_ACCOUNT_LEN];
	char szNeekName[MAX_NEEKNAME_LEN];
	char bOnLine;	//是否在线
};


//搜索结果信息
struct stMsgSearchResult
{
	stMsgHead head;
	stSearchUserInfo searchUserInfo;
};

//请求加好友消息
struct stMsgQueryMakeFriend
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];	//被请求方
};

struct stMsgQueryMakeFriendResult
{
	stMsgHead head;
	OP_ERR_TYPE errType;
};

//加好友消息
struct stMsgMakeFirend
{
	stMsgHead head;
	char bAgree;	//是否同意
	char szAccount[MAX_ACCOUNT_LEN];	//被加为好友
};

//聊天消息
struct stMsgSendMsg
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szMsg[MAX_MSG_LEN];
};

//发送文件消息
struct stMsgSendFile
{
	stMsgHead head;
	char szAccount[MAX_ACCOUNT_LEN];
	char szFileName[MAX_PATH];
	char szFile[1024];
	int size;
	char bFinish;	//是否传输完成
};

//版本号消息
struct stMsgVer
{
	stMsgHead head;
	int ver;
};

//更新消息
struct stMsgUpdata
{
	stMsgHead head;
	int curVersion;	//当前版本号
	char szFileName[50];	//文件名
	char szFile[1024];	//文件 
	int size;
	int bFinish;
	DWORD allSize;	//文件 总大小
};

#endif


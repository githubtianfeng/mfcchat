#pragma once
#include "../sqlnet/sqlnet.h"
#include "../public/MsgDef.h"
#include <list>
using namespace std;
//负责操作操作数据库
class cData
{
public:
	cData();
	~cData();

	//初始化用户基本信息
	void InitUserBaseInfo();

	//注册
	OP_ERR_TYPE AddUser(stMsgRegister st);
	

	//帐号是否存在
	bool IsAccountExist(char* szAccount);
	
	//昵称是否存在
	bool IsNeekNameExist(char* szNeekName);

	//登陆
	OP_ERR_TYPE Login(stMsgLogin st);

	//通过帐号获得用户信息
	stUserInfo*  GetUserInfoByAccount(char* szAccount);

	//通过id获得用户信息
	stUserInfo* GetUserInfoById(DWORD id);

	//查找
	list<stSearchUserInfo> Search(stMsgSearch st);

	//设定两互为好友
	void AddFriend(stUserInfo* pUser1, stUserInfo* pUser2);
private:
	cMySql* m_pSql;
	list<stUserInfo*> m_listUserBaseInfo;	//用户基本信息
};


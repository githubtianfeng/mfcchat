#include "stdafx.h"
#include "cData.h"


cData::cData()
{
	m_pSql = new cMySql;
	m_pSql->connect("127.0.0.1", 3306, "userinfodatabase", "root", "");
	//加载所有用户信息
	InitUserBaseInfo();
}


cData::~cData()
{
	//先保存数据
	//再关闭连接
	//delete m_pSql;
	//把m_listUserBaseInfo清掉
}

void cData::InitUserBaseInfo()
{
	//就是把数据据中所有用户信息加载到m_listUserBaseInfo
	if (m_pSql->sqlExecute("select * from userinfo") == false)
	{
		cout << "加载用户信息失败" << endl;
		return;
	}

	list<list<string>> resList = m_pSql->getExecuteResults();
	for (list<list<string>>::iterator it = resList.begin(); it != resList.end();++it)
	{
		stUserInfo* pInfo = new stUserInfo;
		list<string>::iterator itString = it->begin();
		strcpy(pInfo->szAccount, itString->c_str());
		++itString;
		strcpy(pInfo->szPassword, itString->c_str());
		++itString;
		strcpy(pInfo->szNeekName, itString->c_str());
		++itString;
		strcpy(pInfo->szCardNo, itString->c_str());
		++itString;

		//是一个好友信息字符串，要解析
		string str = *itString;
		int pos = str.find('_');
		while (pos != -1)
		{
			string tempStr = str.substr(0, pos);
			stFriendInfo info;
			strcpy(info.szAccount, tempStr.c_str());
			strcpy(info.szNeekName, "");
			pInfo->listFriends.push_back(info);

			str =str.substr(pos + 1);
			pos = str.find('_');
		}
		pInfo->online = 0;
		pInfo->id = -1;
		m_listUserBaseInfo.push_back(pInfo);
	}
}

OP_ERR_TYPE cData::AddUser(stMsgRegister st)
{
	OP_ERR_TYPE err = noerr;

	//只做两个判断 用户名是否已存在   昵称是否已存在
	//本来还要检测身份证号的正确性，，密码等。。。统统忽略
	if (IsAccountExist(st.szAccount))
	{
		err = account_exist;
	}
	else if (IsNeekNameExist(st.szNeekName))
	{
		err = neekname_exist;
	}
	else
	{
		//添加这个用户
		//1、写数据库  2、保存到m_listUserBaseInfo中
		stUserInfo* pUser = new stUserInfo;
		strcpy(pUser->szAccount, st.szAccount);
		strcpy(pUser->szPassword, st.szPassword);
		strcpy(pUser->szNeekName, st.szNeekName);
		strcpy(pUser->szCardNo, st.szCardNo);
		m_listUserBaseInfo.push_back(pUser);

		//插入到数据库
		string strSql = "insert into userinfo values('";
		strSql  = strSql+ st.szAccount + "','" + st.szPassword + "','" + st.szNeekName + "','" + st.szCardNo + "');";
		m_pSql->sqlExecute((char*)strSql.c_str());
	}
	return err;
}

bool cData::IsAccountExist(char* szAccount)
{
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end();++it)
	{
		if (strcpy((*it)->szAccount,szAccount) == 0)
		{
			return true;
		}
	}
	return false;
}

bool cData::IsNeekNameExist(char* szNeekName)
{
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end(); ++it)
	{
		if (strcpy((*it)->szNeekName, szNeekName) == 0)
		{
			return true;
		}
	}
	return false;
}

OP_ERR_TYPE cData::Login(stMsgLogin st)
{
	//比较帐号和密码是否正确
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end(); ++it)
	{
		if (strcmp((*it)->szAccount, st.szAccount) == 0)
		{
			if (strcmp((*it)->szPassword,st.szPassword) == 0)
			{
				//登陆成功
				(*it)->online = 1;
				return noerr;
			}
			//密码错误
			return password_err;
		}
	}
	//帐号不存在
	return account_notexist;
}

list<stSearchUserInfo> cData::Search(stMsgSearch st)
{
	list<stSearchUserInfo> infoList;
	int len = strlen(st.szName);
	//查找帐号和关键字匹配的
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end(); ++it)
	{
		string strTempAccount = (*it)->szAccount;
		string strTempNeeName = (*it)->szNeekName;
		if (strTempAccount.find(st.szName) != -1 &&
		len  < MAX_ACCOUNT_LEN)
		{
			//说明找到了
			stSearchUserInfo info;
			info.bOnLine = 0;
			strcpy(info.szAccout, (*it)->szAccount);
			strcpy(info.szNeekName, (*it)->szNeekName);
			info.bOnLine = (*it)->online;
			infoList.push_back(info);
			
		}
		else if (strTempNeeName.find(st.szName) != -1)
		{
			//说明找到了
			stSearchUserInfo info;
			info.bOnLine = 0;
			strcpy(info.szAccout, (*it)->szAccount);
			strcpy(info.szNeekName, (*it)->szNeekName);
			info.bOnLine = (*it)->online;
			infoList.push_back(info);
			
		}
	}

	//查找昵称相似的
	
	return infoList;
}

stUserInfo* cData::GetUserInfoByAccount(char* szAccount)
{
	if (szAccount == NULL)
	{
		return NULL;
	}
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end(); ++it)
	{
		if (strcmp((*it)->szAccount,szAccount) == 0)
		{
			return *it;
		}
	}
	return NULL;
}

stUserInfo* cData::GetUserInfoById(DWORD id)
{
	for (list<stUserInfo*>::iterator it = m_listUserBaseInfo.begin();
		it != m_listUserBaseInfo.end(); ++it)
	{
		if ((*it)->id== id)
		{
			return *it;
		}
	}
	return NULL;
}

void cData::AddFriend(stUserInfo* pUser1, stUserInfo* pUser2)
{
	if (pUser2 == NULL || pUser1 == NULL)
	{
		return;
	}
	stFriendInfo stfriend;
	strcpy(stfriend.szAccount, pUser2->szAccount);
	strcpy(stfriend.szNeekName, pUser2->szNeekName);
	pUser1->listFriends.push_back(stfriend);
	strcpy(stfriend.szAccount, pUser1->szAccount);
	strcpy(stfriend.szNeekName, pUser1->szNeekName);
	pUser2->listFriends.push_back(stfriend);
	//写下数据库
	string str;
	//abc_xd_ddfd_
	for (list<stFriendInfo>::iterator it = pUser1->listFriends.begin(); it != pUser1->listFriends.end();++it)
	{
		str += it->szAccount;
		str += "_";
	}
	//在数据库中修改了第一个用户的好友列表
	string sql = "update userinfo set friends = '";
	sql += str;
	sql += "' where account = '";
	sql += pUser1->szAccount;
	sql += "';";
	m_pSql->sqlExecute((char*)sql.c_str());

	//在数据库中修改了第二个用户的好友列表
	str.clear();
	//abc_xd_ddfd_
	for (list<stFriendInfo>::iterator it = pUser2->listFriends.begin(); it != pUser2->listFriends.end(); ++it)
	{
		str += it->szAccount;
		str += "_";
	}
	sql = "update userinfo set friends = '";
	sql += str;
	sql += "' where account = '";
	sql += pUser2->szAccount;
	sql += "';";
	m_pSql->sqlExecute((char*)sql.c_str());
}
#pragma once
#include "../sqlnet/sqlnet.h"
#include "../public/MsgDef.h"
#include <list>
using namespace std;
//��������������ݿ�
class cData
{
public:
	cData();
	~cData();

	//��ʼ���û�������Ϣ
	void InitUserBaseInfo();

	//ע��
	OP_ERR_TYPE AddUser(stMsgRegister st);
	

	//�ʺ��Ƿ����
	bool IsAccountExist(char* szAccount);
	
	//�ǳ��Ƿ����
	bool IsNeekNameExist(char* szNeekName);

	//��½
	OP_ERR_TYPE Login(stMsgLogin st);

	//ͨ���ʺŻ���û���Ϣ
	stUserInfo*  GetUserInfoByAccount(char* szAccount);

	//ͨ��id����û���Ϣ
	stUserInfo* GetUserInfoById(DWORD id);

	//����
	list<stSearchUserInfo> Search(stMsgSearch st);

	//�趨����Ϊ����
	void AddFriend(stUserInfo* pUser1, stUserInfo* pUser2);
private:
	cMySql* m_pSql;
	list<stUserInfo*> m_listUserBaseInfo;	//�û�������Ϣ
};


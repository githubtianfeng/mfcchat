#pragma once
#include "cData.h"
class cServerMain
{
public:
	cServerMain();
	~cServerMain();

	//ע��
	void Register(DWORD id,stMsgRegister st);

	//��½
	void Login(DWORD id, stMsgLogin st);

	//����
	void Search(DWORD id, stMsgSearch st);

	//����Ӻ���
	void QueryMakeFriend(DWORD id, stMsgQueryMakeFriend st);

	//�ظ��Ƿ�ͬ�ⱻ��Ӻ���
	void MakeFriend(DWORD id, stMsgMakeFirend st);

	//������Ϣ
	void SendMsg(DWORD id, stMsgSendMsg st);

	//�����ļ� 
	void SendFile(DWORD id, stMsgSendFile st);
	
	//��ȡ�ͻ������°汾��
	void ReadLastVersion();

	//����汾��
	void QueryVersion(DWORD id);

	//����
	void UpdateFileVersion(DWORD id, stMsgUpdata st);
private:
	cData* m_pUserInfoManager;	//�û���Ϣ������
};


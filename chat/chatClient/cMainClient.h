#pragma once
#include "../public/MsgDef.h"
#include "cSearchDlg.h"
#include <list>
#include "afxwin.h"

class cChatDlg;
// cMainClient �Ի���

class cMainClient : public CDialogEx
{
	DECLARE_DYNAMIC(cMainClient)

public:
	cMainClient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cMainClient();

	//���õ�ǰ�û���Ϣ
	void SetUserInfo(char* szAccount, char* szNeekName);

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

	cSearchDlg* m_pSearchDlg;	//�������ѽ���

	CString m_strNeekName;
	CString m_strAccount;

	//�����б�
	std::list<stFriendInfo> m_listFriend;

	//���촰���б�
	std::list<cChatDlg*> m_listChatWnd;

	//���ҽ��
	void SearchResult(stMsgSearchResult st);

	//���˵���Ӻú�����
	void QueryMakeFriend(stMsgQueryMakeFriend st);

	//��Ӻ���
	void MakeFriend(stMsgMakeFirend st);

	//������Ϣ
	void RectMsg(stMsgSendMsg st);

	//ͨ���ʺŻ�����촰��
	cChatDlg* GetRecvDlg(CStringA strAccount);

	//���մ��ļ���Ϣ
	void RecvFile(stMsgSendFile st);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSearch();
	CListBox m_listBoxFriend;
	afx_msg void OnLbnDblclkList1();
};

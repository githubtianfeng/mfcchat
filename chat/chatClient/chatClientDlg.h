
// chatClientDlg.h : ͷ�ļ�
//

#pragma once
#include "cRegisterDialog.h"
#include "cMainClient.h"

// CchatClientDlg �Ի���
class CchatClientDlg : public CDialogEx
{
// ����
public:
	CchatClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOGIN_DLG };

	cRegisterDialog* m_pRegisterDlg;	//ע�ᴰ��
	cMainClient* m_pMainClientWnd;		//������

	void LoginResult(stMsgLoginResult st);	//��½��� 

	//���ҽ��
	void SearchResult(stMsgSearchResult st);

	//��Ӻ��������� 
	void QueryMakeFriendResult
		(stMsgQueryMakeFriendResult st);

	//���˵���Ӻ�������
	void QueryMakeFriend(stMsgQueryMakeFriend st)
	{
		m_pMainClientWnd->QueryMakeFriend(st);
	}

	//��Ӻ���
	void MakeFriend(stMsgMakeFirend st){
		m_pMainClientWnd->MakeFriend(st);
	}

	//���յ���Ϣ
	void RecvMsg(stMsgSendMsg st){
		m_pMainClientWnd->RectMsg(st);
	}

	//�����ļ� 
	void RecvFile(stMsgSendFile st)
	{
		m_pMainClientWnd->RecvFile(st);
	}


	//�������°汾��
	void RequeryVer();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnRegister();
	CString m_account;
	CString m_password;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

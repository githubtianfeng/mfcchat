#pragma once
#include "../public/MsgDef.h"
// cRegisterDialog �Ի���

class cRegisterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cRegisterDialog)

public:
	cRegisterDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cRegisterDialog();

// �Ի�������
	enum { IDD = IDD_REGISTER_DIALOG };


	//ע��������
	void RegisterResult(stMsgResult rest);
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConfirm();
	afx_msg void OnBnClickedBtnCancel();
	CString m_account;
	CString m_password1;
	CString m_password2;
	CString m_neekname;
	CString m_cardno;
	afx_msg void OnClose();
};

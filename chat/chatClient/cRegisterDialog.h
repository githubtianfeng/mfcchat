#pragma once
#include "../public/MsgDef.h"
// cRegisterDialog 对话框

class cRegisterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cRegisterDialog)

public:
	cRegisterDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cRegisterDialog();

// 对话框数据
	enum { IDD = IDD_REGISTER_DIALOG };


	//注册结果返回
	void RegisterResult(stMsgResult rest);
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

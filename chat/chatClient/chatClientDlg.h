
// chatClientDlg.h : 头文件
//

#pragma once
#include "cRegisterDialog.h"
#include "cMainClient.h"

// CchatClientDlg 对话框
class CchatClientDlg : public CDialogEx
{
// 构造
public:
	CchatClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LOGIN_DLG };

	cRegisterDialog* m_pRegisterDlg;	//注册窗口
	cMainClient* m_pMainClientWnd;		//主窗口

	void LoginResult(stMsgLoginResult st);	//登陆结果 

	//查找结果
	void SearchResult(stMsgSearchResult st);

	//添加好友请求结果 
	void QueryMakeFriendResult
		(stMsgQueryMakeFriendResult st);

	//别人的添加好友请求
	void QueryMakeFriend(stMsgQueryMakeFriend st)
	{
		m_pMainClientWnd->QueryMakeFriend(st);
	}

	//添加好友
	void MakeFriend(stMsgMakeFirend st){
		m_pMainClientWnd->MakeFriend(st);
	}

	//接收到消息
	void RecvMsg(stMsgSendMsg st){
		m_pMainClientWnd->RectMsg(st);
	}

	//接收文件 
	void RecvFile(stMsgSendFile st)
	{
		m_pMainClientWnd->RecvFile(st);
	}


	//请求最新版本号
	void RequeryVer();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

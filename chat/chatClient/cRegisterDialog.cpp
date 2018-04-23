// cRegisterDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "chatClient.h"
#include "cRegisterDialog.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"


// cRegisterDialog 对话框

IMPLEMENT_DYNAMIC(cRegisterDialog, CDialogEx)

cRegisterDialog::cRegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(cRegisterDialog::IDD, pParent)
	, m_account(_T(""))
	, m_password1(_T(""))
	, m_password2(_T(""))
	, m_neekname(_T(""))
	, m_cardno(_T(""))
{

}

cRegisterDialog::~cRegisterDialog()
{
}

void cRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_account);
	DDX_Text(pDX, IDC_PASSWORD1, m_password1);
	DDX_Text(pDX, IDC_PASSWORD2, m_password2);
	DDX_Text(pDX, IDC_NEEKNAME, m_neekname);
	DDX_Text(pDX, IDC_CARDNO, m_cardno);
}


BEGIN_MESSAGE_MAP(cRegisterDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &cRegisterDialog::OnBnClickedBtnConfirm)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &cRegisterDialog::OnBnClickedBtnCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// cRegisterDialog 消息处理程序


void cRegisterDialog::OnBnClickedBtnConfirm()
{
	//确认注册
	UpdateData(TRUE);	//把控件上的数据更新到变量
	//判断 一些数据的合法性
	if (m_account.GetLength() < 6 || m_account.GetLength()>=MAX_ACCOUNT_LEN)
	{
		MessageBox(L"帐号长度不符合（大于6小于10）");
		return;
	}

	//判断密码输入是否正确
	if (m_password1.Compare(m_password2) != 0)
	{
		MessageBox(L"两次输入的密码不一样");
		return;
	}
	CStringA strAccount(m_account);
	CStringA strPassword(m_password1);
	CStringA strNeekName(m_neekname);
	CStringA strCardNo(m_cardno);
	stMsgRegister msg;
	msg.head.msgType = msg_register;
	strcpy(msg.szAccount, strAccount.GetString());
	strcpy(msg.szPassword, strPassword.GetString());
	strcpy(msg.szCardNo, strCardNo.GetString());
	strcpy(msg.szNeekName, strNeekName.GetString());

	SendToServer(&msg, sizeof(stMsgRegister));
}


void cRegisterDialog::OnBnClickedBtnCancel()
{
	//取消
	//回到登陆界面
	m_pParentWnd->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);
//	DestroyWindow();
//	delete this;
}


void cRegisterDialog::RegisterResult(stMsgResult rest)
{
	if (rest.errType == noerr)
	{
		//注册成功
		MessageBox(L"注册成功");
		//返回登陆界面
		m_pParentWnd->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
	}
	else if (rest.errType == account_exist)
	{
		MessageBox(L"帐号已存在");
	}
	else if (rest.errType == neekname_exist)
	{
		MessageBox(L"昵称已存在");
	}
	else
	{
		//出了什么问题我的也不知道
		MessageBox(L"未知错误，注册失败");
	}
}

void cRegisterDialog::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_pParentWnd->DestroyWindow();
	CDialogEx::OnClose();
}

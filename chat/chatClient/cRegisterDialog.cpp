// cRegisterDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chatClient.h"
#include "cRegisterDialog.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"


// cRegisterDialog �Ի���

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


// cRegisterDialog ��Ϣ�������


void cRegisterDialog::OnBnClickedBtnConfirm()
{
	//ȷ��ע��
	UpdateData(TRUE);	//�ѿؼ��ϵ����ݸ��µ�����
	//�ж� һЩ���ݵĺϷ���
	if (m_account.GetLength() < 6 || m_account.GetLength()>=MAX_ACCOUNT_LEN)
	{
		MessageBox(L"�ʺų��Ȳ����ϣ�����6С��10��");
		return;
	}

	//�ж����������Ƿ���ȷ
	if (m_password1.Compare(m_password2) != 0)
	{
		MessageBox(L"������������벻һ��");
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
	//ȡ��
	//�ص���½����
	m_pParentWnd->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);
//	DestroyWindow();
//	delete this;
}


void cRegisterDialog::RegisterResult(stMsgResult rest)
{
	if (rest.errType == noerr)
	{
		//ע��ɹ�
		MessageBox(L"ע��ɹ�");
		//���ص�½����
		m_pParentWnd->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
	}
	else if (rest.errType == account_exist)
	{
		MessageBox(L"�ʺ��Ѵ���");
	}
	else if (rest.errType == neekname_exist)
	{
		MessageBox(L"�ǳ��Ѵ���");
	}
	else
	{
		//����ʲô�����ҵ�Ҳ��֪��
		MessageBox(L"δ֪����ע��ʧ��");
	}
}

void cRegisterDialog::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pParentWnd->DestroyWindow();
	CDialogEx::OnClose();
}

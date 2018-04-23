// cMainClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chatClient.h"
#include "cMainClient.h"
#include "afxdialogex.h"
#include "cSearchDlg.h"
#include "cChatDlg.h"

// cMainClient �Ի���

IMPLEMENT_DYNAMIC(cMainClient, CDialogEx)

cMainClient::cMainClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(cMainClient::IDD, pParent)
{
	m_pSearchDlg = NULL;
}

cMainClient::~cMainClient()
{
}

void cMainClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBoxFriend);
}


BEGIN_MESSAGE_MAP(cMainClient, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &cMainClient::OnBnClickedBtnSearch)
	ON_LBN_DBLCLK(IDC_LIST1, &cMainClient::OnLbnDblclkList1)
END_MESSAGE_MAP()


// cMainClient ��Ϣ�������


void cMainClient::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pParentWnd->DestroyWindow();
	CDialogEx::OnClose();
}


void cMainClient::OnBnClickedBtnSearch()
{
	// ��������
	//�����������ѽ���
	if (!m_pSearchDlg)
	{
		m_pSearchDlg = new cSearchDlg(this);
		m_pSearchDlg->Create(IDD_DLG_SEARCH, this);
	}
	m_pSearchDlg->ShowWindow(SW_SHOW);
}


void cMainClient::SearchResult(stMsgSearchResult st)
{
	m_pSearchDlg->OnSearch(st);
}

void cMainClient::SetUserInfo(char* szAccount, char* szNeekName)
{
	if (szAccount == NULL || szNeekName == NULL)
	{
		return;
	}
	m_strNeekName=szNeekName;
	SetDlgItemText(IDC_NEEKNAME, m_strNeekName.GetString());
	m_strAccount = szAccount;
}


void cMainClient::QueryMakeFriend(stMsgQueryMakeFriend st)
{
	//����Ӻ�����������
	CString strAccountRest(st.szAccount);
	CString strTip;
	strTip.Format(L"�û� %s ���������Ϊ���ѣ��Ƿ�ͬ��",strAccountRest.GetString());
	if (MessageBox(strTip, L"��������", MB_YESNO) == IDYES)
	{
		//ͬ��
		stMsgMakeFirend msg;
		msg.head.msgType = msg_makeFriend;
		strcpy(msg.szAccount, st.szAccount);
		msg.bAgree = 1;
		SendToServer(&msg, sizeof(stMsgMakeFirend));
	}
	else
	{
		//�ܾ�
		stMsgMakeFirend st;
		st.head.msgType = msg_makeFriend;
		strcpy(st.szAccount, st.szAccount);
		st.bAgree = 0;
		SendToServer(&st, sizeof(stMsgMakeFirend));
	}
}

void cMainClient::MakeFriend(stMsgMakeFirend st)
{
	stFriendInfo info;
	strcpy(info.szAccount, st.szAccount);
	strcpy(info.szNeekName, "12345");
	m_listFriend.push_back(info);

	//�ں����б������һ��
	CString str(info.szAccount);
	m_listBoxFriend.AddString(str);
}

void cMainClient::OnLbnDblclkList1()
{
	//˫��ѡ��ĳ�����Ѿͷ���Ϣ
	int index = m_listBoxFriend.GetCurSel();
	CString strAccount;
	m_listBoxFriend.GetText(index, strAccount);
	cChatDlg* pDlg = new cChatDlg(strAccount, this);
	pDlg->Create(IDD_CHAT, this);
	strAccount.Format(L"�� %s ����", strAccount.GetString());
	pDlg->SetWindowTextW(strAccount);
	pDlg->ShowWindow(SW_SHOW);

	//��ӵ����촰��list
	m_listChatWnd.push_back(pDlg);
}


void cMainClient::RectMsg(stMsgSendMsg st)
{
	cChatDlg* pDlg = GetRecvDlg(st.szAccount);
	if (pDlg == NULL)
	{
		//Ϊ����˴���һ�����촰��
		return;
	}
	pDlg->AddMsg(st.szMsg);
}

cChatDlg* cMainClient::GetRecvDlg(CStringA strAccount)
{
	CString str(strAccount);
	for (list<cChatDlg*>::iterator it = m_listChatWnd.begin(); it != m_listChatWnd.end();++it)
	{
		if ((*it)->m_strDesAccount == str)
		{
			return *it;
		}
	}
	return NULL;
}

void cMainClient::RecvFile(stMsgSendFile st)
{
	cChatDlg* pChatDlg = GetRecvDlg(st.szAccount);
	if (pChatDlg == NULL)
	{
		return;
	}

	pChatDlg->RecvFile(st);
}
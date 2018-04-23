// cMainClient.cpp : 实现文件
//

#include "stdafx.h"
#include "chatClient.h"
#include "cMainClient.h"
#include "afxdialogex.h"
#include "cSearchDlg.h"
#include "cChatDlg.h"

// cMainClient 对话框

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


// cMainClient 消息处理程序


void cMainClient::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_pParentWnd->DestroyWindow();
	CDialogEx::OnClose();
}


void cMainClient::OnBnClickedBtnSearch()
{
	// 搜索好友
	//弹出搜索好友界面
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
	//有添加好友请求来了
	CString strAccountRest(st.szAccount);
	CString strTip;
	strTip.Format(L"用户 %s 请求添加你为好友，是否同意",strAccountRest.GetString());
	if (MessageBox(strTip, L"好友请求", MB_YESNO) == IDYES)
	{
		//同意
		stMsgMakeFirend msg;
		msg.head.msgType = msg_makeFriend;
		strcpy(msg.szAccount, st.szAccount);
		msg.bAgree = 1;
		SendToServer(&msg, sizeof(stMsgMakeFirend));
	}
	else
	{
		//拒绝
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

	//在好友列表中添加一行
	CString str(info.szAccount);
	m_listBoxFriend.AddString(str);
}

void cMainClient::OnLbnDblclkList1()
{
	//双击选中某个好友就发消息
	int index = m_listBoxFriend.GetCurSel();
	CString strAccount;
	m_listBoxFriend.GetText(index, strAccount);
	cChatDlg* pDlg = new cChatDlg(strAccount, this);
	pDlg->Create(IDD_CHAT, this);
	strAccount.Format(L"和 %s 聊天", strAccount.GetString());
	pDlg->SetWindowTextW(strAccount);
	pDlg->ShowWindow(SW_SHOW);

	//添加到聊天窗口list
	m_listChatWnd.push_back(pDlg);
}


void cMainClient::RectMsg(stMsgSendMsg st)
{
	cChatDlg* pDlg = GetRecvDlg(st.szAccount);
	if (pDlg == NULL)
	{
		//为这个人创建一个聊天窗口
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
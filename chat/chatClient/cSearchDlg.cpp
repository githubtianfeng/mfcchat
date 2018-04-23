// cSearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chatClient.h"
#include "cSearchDlg.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"


// cSearchDlg 对话框

IMPLEMENT_DYNAMIC(cSearchDlg, CDialogEx)

cSearchDlg::cSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(cSearchDlg::IDD, pParent)
{

}

cSearchDlg::~cSearchDlg()
{
}

void cSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_searchList);
}


BEGIN_MESSAGE_MAP(cSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &cSearchDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ADD, &cSearchDlg::OnBnClickedBtnAdd)
END_MESSAGE_MAP()


// cSearchDlg 消息处理程序


void cSearchDlg::OnBnClickedBtnSearch()
{
	// 确认搜索
	CString strText;
	GetDlgItemText(IDC_ACCOUNT, strText);
	if (strText.GetLength() == 0)
	{
		MessageBox(L"输入点文字吧");
		return;
	}
	m_searchList.RemoveAllGroups();

	//发送消息给服务器
	stMsgSearch msg;
	msg.head.msgType = msg_search;
	CStringA str(strText);
	strcpy(msg.szName, str.GetString());
	SendToServer(&msg, sizeof(stMsgSearch));
}


void cSearchDlg::OnBnClickedBtnAdd()
{
	//添加好友
	int index = m_searchList.GetSelectionMark();
	if (index == -1)
	{
		MessageBox(L"请选择一个人");
		return;
	}
	
	//获得这个item的信息
	CString strAccount = m_searchList.GetItemText(index, 0);
	//判断当前用户是否已经在我的好友列表中

	//发送好友请求给服务器
	stMsgQueryMakeFriend msg;
	msg.head.msgType = msg_makeQueryFriend;
	CStringA str(strAccount);
	strcpy(msg.szAccount, str.GetString());
	SendToServer(&msg, sizeof(stMsgQueryMakeFriend));
}


BOOL cSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	DWORD dwStype = m_searchList.GetExtendedStyle();

	dwStype |= LVS_EX_FULLROWSELECT;
	m_searchList.SetExtendedStyle(dwStype);


	//设置一下查找结果列表属性
	m_searchList.InsertColumn(0, L"帐号", 0, 100);
	m_searchList.InsertColumn(1, L"昵称", 0, 100);
	m_searchList.InsertColumn(2, L"是否在线", 0, 100);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void cSearchDlg::OnSearch(stMsgSearchResult st)
{
	CString strAccount(st.searchUserInfo.szAccout);
	int count = m_searchList.GetItemCount();
	m_searchList.InsertItem(count, strAccount.GetString());
	CString strNeekName(st.searchUserInfo.szNeekName);
	m_searchList.SetItemText(count, 1, strNeekName.GetString());
	CString strOnline = L"在线";
	if (st.searchUserInfo.bOnLine == 0)
	{
		strOnline = L"不在线";
	}
	m_searchList.SetItemText(count, 2, strOnline.GetString());
}
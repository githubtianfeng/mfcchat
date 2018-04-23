// cSearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chatClient.h"
#include "cSearchDlg.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"


// cSearchDlg �Ի���

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


// cSearchDlg ��Ϣ�������


void cSearchDlg::OnBnClickedBtnSearch()
{
	// ȷ������
	CString strText;
	GetDlgItemText(IDC_ACCOUNT, strText);
	if (strText.GetLength() == 0)
	{
		MessageBox(L"��������ְ�");
		return;
	}
	m_searchList.RemoveAllGroups();

	//������Ϣ��������
	stMsgSearch msg;
	msg.head.msgType = msg_search;
	CStringA str(strText);
	strcpy(msg.szName, str.GetString());
	SendToServer(&msg, sizeof(stMsgSearch));
}


void cSearchDlg::OnBnClickedBtnAdd()
{
	//��Ӻ���
	int index = m_searchList.GetSelectionMark();
	if (index == -1)
	{
		MessageBox(L"��ѡ��һ����");
		return;
	}
	
	//������item����Ϣ
	CString strAccount = m_searchList.GetItemText(index, 0);
	//�жϵ�ǰ�û��Ƿ��Ѿ����ҵĺ����б���

	//���ͺ��������������
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


	//����һ�²��ҽ���б�����
	m_searchList.InsertColumn(0, L"�ʺ�", 0, 100);
	m_searchList.InsertColumn(1, L"�ǳ�", 0, 100);
	m_searchList.InsertColumn(2, L"�Ƿ�����", 0, 100);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void cSearchDlg::OnSearch(stMsgSearchResult st)
{
	CString strAccount(st.searchUserInfo.szAccout);
	int count = m_searchList.GetItemCount();
	m_searchList.InsertItem(count, strAccount.GetString());
	CString strNeekName(st.searchUserInfo.szNeekName);
	m_searchList.SetItemText(count, 1, strNeekName.GetString());
	CString strOnline = L"����";
	if (st.searchUserInfo.bOnLine == 0)
	{
		strOnline = L"������";
	}
	m_searchList.SetItemText(count, 2, strOnline.GetString());
}
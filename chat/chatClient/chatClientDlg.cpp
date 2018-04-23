
// chatClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chatClient.h"
#include "chatClientDlg.h"
#include "afxdialogex.h"
#include "cRegisterDialog.h"
#include "../public/MsgDef.h"
#include "cMainClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int g_curVer = 2;	//�ͻ��˵�ǰ�汾��
int g_iStatus = 0;	//0���뻹δ�Ƚϰ汾�ţ�1��������°汾����ͬ��2����汾�Ų�ͬ����Ҫ����

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEditAccount();
};

void CAboutDlg::OnEnKillfocusEditAccount()
{
	//�����ʺ���
}


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_ACCOUNT, &CAboutDlg::OnEnKillfocusEditAccount)
END_MESSAGE_MAP()


// CchatClientDlg �Ի���

CchatClientDlg* g_LoginWnd = NULL;

CchatClientDlg::CchatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CchatClientDlg::IDD, pParent)
	, m_account(_T(""))
	, m_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_account);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}

BEGIN_MESSAGE_MAP(CchatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CchatClientDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CchatClientDlg::OnBnClickedBtnRegister)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CchatClientDlg ��Ϣ�������

void  ClientNetCallBack(DWORD id, void* param, int len)
{
	//����������Ϣ���͹���
	stMsgHead head;
	memcpy(&head, param, sizeof(head));
	switch (head.msgType)
	{
	case msg_registerRusult:
	{
		stMsgResult rest;
		memcpy(&rest, param, sizeof(stMsgResult));
		g_LoginWnd->m_pRegisterDlg->RegisterResult(rest);
		break;
	}
	case msg_loginResult:
	{
		stMsgLoginResult rest;
		memcpy(&rest, param, sizeof(stMsgLoginResult));
		g_LoginWnd->LoginResult(rest);
		break;
	}
	case msg_searchResult:
	{
		stMsgSearchResult res;
		memcpy(&res, param, sizeof(stMsgSearchResult));
		g_LoginWnd->SearchResult(res);
		break;
	}
	case msg_QueryFriendResul:
	{
		stMsgQueryMakeFriendResult rest;
		memcpy(&rest, param, sizeof(stMsgQueryMakeFriendResult));
		g_LoginWnd->QueryMakeFriendResult(rest);
		break;
	}
	case msg_makeQueryFriend:
	{
		stMsgQueryMakeFriend msg;
		memcpy(&msg, param, sizeof(stMsgQueryMakeFriend));
		g_LoginWnd->QueryMakeFriend(msg);
		break;
	}
	case msg_makeFriend:
	{
		stMsgMakeFirend msg;
		memcpy(&msg, param, sizeof(stMsgMakeFirend));
		g_LoginWnd->MakeFriend(msg);
		break;
	}
	case  msg_sendMsg:
	{
		stMsgSendMsg msg;
		memcpy(&msg, param, sizeof(stMsgSendMsg));
		g_LoginWnd->RecvMsg(msg);
		break;
	}
	case msg_sendFile:
	{
		stMsgSendFile msg;
		memcpy(&msg, param, sizeof(stMsgSendFile));
		g_LoginWnd->RecvFile(msg);
		break;
	}
	case msg_version:
	{
		if (sizeof(stMsgVer) != len)
		{
			return;
		}
		stMsgVer msg;
		memcpy(&msg, param, len);
		if (msg.ver != g_curVer)
		{
			g_iStatus = 2;
			break;
			//�����Զ����³���
		//	ShellExecute(NULL, L"open", L"updateClient.exe", NULL, NULL, SW_SHOW);
			//
			//g_LoginWnd->DestroyWindow();
		}
		g_iStatus = 1;
		break;
	}
	default:
		break;
	}
}

BOOL CchatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_LoginWnd = this;
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_pMainClientWnd = NULL;
	m_pRegisterDlg = NULL;
	/*AllocConsole();
	freopen("conin$", "w", stdin);
	freopen("conout$", "w", stdout);*/
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��������
	InitNet(2222, ClientNetCallBack, false, "127.0.0.1");

	//����һ����Ϣ��������������汾��
	RequeryVer();

	m_pMainClientWnd = new cMainClient(this);
	m_pMainClientWnd->Create(IDD_DLG_MAIN, this);

	SetTimer(1, 1000, NULL);
	MessageBox(L"�¿ͻ�������");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CchatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CchatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CchatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatClientDlg::OnBnClickedBtnLogin()
{
	if (g_iStatus == 0)
	{
		//�汾δ���£���δ���ӵ�����
	}
	//��½
	UpdateData(TRUE);
	CStringA strAccount(m_account);
	CStringA strPassword(m_password);
	//���������һЩ�ж�

	//���͸�������
	stMsgLogin stMsg;
	stMsg.head.msgType = msg_login;
	strcpy(stMsg.szAccount, strAccount.GetString());
	strcpy(stMsg.szPassword, strPassword.GetString());
	SendToServer(&stMsg, sizeof(stMsgLogin));
}


void CchatClientDlg::OnBnClickedBtnRegister()
{
	//ע��
	if (!m_pRegisterDlg)
	{
		m_pRegisterDlg = new cRegisterDialog(this);
		m_pRegisterDlg->Create(IDD_REGISTER_DIALOG, this);
	}
	
	m_pRegisterDlg->ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);
}

void CchatClientDlg::LoginResult(stMsgLoginResult st)
{
	//��½��� 
	if (st.errType == account_notexist)
	{
		MessageBox(L"�ʺŲ�����");
	}
	else if (st.errType  == password_err)
	{
		MessageBox(L"�������");
	}
	else if (st.errType == noerr)
	{
		//��½�ɹ�
		//������Ҫ���� ����½����Ҫ����

		if (m_pMainClientWnd)
		{
			m_pMainClientWnd->SetUserInfo(st.szAccount,st.szNeekNsame);
			m_pMainClientWnd->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
	
	}
	else
	{
		//������
	}
}

void CchatClientDlg::SearchResult(stMsgSearchResult st)
{
	m_pMainClientWnd->SearchResult(st);
}

void CchatClientDlg::QueryMakeFriendResult(stMsgQueryMakeFriendResult st)
{
	if (st.errType == account_notexist)
	{
		//�ʺŲ�����
		::MessageBox(NULL, L"�û�������", L"����", MB_OK);
	}
	else if (st.errType == err_noOnLine)
	{
		::MessageBox(NULL, L"�û�������", L"����", MB_OK);
	}
}

void CchatClientDlg::RequeryVer()
{
	//��ȡ�ͻ��˰汾��
/*	FILE* pFile = fopen("clientVer.txt", "r");
	if (pFile == NULL)
	{
		return;
	}

	char buf[102] = { 0 };
	fgets(buf, 100, pFile);
	g_curVer = atoi(buf);

	fclose(pFile);*/

	//��������ǰ���¿ͻ��˰汾����Ϣ
	stMsgVer msg;
	msg.head.msgType = msg_version;
	SendToServer(&msg, sizeof(msg));
}

void CchatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	//���

	if (g_iStatus == 0)
	{
	}
	else if (g_iStatus == 1)
		KillTimer(1);
	else
	{
		ShellExecute(NULL, L"open", L"updateClient.exe", NULL, NULL, SW_SHOW);
		//
	//	Sleep(1000);
		DestroyWindow();
	}


	CDialogEx::OnTimer(nIDEvent);
}

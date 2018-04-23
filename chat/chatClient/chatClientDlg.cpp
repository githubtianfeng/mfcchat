
// chatClientDlg.cpp : 实现文件
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

int g_curVer = 2;	//客户端当前版本号
int g_iStatus = 0;	//0代码还未比较版本号，1代表和最新版本号相同，2代码版本号不同，需要更新

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEditAccount();
};

void CAboutDlg::OnEnKillfocusEditAccount()
{
	//填完帐号了
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


// CchatClientDlg 对话框

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


// CchatClientDlg 消息处理程序

void  ClientNetCallBack(DWORD id, void* param, int len)
{
	//服务器有消息发送过来
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
			//启动自动更新程序
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
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_pMainClientWnd = NULL;
	m_pRegisterDlg = NULL;
	/*AllocConsole();
	freopen("conin$", "w", stdin);
	freopen("conout$", "w", stdout);*/
	// TODO:  在此添加额外的初始化代码
	//连接网络
	InitNet(2222, ClientNetCallBack, false, "127.0.0.1");

	//发送一个消息给服务器，请求版本号
	RequeryVer();

	m_pMainClientWnd = new cMainClient(this);
	m_pMainClientWnd->Create(IDD_DLG_MAIN, this);

	SetTimer(1, 1000, NULL);
	MessageBox(L"新客户端运行");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatClientDlg::OnBnClickedBtnLogin()
{
	if (g_iStatus == 0)
	{
		//版本未更新，或未连接到网络
	}
	//登陆
	UpdateData(TRUE);
	CStringA strAccount(m_account);
	CStringA strPassword(m_password);
	//最好这里做一些判断

	//发送给服务器
	stMsgLogin stMsg;
	stMsg.head.msgType = msg_login;
	strcpy(stMsg.szAccount, strAccount.GetString());
	strcpy(stMsg.szPassword, strPassword.GetString());
	SendToServer(&stMsg, sizeof(stMsgLogin));
}


void CchatClientDlg::OnBnClickedBtnRegister()
{
	//注册
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
	//登陆结果 
	if (st.errType == account_notexist)
	{
		MessageBox(L"帐号不存在");
	}
	else if (st.errType  == password_err)
	{
		MessageBox(L"密码错误");
	}
	else if (st.errType == noerr)
	{
		//登陆成功
		//主界面要出现 ，登陆界面要隐藏

		if (m_pMainClientWnd)
		{
			m_pMainClientWnd->SetUserInfo(st.szAccount,st.szNeekNsame);
			m_pMainClientWnd->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
	
	}
	else
	{
		//出错了
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
		//帐号不存在
		::MessageBox(NULL, L"用户不存在", L"错误", MB_OK);
	}
	else if (st.errType == err_noOnLine)
	{
		::MessageBox(NULL, L"用户不在线", L"错误", MB_OK);
	}
}

void CchatClientDlg::RequeryVer()
{
	//读取客户端版本号
/*	FILE* pFile = fopen("clientVer.txt", "r");
	if (pFile == NULL)
	{
		return;
	}

	char buf[102] = { 0 };
	fgets(buf, 100, pFile);
	g_curVer = atoi(buf);

	fclose(pFile);*/

	//发送请求当前最新客户端版本号消息
	stMsgVer msg;
	msg.head.msgType = msg_version;
	SendToServer(&msg, sizeof(msg));
}

void CchatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	//检测

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

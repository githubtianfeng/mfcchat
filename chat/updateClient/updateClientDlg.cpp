
// updateClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "updateClient.h"
#include "updateClientDlg.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WriteUpdateFile(stMsgUpdata st);

CupdateClientDlg* g_pDlg = NULL;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
int g_curVer = 1;	//�ͻ��˵�ǰ�汾��
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CupdateClientDlg �Ի���



CupdateClientDlg::CupdateClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CupdateClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pDlg = this;
}

void CupdateClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(CupdateClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


void MsgCallBack(DWORD id, void* param, int len)
{
	stMsgHead head;
	memcpy(&head, param, sizeof(head));
	switch (head.msgType)
	{
	case msg_UpdateFile:
	{
		if (sizeof(stMsgUpdata) != len)
		{
			return;
		}
		stMsgUpdata msg;
		memcpy(&msg, param, len);

		WriteUpdateFile(msg);
	}
		break;
	}
}

// CupdateClientDlg ��Ϣ�������

BOOL CupdateClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitNet(2222, MsgCallBack, false, "127.0.0.1");

	//��ȡ�汾��
/*	FILE* pFile = fopen("clientVer.txt", "r");
	if (pFile == NULL)
	{
		return TRUE;
	}

	char buf[102] = { 0 };
	fgets(buf, 100, pFile);
	g_curVer = atoi(buf);*/

	//fclose(pFile);

	UpdateFiles();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CupdateClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CupdateClientDlg::OnPaint()
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
HCURSOR CupdateClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CupdateClientDlg::UpdateFiles()
{
	//������Ϣ���߷�������Ҫ������
	stMsgUpdata msg;
	msg.curVersion = g_curVer;
	msg.head.msgType = msg_UpdateFile;
	SendToServer(&msg, sizeof(stMsgUpdata));
}

FILE* pFile = NULL;
DWORD g_curSize = 0;
DWORD g_allSize = 0;

void WriteUpdateFile(stMsgUpdata st)
{
	if (pFile == NULL)
	{
		g_allSize = st.allSize;
		g_pDlg->m_progress.SetPos(0);
		//CStringA strInfo;
		//strInfo.
		//MessageBox(L")
		//˵�����ǵڴη��͹�����
		pFile = fopen(st.szFileName, "wb");
		if (pFile == NULL)
		{
			return;
		}
	}
	//pFile�϶���Ϊ��
	size_t a = fwrite(st.szFile, 1, st.size, pFile);
	g_curSize += st.size;
	
	g_pDlg->m_progress.SetPos(100*g_curSize/g_allSize);//0-100��ֵ,��ǰ��С���ܴ�С

	//�ж��Ƿ�����ȫ�������ɾ�ɾ��
	if (st.bFinish)
	{
		fclose(pFile);
		pFile = NULL;

		//�رյ�ǰ���� ����chatClient.exe
	}
}
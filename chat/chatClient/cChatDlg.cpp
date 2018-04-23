// cChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chatClient.h"
#include "cChatDlg.h"
#include "afxdialogex.h"
#include "../public/MsgDef.h"

cChatDlg* g_pDlg = NULL;

// cChatDlg 对话框

IMPLEMENT_DYNAMIC(cChatDlg, CDialogEx)

cChatDlg::cChatDlg(CString strDesAccount, CWnd* pParent /*=NULL*/)
	: CDialogEx(cChatDlg::IDD, pParent)
	, m_sendStr(_T(""))
{
	m_strDesAccount = strDesAccount;
	g_pDlg = this;
}

cChatDlg::~cChatDlg()
{
}

void cChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_sendStr);
}


BEGIN_MESSAGE_MAP(cChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &cChatDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_SENDFILE, &cChatDlg::OnBnClickedSendfile)
END_MESSAGE_MAP()


// cChatDlg 消息处理程序


void cChatDlg::OnBnClickedBtnSend()
{
	//发送消息
	UpdateData(TRUE);
	if (m_sendStr.GetLength() == 0)
	{
		return;
	}
	CStringA str(m_strDesAccount);
	stMsgSendMsg msg;
	msg.head.msgType = msg_sendMsg;
	strcpy(msg.szAccount, str.GetString());
	CStringA strMsg(m_sendStr);
	strcpy(msg.szMsg, strMsg.GetString());
	SendToServer(&msg, sizeof(stMsgSendMsg));

	CString strMsgAll;
	GetDlgItemText(IDC_RECV_MSG, strMsgAll);
	strMsgAll += L"\r\n                  ";
	strMsgAll += L"自己";
	strMsgAll += L"\r\n    ";
	strMsgAll += strMsg;
	SetDlgItemText(IDC_RECV_MSG, strMsgAll);
}

void cChatDlg::AddMsg(CStringA str)
{
	CString strNew(str);
	CString strMsgAll;
	GetDlgItemText(IDC_RECV_MSG, strMsgAll);
	strMsgAll += L"\r\n";
	strMsgAll += m_strDesAccount;
	strMsgAll += L"\r\n    ";
	strMsgAll += strNew;
	SetDlgItemText(IDC_RECV_MSG, strMsgAll);
}

DWORD WINAPI SendFileThread(void* param)
{
	stSendFileInfo* pInfo = (stSendFileInfo*)param;
//	CString str1((WCHAR*)param);
	CStringA strPath = pInfo->strPath;
	CStringA strName = pInfo->strName;
	FILE* pFile = fopen(strPath.GetString(), "rb");
	if (pFile == NULL)
	{
		return 0;
	}
	char buf[1024] = { 0 };
	int ret = 0;
	ret = fread(buf, 1, 1024, pFile);

	while (ret)
	{
		//发送读取到的文件 信息给对方
		//要获得是哪个
		stMsgSendFile msg;
		msg.head.msgType = msg_sendFile;
		CStringA strAccount(g_pDlg->m_strDesAccount);
		strcpy(msg.szAccount,strAccount.GetString());
		strcpy(msg.szFileName, strName.GetString());
		memcpy(msg.szFile, buf,ret);
		msg.size = ret;
		msg.bFinish = 0;
		ret = fread(buf, 1, 1024, pFile);
		if (!ret)
		{
			//传输完成
		//	cout << "传输完成" << endl;
			msg.bFinish = 1;
		}
		SendToServer(&msg, sizeof(stMsgSendFile));
		Sleep(20);
	}
	fclose(pFile);
	_endthread();
	return 0;
}

void cChatDlg::OnBnClickedSendfile()
{
	//发送文件
	CFileDialog file(TRUE);
	if (file.DoModal() == IDOK)
	{
		CString strPath = file.GetPathName();
		CString strName = file.GetFileName();

		m_strFilePath.push_back(strPath);
		stSendFileInfo st(strPath,strName);
		CreateThread(0, 0, SendFileThread, (void*)&st, 0, 0);
		Sleep(10);
	}
}


void cChatDlg::RecvFile(stMsgSendFile st)
{
	FILE* pFile = NULL;
	for (list<stRecvFileInfo>::iterator it = m_recvFileList.begin();it != m_recvFileList.end();++it)
	{
		if ((*it).strName == st.szFileName)
		{
			pFile = (*it).pFile;
		}
	}
	if (pFile == NULL)
	{
		//CStringA strInfo;
		//strInfo.
		//MessageBox(L")
		//说明这是第次发送过来的
		pFile = fopen("d:/data.abc", "wb");
		if (pFile == NULL)
		{
			return;
		}
		stRecvFileInfo info;
		info.strName = st.szFileName;
		info.pFile = pFile;
		m_recvFileList.push_back(info);
	}
	//pFile肯定不为空
	fwrite(st.szFile, 1, st.size, pFile);
	
	//判断是否传输完全，如果完成就删除
	if (st.bFinish)
	{
		fclose(pFile);
		for (list<stRecvFileInfo>::iterator it = m_recvFileList.begin(); it != m_recvFileList.end();++it)
		{
			if (pFile == (*it).pFile)
			{
				m_recvFileList.erase(it);
				break;
			}
		}
	}
}
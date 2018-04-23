#pragma once
#include <list>
#include "../public/MsgDef.h"
using namespace std;

// cChatDlg 对话框

struct stRecvFileInfo
{
	CStringA strName;
	FILE* pFile;
};

struct stSendFileInfo
{
	CStringA strPath;
	CStringA strName;
	stSendFileInfo(CString path, CString name) :strPath(path), strName(name)
	{

	}
};

class cChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(cChatDlg)

public:
	cChatDlg(CString strDesAccount,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cChatDlg();

// 对话框数据
	enum { IDD = IDD_CHAT };

	//当前窗口的聊天对象
	CString m_strDesAccount;

	list<CString> m_strFilePath;

	//添加一个信息
	void AddMsg(CStringA str);

	//接收文件
	void RecvFile(stMsgSendFile st);

	list<stRecvFileInfo> m_recvFileList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSend();
	CString m_sendStr;
	afx_msg void OnBnClickedSendfile();
};

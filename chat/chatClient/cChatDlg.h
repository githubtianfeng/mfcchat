#pragma once
#include <list>
#include "../public/MsgDef.h"
using namespace std;

// cChatDlg �Ի���

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
	cChatDlg(CString strDesAccount,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cChatDlg();

// �Ի�������
	enum { IDD = IDD_CHAT };

	//��ǰ���ڵ��������
	CString m_strDesAccount;

	list<CString> m_strFilePath;

	//���һ����Ϣ
	void AddMsg(CStringA str);

	//�����ļ�
	void RecvFile(stMsgSendFile st);

	list<stRecvFileInfo> m_recvFileList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSend();
	CString m_sendStr;
	afx_msg void OnBnClickedSendfile();
};

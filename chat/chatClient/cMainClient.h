#pragma once
#include "../public/MsgDef.h"
#include "cSearchDlg.h"
#include <list>
#include "afxwin.h"

class cChatDlg;
// cMainClient 对话框

class cMainClient : public CDialogEx
{
	DECLARE_DYNAMIC(cMainClient)

public:
	cMainClient(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cMainClient();

	//设置当前用户信息
	void SetUserInfo(char* szAccount, char* szNeekName);

// 对话框数据
	enum { IDD = IDD_DLG_MAIN };

	cSearchDlg* m_pSearchDlg;	//搜索好友界面

	CString m_strNeekName;
	CString m_strAccount;

	//好友列表
	std::list<stFriendInfo> m_listFriend;

	//聊天窗口列表
	std::list<cChatDlg*> m_listChatWnd;

	//查找结果
	void SearchResult(stMsgSearchResult st);

	//别人的添加好好请求
	void QueryMakeFriend(stMsgQueryMakeFriend st);

	//添加好友
	void MakeFriend(stMsgMakeFirend st);

	//接收消息
	void RectMsg(stMsgSendMsg st);

	//通过帐号获得聊天窗口
	cChatDlg* GetRecvDlg(CStringA strAccount);

	//接收传文件消息
	void RecvFile(stMsgSendFile st);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSearch();
	CListBox m_listBoxFriend;
	afx_msg void OnLbnDblclkList1();
};

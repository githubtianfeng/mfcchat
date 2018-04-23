#pragma once
#include "afxcmn.h"
#include "../public/MsgDef.h"

// cSearchDlg 对话框

class cSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(cSearchDlg)

public:
	cSearchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cSearchDlg();


	//搜索结果 
	void OnSearch(stMsgSearchResult st);

// 对话框数据
	enum { IDD = IDD_DLG_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnAdd();
	CListCtrl m_searchList;
	virtual BOOL OnInitDialog();
};

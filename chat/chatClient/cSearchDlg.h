#pragma once
#include "afxcmn.h"
#include "../public/MsgDef.h"

// cSearchDlg �Ի���

class cSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(cSearchDlg)

public:
	cSearchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cSearchDlg();


	//������� 
	void OnSearch(stMsgSearchResult st);

// �Ի�������
	enum { IDD = IDD_DLG_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnAdd();
	CListCtrl m_searchList;
	virtual BOOL OnInitDialog();
};

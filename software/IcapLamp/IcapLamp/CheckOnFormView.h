#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"


// CCheckOnFormView 窗体视图

class CCheckOnFormView : public CFormView
{
	DECLARE_DYNCREATE(CCheckOnFormView)

public:
	CCheckOnFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCheckOnFormView();

public:
	enum { IDD = IDD_FORMVIEW_CHECKON };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnInitialUpdate();
	afx_msg void OnBnClickedButtonCheckonShow();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_checkOnListCtrl;
private:
	void GetCheckOnRecord(const CTime &time);
public:
	CDateTimeCtrl m_dateTime;
};



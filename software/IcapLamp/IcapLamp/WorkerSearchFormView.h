#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CWorkerSearchFormView 窗体视图

class CWorkerSearchFormView : public CFormView
{
	DECLARE_DYNCREATE(CWorkerSearchFormView)

public:
	CWorkerSearchFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkerSearchFormView();

public:
	enum { IDD = IDD_FORMVIEW_SEARCH };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnInitialUpdate();
	afx_msg void OnBnClickedButtonSearchShow();

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_workerSearchListCtrl;
	CEdit m_workerNameEdit;
	CDateTimeCtrl m_searchBeginTime;
};



#pragma once
#include "afxcmn.h"



// CDirectionMonitorFormView 窗体视图

class CDirectionMonitorFormView : public CFormView
{
	DECLARE_DYNCREATE(CDirectionMonitorFormView)

public:
	CDirectionMonitorFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDirectionMonitorFormView();

public:
	enum { IDD = IDD_FORMVIEW_MONITOR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

private:
	void GetMonitorRecord(void);

private:
	CListCtrl m_directionMonitorList;
public:
	afx_msg void OnBnClickedButtonMonitor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

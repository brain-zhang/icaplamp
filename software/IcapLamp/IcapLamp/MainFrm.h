
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "CalendarBar.h"
#include "Resource.h"
#include "ScanAPThread.h"

#define NUMVIEWS 5


class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:
	void InitViews ();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutlookBar       m_wndNavigationBar;
	CMFCOutlookBarPane	m_wndShortcutsPane1;
	CMFCCaptionBar		m_wndCaptionBar;
	CCalendarBar      m_wndCalendar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnOutlookNavigation(UINT id);
	afx_msg void OnUpdateOutlookNavigation(CCmdUI* pCmdUI);
	afx_msg void OnViewNavigation();
	afx_msg void OnUpdateViewNavigation(CCmdUI *pCmdUI);

	//处理自定义消息
	afx_msg LRESULT OnStatusMessage(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

private:
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CCalendarBar& calendar, int nInitialWidth);
	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

private:
	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

	// Array of views attached to single document
	CView * m_pViews[NUMVIEWS];
	// Index to current view
	UINT m_nCurView;

	CList<CSCanAPThread *, CSCanAPThread *> m_pScanAPThreadList;

public:
	afx_msg void OnClose();
};



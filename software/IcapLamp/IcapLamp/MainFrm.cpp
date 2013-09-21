
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "IcapLamp.h"

#include "MainFrm.h"
#include "SendMessageFormView.h"
#include "CheckOnFormView.h"
#include "SetupFormView.h"
#include "WorkerSearchFormView.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_NAVIGATION, &CMainFrame::OnViewNavigation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAVIGATION, &CMainFrame::OnUpdateViewNavigation)
	ON_COMMAND_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_5, OnOutlookNavigation)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHORTCUT_1, ID_SHORTCUT_5, OnUpdateOutlookNavigation)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STATUS_MESSAGE, OnStatusMessage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2003);

	//扫描AP，启动对应数目的线程开始扫描工作
	CModel *pmodel = CSingleton<CModel>::instance();
	std::vector<std::string> vec_ip_pool = pmodel->GetAPIPList();

	for (std::vector<std::string>::iterator iter = vec_ip_pool.begin(); iter != vec_ip_pool.end(); iter++)
	{
		CSCanAPThread *pscan_thread = new CSCanAPThread(*iter);
		pscan_thread->CreateThread();
		m_pScanAPThreadList.AddTail(pscan_thread);
	}
}

CMainFrame::~CMainFrame()
{
	delete []m_pViews;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 导航窗格将创建在左侧，因此将暂时禁用左侧的停靠:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// 创建并设置“Outlook”导航栏:
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndCalendar, 250))
	{
		TRACE0("未能创建导航窗格\n");
		return -1;      // 未能创建
	}

	// 已创建 Outlook 栏，应允许在左侧停靠。
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // 未能创建
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	CImageList images;
	images.Create (IDB_SHORTCUTS, 32, 0, RGB (255, 0, 255));

	// Create first page:
	m_wndShortcutsPane1.Create (&bar, AFX_DEFAULT_TOOLBAR_STYLE, ID_SHORTCUTS_PANE_1);
	m_wndShortcutsPane1.SetOwner (this);
	m_wndShortcutsPane1.EnableTextLabels ();
	m_wndShortcutsPane1.EnableDocking (CBRS_ALIGN_ANY);

	m_wndShortcutsPane1.AddButton (images.ExtractIcon (0), _T("人员定位"), ID_SHORTCUT_1);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (1), _T("人员考勤"), ID_SHORTCUT_2);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (2), _T("发信息"), ID_SHORTCUT_3);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (3), _T("工人下井活动记录"), ID_SHORTCUT_4);
	m_wndShortcutsPane1.AddButton (images.ExtractIcon (4), _T("设置"), ID_SHORTCUT_5);
	pOutlookBar->AddTab (&m_wndShortcutsPane1, _T("基本功能区"), (UINT)-1, FALSE);


	// 可浮动，可自动隐藏，可调整大小，但不能关闭
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



void CMainFrame::OnViewNavigation()
{
	// TODO: 在此添加命令处理程序代码
	ShowPane (&m_wndNavigationBar, !(m_wndNavigationBar.IsVisible ()),	FALSE, TRUE);
	RecalcLayout ();
}


void CMainFrame::OnUpdateViewNavigation(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//pCmdUI->SetCheck (m_wndNavigationBar.IsVisible ());
}

void CMainFrame::OnOutlookNavigation(UINT id)
{
	const int nIndex = id - ID_SHORTCUT_1;

	ASSERT( nIndex >=0 && nIndex < NUMVIEWS );
	if ( nIndex < 0 || nIndex >= NUMVIEWS )
		return;

	if (!m_pViews)
		return;

	CView* pNewView = m_pViews[nIndex];
	if (!pNewView)
		return;

	CView* pActiveView =GetActiveView();

	if ( !pActiveView )    // No currently active view
		return;

	if ( pNewView == pActiveView )    // Already there
		return;

	m_nCurView = nIndex;    // Store the new current view's index

	// exchange view window ID's so RecalcLayout() works
	UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
	::SetWindowLong(pActiveView->m_hWnd, GWL_ID, ::GetWindowLong(pNewView->m_hWnd, GWL_ID));
	::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);

	// Display and update the new current view - hide the old one    
	pActiveView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);
	SetActiveView(pNewView);
	RecalcLayout();
	pNewView->Invalidate();

	//CString strCaption;
	//strCaption.Format (_T("功能 %d"), nIndex + 1);
	//m_wndCaptionBar.SetText (strCaption);
	//m_wndCaptionBar.RedrawWindow ();
}

void CMainFrame::OnUpdateOutlookNavigation(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (pCmdUI->m_nID == ID_SHORTCUT_1 + m_nCurView);
}

void CMainFrame::InitViews ()
{
	m_nCurView = 0;        // Save index of the currently active view class
	CView* pActiveView = GetActiveView();

	m_pViews[0] = pActiveView;
	m_pViews[1] = (CView*) new CCheckOnFormView();
	m_pViews[2] = (CView*) new CSendMessageFormView();
	m_pViews[3] = (CView*) new CWorkerSearchFormView();
	m_pViews[4] = (CView*) new CSetupFormView();

	CDocument* pCurrentDoc = GetActiveDocument();

	// Initialize a CCreateContext to point to the active document.
	// With this context, the new view is added to the document
	// when the view is created in CView::OnCreate().
	CCreateContext newContext;
	newContext.m_pNewViewClass = NULL;
	newContext.m_pNewDocTemplate = NULL;
	newContext.m_pLastView = NULL;
	newContext.m_pCurrentFrame = NULL;
	newContext.m_pCurrentDoc = pCurrentDoc;

	CRect rect(0, 0, 0, 0); // gets resized later

	for (int nView = 1; nView < NUMVIEWS; nView++)
	{
		// Create the new view. In this example, the view persists for
		// the life of the application. The application automatically
		// deletes the view when the application is closed.
		m_pViews[nView]->Create(NULL, NULL,
			(AFX_WS_DEFAULT_VIEW & ~WS_VISIBLE),
			// views are created with the style of AFX_WS_DEFAULT_VIEW
			// In MFC 4.0, this is (WS_BORDER | WS_VISIBLE | WS_CHILD)
			rect, this,
			AFX_IDW_PANE_FIRST + nView, &newContext);

		// When a document template creates a view, the WM_INITIALUPDATE
		// message is sent automatically. However, this code must
		// explicitly send the message, as follows.
		m_pViews [nView]->OnInitialUpdate();
	}
}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POSITION pos = m_pScanAPThreadList.GetHeadPosition();
	while(pos != NULL)
	{
		CSCanAPThread *pscan_thread = m_pScanAPThreadList.GetNext(pos);
		::PostThreadMessage(pscan_thread-> m_nThreadID, WM_QUIT, 0, 0); 
	}
	
	CFrameWndEx::OnClose();
}

//处理自定义消息
LRESULT CMainFrame::OnStatusMessage(WPARAM w, LPARAM l)
{
	static int count = 1;
	CString str_count;
	str_count.Format(_T("%d"), count);

	USES_CONVERSION;
	CString msg(A2W((char *)w));

	if(CString(_T("Error")) == msg)
	{
		CString ip(A2W((char *)l));
		CString status_text = _T("连接AP出现故障! 故障AP:") + ip;
		m_wndStatusBar.SetWindowText(status_text);
	}
	else
	{
		CString status_text = _T("正在扫描AP，IP:") + msg + _T(".......第") + str_count + _T("次扫描");
		m_wndStatusBar.SetWindowText(status_text);
	}
	count++;
	return 0;
}
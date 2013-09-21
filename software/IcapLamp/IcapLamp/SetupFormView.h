#pragma once



// CSetupFormView 窗体视图

class CSetupFormView : public CFormView
{
	DECLARE_DYNCREATE(CSetupFormView)

public:
	CSetupFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSetupFormView();

public:
	enum { IDD = IDD_FORMVIEW_SETUP };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};



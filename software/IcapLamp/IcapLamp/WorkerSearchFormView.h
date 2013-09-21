#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"



// CWorkerSearchFormView ������ͼ

class CWorkerSearchFormView : public CFormView
{
	DECLARE_DYNCREATE(CWorkerSearchFormView)

public:
	CWorkerSearchFormView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnInitialUpdate();
	afx_msg void OnBnClickedButtonSearchShow();

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_workerSearchListCtrl;
	CEdit m_workerNameEdit;
	CDateTimeCtrl m_searchBeginTime;
};



// SetupFormView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IcapLamp.h"
#include "SetupFormView.h"


// CSetupFormView

IMPLEMENT_DYNCREATE(CSetupFormView, CFormView)

CSetupFormView::CSetupFormView()
	: CFormView(CSetupFormView::IDD)
{

}

CSetupFormView::~CSetupFormView()
{
}

void CSetupFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetupFormView, CFormView)
END_MESSAGE_MAP()


// CSetupFormView ���

#ifdef _DEBUG
void CSetupFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSetupFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSetupFormView ��Ϣ�������

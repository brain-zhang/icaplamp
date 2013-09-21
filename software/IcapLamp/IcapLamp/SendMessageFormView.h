#include <iostream>

#include "afxwin.h"
#include "SockSendMessage.h"
#include "SendMessageThread.h"
#include "afxcmn.h"

// CSendMessageFormView 窗体视图

class CSendMessageFormView : public CFormView
{
	DECLARE_DYNCREATE(CSendMessageFormView)

public:
	CSendMessageFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSendMessageFormView();

public:
	enum { IDD = IDD_FORMVIEW_SENDMESSAGE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInitialUpdate();
	afx_msg void OnBnClickedBtnSendMessage();

DECLARE_MESSAGE_MAP()

private:
	CRect m_rectOriginalWindow;

private:
	CButton m_btnSendMessage;
	CEdit m_editMessage;
	CStatic m_grpRecord;
	CStatic m_grpSend;
	CListCtrl m_messageListCtrl;
	CSendMessageThread *m_pSendMessageThread;

private:
	//将新发送的消息存到数据库中
	void SaveMessageToDB(const std::string &message);
	void RefreshMessageEdit(void);
};
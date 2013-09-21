// SendMessageFormView.cpp : ʵ���ļ�
//
#include "stdafx.h"

#include <iostream>

#include "resource.h"
#include "SendMessageFormView.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Util.h"

// CSendMessageFormView

IMPLEMENT_DYNCREATE(CSendMessageFormView, CFormView)

CSendMessageFormView::CSendMessageFormView()
	: CFormView(CSendMessageFormView::IDD)
{
	m_pSendMessageThread = new CSendMessageThread();
	m_pSendMessageThread->CreateThread();
}

CSendMessageFormView::~CSendMessageFormView()
{
	delete m_pSendMessageThread;
	m_pSendMessageThread = NULL;
}

void CSendMessageFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SEND_MESSAGE, m_btnSendMessage);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_STATIC_RECORD, m_grpRecord);
	DDX_Control(pDX, IDC_STATIC_SEND, m_grpSend);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_messageListCtrl);
}

BEGIN_MESSAGE_MAP(CSendMessageFormView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SEND_MESSAGE, &CSendMessageFormView::OnBnClickedBtnSendMessage)
END_MESSAGE_MAP()


// CSendMessageFormView ���

#ifdef _DEBUG
void CSendMessageFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSendMessageFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSendMessageFormView ��Ϣ�������
void CSendMessageFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( m_grpRecord.m_hWnd != NULL)
	//�ж��Ƿ�Ϊ�棬Ϊ�棬��ʾ�ؼ��Ѿ�����ʼ���������������Խ��в�����
	//��������Ǵ��������ʱ���ô˺�������û�н���OnInitialUpdate()������
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		m_grpRecord.GetWindowRect(&rect);

		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������

		//��cx/m_rect.Width()Ϊ�Ի����ں���ı仯����
		rect.left=rect.left * cx / m_rectOriginalWindow.Width();
		rect.right=rect.right* cx / m_rectOriginalWindow.Width();
		rect.top=rect.top * cy / m_rectOriginalWindow.Height();
		rect.bottom=rect.bottom * cy / m_rectOriginalWindow.Height();

		m_grpRecord.MoveWindow(rect);//���ÿؼ���С
			
	}
	GetClientRect(&m_rectOriginalWindow);//���仯��ĶԻ����С��Ϊ�ɴ�С
}

void CSendMessageFormView::OnInitialUpdate() 
{
	m_messageListCtrl.SubclassDlgItem(IDC_MESSAGE_LIST, this);

	DWORD dwStyle = m_messageListCtrl.GetExtendedStyle();
	m_messageListCtrl.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);  
	m_messageListCtrl.InsertColumn(0, _T("������"));
	m_messageListCtrl.InsertColumn(1, _T("��Ϣ����"));
	m_messageListCtrl.InsertColumn(2, _T("����ʱ��"));


	m_messageListCtrl.SetColumnWidth(0, 80);
	m_messageListCtrl.SetColumnWidth(2, 140);

	CRect rect;
	m_messageListCtrl.GetWindowRect(&rect);
	m_messageListCtrl.SetColumnWidth(1, rect.Width() - 80 - 170);

	RefreshMessageEdit();
	GetClientRect(&m_rectOriginalWindow);
}

void CSendMessageFormView::OnBnClickedBtnSendMessage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString str; 
	GetDlgItem(IDC_EDIT_MESSAGE)->GetWindowText(str);

	if(0 == str.GetLength())
	{
		MessageBox(_T("���������ݣ�"), _T("��ʾ"), MB_ICONWARNING);
		return;
	}
	else if(str.GetLength() > 255)
	{
		MessageBox(_T("�����������ܳ���255���ַ���"));
		return;
	}

	//ת��utf-8�����ݿ�
	SaveMessageToDB(CUtil::EncodeUtf8(std::wstring(str.GetBuffer())));
	str.ReleaseBuffer();

	//������Ϣ
	unsigned char p[512] = {0};
	CUtil::GetGB2312String(str, (char *)p, sizeof(p));
	unsigned char crc_sum = CUtil::CRCCacu(p, strlen((char *)p));
	p[strlen((char *)p)] = crc_sum;
	p[strlen((char *)p)] = (unsigned char)0xFF;
	p[strlen((char *)p)] = (unsigned char)0xFF;
	m_pSendMessageThread->SetMessage((char *)p);
	m_pSendMessageThread->PostThreadMessage(WM_SENDMESSAGEDAEMON, NULL, NULL);

	//ˢ�½���
	RefreshMessageEdit();
}

void CSendMessageFormView::SaveMessageToDB(const std::string &message)
{
	/*
	CTime cur_time;
	cur_time = CTime::GetCurrentTime();
	CString strcur_time;                 
	strcur_time.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), 
		cur_time.GetYear(), cur_time.GetMonth(), cur_time.GetDay(), cur_time.GetHour(), cur_time.GetMinute(), cur_time.GetSecond());
	*/

	//д�����ݿ�
	CModel *pmodel = CSingleton<CModel>::instance();
	Field definition_tbPerson[] = 
	{
		Field(FIELD_KEY),
		Field("content", type_text),
		//Field("datetime", type_time),
		Field("dest", type_int),
		Field(DEFINITION_END),
	};
	Table tbMessage(pmodel->MGetDatabase(), std::string("Message"), definition_tbPerson);
	Record record(tbMessage.fields());

	record.setString("content", message);
	//record.setString("datetime", (LPCTSTR)strcur_time);
	record.setTime("dest", -1);
	tbMessage.addRecord(&record);
}

void CSendMessageFormView::RefreshMessageEdit()
{
	//�����ݿ�
	CModel *pmodel = CSingleton<CModel>::instance();
	Field definition_tbPerson[] = 
	{
		Field(FIELD_KEY),
		Field("content", type_text),
		Field("datetime", type_text),
		Field("dest", type_int),
		Field(DEFINITION_END),
	};
	Table tbMessage(pmodel->MGetDatabase(), std::string("Message"), definition_tbPerson);
	tbMessage.open();
	Record record(tbMessage.fields());

	m_messageListCtrl.DeleteAllItems();

	for (int index = 0; index < tbMessage.recordCount(); index++)
	{
		if (Record* record = tbMessage.getRecord(index))
		{
			int nRom = m_messageListCtrl.InsertItem(index, _T("test")); 
			//������ʾ������groupid
			m_messageListCtrl.SetItemText(nRom, 0, CUtil::DecodeUtf8(record->getValue(std::string("dest"))->asString()).c_str());
			//��Ϣ����
			m_messageListCtrl.SetItemText(nRom, 1, CUtil::DecodeUtf8(record->getValue(std::string("content"))->asString()).c_str());
			//����ʱ��
			m_messageListCtrl.SetItemText(nRom, 2, CUtil::DecodeUtf8(record->getValue(std::string("datetime"))->asString()).c_str());
		}
	}
	//m_messageListCtrl.UpdateWindow();
}
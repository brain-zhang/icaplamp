// CheckOnView.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include <iostream>

#include "IcapLamp.h"
#include "CheckOnFormView.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Util.h"


// CCheckOnFormView

IMPLEMENT_DYNCREATE(CCheckOnFormView, CFormView)

CCheckOnFormView::CCheckOnFormView()
	: CFormView(CCheckOnFormView::IDD)
{

}

CCheckOnFormView::~CCheckOnFormView()
{
}

void CCheckOnFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKON_LIST, m_checkOnListCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateTime);
}

BEGIN_MESSAGE_MAP(CCheckOnFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_CHECKON_SHOW, &CCheckOnFormView::OnBnClickedButtonCheckonShow)
END_MESSAGE_MAP()


// CCheckOnFormView ���

#ifdef _DEBUG
void CCheckOnFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCheckOnFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCheckOnFormView ��Ϣ�������


//�����ѯ��ť
void CCheckOnFormView::OnBnClickedButtonCheckonShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime timeTime;
	DWORD dwResult = m_dateTime.GetTime(timeTime);
	GetCheckOnRecord(timeTime);
}

//�����ʼ��
void CCheckOnFormView::OnInitialUpdate()
{
	m_checkOnListCtrl.SubclassDlgItem(IDC_CHECKON_LIST, this);
	m_dateTime.SubclassDlgItem(IDC_DATETIMEPICKER1, this);

	DWORD dwStyle = m_checkOnListCtrl.GetExtendedStyle();
	m_checkOnListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	m_checkOnListCtrl.InsertColumn(0, _T("�豸ID"));
	m_checkOnListCtrl.InsertColumn(1, _T("��������"));
	m_checkOnListCtrl.InsertColumn(2, _T("�¾�ʱ��"));
	m_checkOnListCtrl.InsertColumn(3, _T("�Ͼ�ʱ��"));

	m_checkOnListCtrl.SetColumnWidth(0, 80);
	m_checkOnListCtrl.SetColumnWidth(1, 100);
	m_checkOnListCtrl.SetColumnWidth(2, 160);
	m_checkOnListCtrl.SetColumnWidth(3, 160);
}


//�����ݿ��л�ȡ���ڼ�¼����ʾ��¼
//�˰汾���ܼ������ƣ�������ʾʹ��
void CCheckOnFormView::GetCheckOnRecord(const CTime &time)
{
	CTime check_time(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
	char str_ascii[128] = {0};
	CUtil::GetAnsiString(check_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_check_time_begin(str_ascii);

	check_time += 60 * 60 * 24 - 1;
	memset(str_ascii, 0, sizeof(str_ascii));
	CUtil::GetAnsiString(check_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_check_time_end(str_ascii);


	//�����ѯ�ַ���
	std::string sql_get_first_records = 
		std::string("select A._ID, Worker.DeviceID, Worker.name,A.datetime  from (SELECT * FROM  (select * from DirectionMonitor where APID = 1 and datetime between \"") +
		str_check_time_begin + std::string("\" and \"") + str_check_time_end + 
		std::string("\" order by datetime DESC) group by DeviceID) as A, Worker where Worker.DeviceID = A.DeviceID");
	std::string sql_get_last_records = 
		std::string("SELECT * FROM (select * from DirectionMonitor where APID = 1  and datetime between \"") + 
		str_check_time_begin + std::string("\" and \"") + str_check_time_end + 
		std::string("\" order by datetime ASC) group by DeviceID");
	
	CModel *pmodel = CSingleton<CModel>::instance();
	Field definition_tbCheckOn[] = 
	{
		Field(FIELD_KEY),
		Field("DeviceID", type_int),
		Field("name", type_text),
		Field("datetime", type_time),
		Field(DEFINITION_END),
	};

	Field definition_tbCheckOnLast[] = 
	{
		Field(FIELD_KEY),
		Field("DeviceID", type_int),
		Field("APID", type_int),
		Field("datetime", type_time),
		Field(DEFINITION_END),
	};
	RecordSet rs_check_on(pmodel->MGetDatabase(), definition_tbCheckOn);
	rs_check_on.query(sql_get_first_records);

	RecordSet rs_checkOn_last(pmodel->MGetDatabase(), definition_tbCheckOnLast);
	rs_checkOn_last.query(sql_get_last_records);

	//����б�
	m_checkOnListCtrl.DeleteAllItems();

	//��ʾ��ѯ����
	for (int i = 0, count = rs_check_on.count(); i < count; i++)
	{
		Record rs = rs_check_on.getRecord(i), rslast = rs_checkOn_last.getRecord(i);
		int nRom = m_checkOnListCtrl.InsertItem(i, _T("test")); 

		m_checkOnListCtrl.SetItemText(nRom, 0, CUtil::DecodeUtf8(rs.getValue(std::string("DeviceID"))->asString()).c_str());
		m_checkOnListCtrl.SetItemText(nRom, 1, CUtil::DecodeUtf8(rs.getValue(std::string("name"))->asString()).c_str());
		m_checkOnListCtrl.SetItemText(nRom, 2, CUtil::DecodeUtf8(rs.getValue(std::string("datetime"))->asString()).c_str());

		//�����ж��Ͼ�ʱ��
		if ((rs.getValue(std::string("DeviceID"))->asInteger() == rslast.getValue(std::string("DeviceID"))->asInteger())
			&&(rs.getValue(std::string("datetime"))->asTime().asInteger() < rslast.getValue(std::string("datetime"))->asTime().asInteger()))
		{
			m_checkOnListCtrl.SetItemText(nRom, 3, CUtil::DecodeUtf8(rslast.getValue(std::string("datetime"))->asString()).c_str());
		}
	}
}

// DirectionMonitorView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IcapLamp.h"
#include "DirectionMonitorFormView.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Util.h"


// CDirectionMonitorFormView

IMPLEMENT_DYNCREATE(CDirectionMonitorFormView, CFormView)

CDirectionMonitorFormView::CDirectionMonitorFormView()
	: CFormView(CDirectionMonitorFormView::IDD)
{

}

CDirectionMonitorFormView::~CDirectionMonitorFormView()
{
}

void CDirectionMonitorFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONITOR_LIST, m_directionMonitorList);
}

BEGIN_MESSAGE_MAP(CDirectionMonitorFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR, &CDirectionMonitorFormView::OnBnClickedButtonMonitor)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDirectionMonitorFormView ���

#ifdef _DEBUG
void CDirectionMonitorFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDirectionMonitorFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDirectionMonitorFormView ��Ϣ�������

//�����ʼ��
void CDirectionMonitorFormView::OnInitialUpdate()
{
	m_directionMonitorList.SubclassDlgItem(IDC_MONITOR_LIST, this);

	DWORD dwStyle = m_directionMonitorList.GetExtendedStyle();
	m_directionMonitorList.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	m_directionMonitorList.InsertColumn(0, _T("�豸ID"));
	m_directionMonitorList.InsertColumn(1, _T("��������"));
	m_directionMonitorList.InsertColumn(2, _T("�����ص�"));
	m_directionMonitorList.InsertColumn(3, _T("����ʱ��"));

	m_directionMonitorList.SetColumnWidth(0, 80);
	m_directionMonitorList.SetColumnWidth(1, 100);
	m_directionMonitorList.SetColumnWidth(2, 160);
	m_directionMonitorList.SetColumnWidth(3, 160);

	//���ö�ʱ����ÿ1����ˢ��һ��
	SetTimer(1, 5000, NULL); 
}

//��ȡ��ǰ����λ��״��
void CDirectionMonitorFormView::GetMonitorRecord()
{
	CTime cur_time = CTime::GetCurrentTime();
	char str_ascii[128] = {0};
	CUtil::GetAnsiString(cur_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_end_time(str_ascii);

	//����ʱ�䷶ΧΪ10����
	cur_time -= 10;
	memset(str_ascii, 0, sizeof(str_ascii));
	CUtil::GetAnsiString(cur_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_begin_time(str_ascii);
	

	std::string sql_monitor = std::string("select A._ID, A.DeviceID, Worker.name, AP.direction, A.datetime from (select * from DirectionMonitor where datetime between \"") + 
		str_begin_time + std::string("\" and \"") + str_end_time +
		std::string("\" group by DeviceID) as A , AP,Worker where A.APID = AP._ID and A.DeviceID = Worker.DeviceID");
		
	CModel *pmodel = CSingleton<CModel>::instance();
	Field definition_tbMonitor[] = 
	{
		Field(FIELD_KEY),
		Field("DeviceID", type_int),
		Field("name", type_text),
		Field("direction", type_text),
		Field("datetime", type_time),
		Field(DEFINITION_END),
	};

	RecordSet rs_monitor(pmodel->MGetDatabase(), definition_tbMonitor);
	rs_monitor.query(sql_monitor);

	//����б�
	m_directionMonitorList.DeleteAllItems();

	//��ʾ��ѯ����
	for (int i = 0, count = rs_monitor.count(); i < count; i++)
	{
		Record rs = rs_monitor.getRecord(i), rslast = rs_monitor.getRecord(i);
		int nRom = m_directionMonitorList.InsertItem(i, _T("test")); 
		m_directionMonitorList.SetItemText(nRom, 0, CUtil::DecodeUtf8(rs.getValue(std::string("DeviceID"))->asString()).c_str());
		m_directionMonitorList.SetItemText(nRom, 1, CUtil::DecodeUtf8(rs.getValue(std::string("name"))->asString()).c_str());
		m_directionMonitorList.SetItemText(nRom, 2, CUtil::DecodeUtf8(rs.getValue(std::string("direction"))->asString()).c_str());
		m_directionMonitorList.SetItemText(nRom, 3, CUtil::DecodeUtf8(rs.getValue(std::string("datetime"))->asString()).c_str());
	}
}

void CDirectionMonitorFormView::OnBnClickedButtonMonitor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetMonitorRecord();
}


void CDirectionMonitorFormView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetMonitorRecord();
	CFormView::OnTimer(nIDEvent);
}

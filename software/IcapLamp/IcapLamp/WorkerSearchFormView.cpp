// WorkerSearchFormView.cpp : 实现文件
//

#include "stdafx.h"
#include "IcapLamp.h"
#include "WorkerSearchFormView.h"
#include "easySQLite/SqlCommon.h"
#include "CustomUtil/Util.h"
#include "CustomUtil/Singleton.h"
#include "CustomUtil/Model.h"
#include "CustomUtil/Util.h"


// CWorkerSearchFormView

IMPLEMENT_DYNCREATE(CWorkerSearchFormView, CFormView)

CWorkerSearchFormView::CWorkerSearchFormView()
	: CFormView(CWorkerSearchFormView::IDD)
{

}

CWorkerSearchFormView::~CWorkerSearchFormView()
{
}

void CWorkerSearchFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_LIST, m_workerSearchListCtrl);
	DDX_Control(pDX, IDC_EDIT_WORKERNAME, m_workerNameEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SEARCH, m_searchBeginTime);
}

BEGIN_MESSAGE_MAP(CWorkerSearchFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_SHOW, &CWorkerSearchFormView::OnBnClickedButtonSearchShow)
END_MESSAGE_MAP()


// CWorkerSearchFormView 诊断

#ifdef _DEBUG
void CWorkerSearchFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorkerSearchFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWorkerSearchFormView 消息处理程序

//窗体初始化
void CWorkerSearchFormView::OnInitialUpdate()
{
	m_workerSearchListCtrl.SubclassDlgItem(IDC_SEARCH_LIST, this);
	m_searchBeginTime.SubclassDlgItem(IDC_DATETIMEPICKER_SEARCH, this);
	m_workerNameEdit.SubclassDlgItem(IDC_EDIT_WORKERNAME, this);

	DWORD dwStyle = m_workerSearchListCtrl.GetExtendedStyle();
	m_workerSearchListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	m_workerSearchListCtrl.InsertColumn(0, _T("设备ID"));
	m_workerSearchListCtrl.InsertColumn(1, _T("工人姓名"));
	m_workerSearchListCtrl.InsertColumn(2, _T("活动地点"));
	m_workerSearchListCtrl.InsertColumn(3, _T("活动时间"));

	m_workerSearchListCtrl.SetColumnWidth(0, 80);
	m_workerSearchListCtrl.SetColumnWidth(1, 100);
	m_workerSearchListCtrl.SetColumnWidth(2, 160);
	m_workerSearchListCtrl.SetColumnWidth(3, 160);
}

void CWorkerSearchFormView::OnBnClickedButtonSearchShow()
{
	// TODO: 在此添加控件通知处理程序代码

	//先检查一下姓名输入
	CString wName;
	m_workerNameEdit.GetWindowText(wName);
	if(wName.GetLength() == 0)
	{
		MessageBox(_T("请输入工人姓名"), _T("提示"), MB_OK);
		return;
	}
	std::string name = CUtil::EncodeUtf8(wName.GetBuffer());

	CTime begin_time;
	m_searchBeginTime.GetTime(begin_time);
	char str_ascii[128] = {0};
	CUtil::GetAnsiString(begin_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_begin_time(str_ascii);

	//设置时间范围为10秒钟
	CTime end_time = begin_time += 60 * 60 * 24 - 1;
	memset(str_ascii, 0, sizeof(str_ascii));
	CUtil::GetAnsiString(end_time.Format("%Y-%m-%d %H:%M:%S"), str_ascii, sizeof(str_ascii));
	std::string str_end_time(str_ascii);

	std::string sql_search = std::string("select A._ID, A.DeviceID, Worker.name, AP.direction, A.datetime from (select * from DirectionMonitor where datetime between \"") + 
		str_begin_time + std::string("\" and \"") + str_end_time +
		std::string("\") as A , AP,Worker where A.APID = AP._ID and A.DeviceID = Worker.DeviceID and Worker.name = \"") + name + "\"";

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
	rs_monitor.query(sql_search);

	//清空列表
	m_workerSearchListCtrl.DeleteAllItems();

	//显示查询数据
	for (int i = 0, count = rs_monitor.count(); i < count; i++)
	{
		Record rs = rs_monitor.getRecord(i), rslast = rs_monitor.getRecord(i);
		int nRom = m_workerSearchListCtrl.InsertItem(i, _T("test")); 
		m_workerSearchListCtrl.SetItemText(nRom, 0, CUtil::DecodeUtf8(rs.getValue(std::string("DeviceID"))->asString()).c_str());
		m_workerSearchListCtrl.SetItemText(nRom, 1, CUtil::DecodeUtf8(rs.getValue(std::string("name"))->asString()).c_str());
		m_workerSearchListCtrl.SetItemText(nRom, 2, CUtil::DecodeUtf8(rs.getValue(std::string("direction"))->asString()).c_str());
		m_workerSearchListCtrl.SetItemText(nRom, 3, CUtil::DecodeUtf8(rs.getValue(std::string("datetime"))->asString()).c_str());
	}
}

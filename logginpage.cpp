// logginpage.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "logginpage.h"
#include "cfgFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CLogginPage 

IMPLEMENT_DYNCREATE(CLogginPage, CPropertyPage)

CLogginPage::CLogginPage() : CPropertyPage(CLogginPage::IDD)
{
	//{{AFX_DATA_INIT(CLogginPage)
	m_strEndSession = _T("");
	m_strLogEntry = _T("");
	m_strStartSession = _T("");
	//}}AFX_DATA_INIT
	m_pToolTip = NULL;

}

CLogginPage::~CLogginPage()
{
	delete m_pToolTip;
}

void CLogginPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogginPage)
	DDX_Control(pDX, IDC_START_SESSION, m_startSession);
	DDX_Control(pDX, IDC_LOG_ENTRY, m_logEntry);
	DDX_Control(pDX, IDC_END_SESSION, m_endSession);
	DDX_Text(pDX, IDC_END_SESSION, m_strEndSession);
	DDX_Text(pDX, IDC_LOG_ENTRY, m_strLogEntry);
	DDX_Text(pDX, IDC_START_SESSION, m_strStartSession);
	DDX_Control(pDX, IDC_REPORT_STYLE, m_oldLogStyle);
	DDX_Control(pDX, IDC_REAL_TIME_LOG, m_newLogStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogginPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLogginPage)
	ON_BN_CLICKED(IDC_REAL_TIME_LOG, OnRealTimeLog)
	ON_BN_CLICKED(IDC_REPORT_STYLE, OnReportStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLogginPage 


void CLogginPage::Init(CString wd)
{

	m_strWd = wd;
}

BOOL CLogginPage::OnInitDialog()
{

	CPropertyPage::OnInitDialog();
	
	cfgFile cfg(m_strWd);
	
	m_strStartSession = cfg.GetStringValue("log_sessionstart");
	m_strEndSession   = cfg.GetStringValue("log_sessionend");
	m_strLogEntry	  = cfg.GetStringValue("log_logentry");

	m_strStartSession.Replace("\\n", "\n");
	m_strEndSession.Replace("\\n", "\n");
	m_strLogEntry.Replace("\\n", "\n");
	m_oldLogStyle.SetCheck(cfg.GetValue("log_useoldstyle"));
	m_newLogStyle.SetCheck(!m_oldLogStyle.GetCheck());
	
	if(m_oldLogStyle.GetCheck()){

		OnReportStyle();
	}
	else{

		OnRealTimeLog();
	}

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	//m_pToolTip->AddTool(this, "Encoder Settings");
	m_pToolTip->AddTool(&m_oldLogStyle, IDS_TOOL_REPORTSTYLE);
	m_pToolTip->AddTool(&m_newLogStyle, IDS_TOOL_NEWLOGSTYLE);
	m_pToolTip->AddTool(&m_startSession, IDS_TOOL_STARTSESSION);
	m_pToolTip->AddTool(&m_logEntry, IDS_TOOL_LOGENTRY);
	m_pToolTip->AddTool(&m_endSession, IDS_TOOL_ENDSESSION);
	

	UpdateData(FALSE);
	return TRUE;
}

void CLogginPage::OnOK() 
{
	
	UpdateData(TRUE);

	cfgFile cfg(m_strWd);
	
	m_strStartSession.Replace("\n", "\\n");
	m_strEndSession.Replace("\n", "\\n");
	m_strLogEntry.Replace("\n", "\\n");

	cfg.SetStringValue("log_sessionstart", m_strStartSession);
	cfg.SetStringValue("log_sessionend", m_strEndSession);
	cfg.SetStringValue("log_logentry", m_strLogEntry);
	cfg.SetValue("log_useoldstyle", m_oldLogStyle.GetCheck());
	CPropertyPage::OnOK();
}

void CLogginPage::OnRealTimeLog() 
{

	m_oldLogStyle.SetCheck(FALSE);
	m_endSession.EnableWindow(FALSE);
	m_startSession.EnableWindow(FALSE);
	m_logEntry.EnableWindow(FALSE);
}

void CLogginPage::OnReportStyle() 
{

	m_newLogStyle.SetCheck(FALSE);
	m_endSession.EnableWindow(TRUE);
	m_startSession.EnableWindow(TRUE);
	m_logEntry.EnableWindow(TRUE);
}

BOOL CLogginPage::PreTranslateMessage(MSG* pMsg) 
{

	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

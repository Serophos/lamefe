// SettingsDecoder.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lamefe.h"
#include "SettingsDecoder.h"
#include "PathDialog.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsDecoder 


CSettingsDecoder::CSettingsDecoder(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsDecoder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDecoder)
	m_strPath = _T("");
	//}}AFX_DATA_INIT
}


void CSettingsDecoder::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDecoder)
	DDX_Control(pDX, IDC_DEFAULT_FILE, m_cFile);
	DDX_Control(pDX, IDC_DEFAULT_CD, m_cCD);
	DDX_Control(pDX, IDC_REMEMBER_LAST, m_cLastDec);
	DDX_Text(pDX, IDC_PATH_WINAMP, m_strPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDecoder, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsDecoder)
	ON_BN_CLICKED(IDC_DEFAULT_CD, OnCD)
	ON_BN_CLICKED(IDC_DEFAULT_FILE, OnFile)
	ON_BN_CLICKED(IDC_WINAMP_PATH, OnWinampPath)
	ON_BN_CLICKED(IDC_REMEMBER_LAST, OnRememberLast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsDecoder 


void CSettingsDecoder::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&m_cCD, IDS_TOOL_DEFAULTCD);
		m_pToolTip->AddTool(&m_cFile, IDS_TOOL_DEFAULTFILE);
		m_pToolTip->AddTool(&m_cLastDec, IDS_TOOL_LASTDECODER);
		m_pToolTip->Activate(TRUE);
	}
	
	m_cCD.SetCheck(g_sSettings.GetDefaultFromCD());
	m_cFile.SetCheck(!g_sSettings.GetDefaultFromCD() && !g_sSettings.GetRememberLastDecoder());
	m_cLastDec.SetCheck(g_sSettings.GetRememberLastDecoder());
	m_strPath = g_sSettings.GetWinampPluginPath();

	UpdateData(FALSE);
}

BOOL CSettingsDecoder::Save()
{

	UpdateData(TRUE);
	g_sSettings.SetDefaultFromCD(m_cCD.GetCheck());
	g_sSettings.SetRememberLastDecoder(m_cLastDec.GetCheck());
	g_sSettings.SetWinampPluginPath(m_strPath);
	g_sSettings.Save();
	return TRUE;
}

void CSettingsDecoder::OnFile()
{

	m_cCD.SetCheck(!m_cFile.GetCheck());
	m_cLastDec.SetCheck(FALSE);
}

void CSettingsDecoder::OnCD()
{

	m_cFile.SetCheck(!m_cCD.GetCheck());
	m_cLastDec.SetCheck(FALSE);
}

void CSettingsDecoder::OnRememberLast() 
{

	m_cFile.SetCheck(FALSE);
	m_cCD.SetCheck(FALSE);
}

void CSettingsDecoder::OnWinampPath() 
{

	UpdateData(TRUE);
	
	CPathDialog dlg("Select Winamp plugin folder", "Select directory where the Winamp plugins are located.", m_strPath);
	int nResult = dlg.DoModal();
	if(nResult == IDOK){
		
		if(m_strPath != dlg.GetPathName()){

			AfxMessageBox(IDS_RESTARTLAME, MB_OK+MB_ICONINFORMATION);
			m_strPath = dlg.GetPathName();
		}

		UpdateData(FALSE);
	}
	
}


// SettingsDecoder.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lamefe.h"
#include "SettingsDecoder.h"
#include "PathDialog.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString		g_strIniFile;

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
	DDX_Text(pDX, IDC_PATH_WINAMP, m_strPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDecoder, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsDecoder)
	ON_BN_CLICKED(IDC_DEFAULT_CD, OnCD)
	ON_BN_CLICKED(IDC_DEFAULT_FILE, OnFile)
	ON_BN_CLICKED(IDC_WINAMP_PATH, OnWinampPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsDecoder 


void CSettingsDecoder::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);
	m_cCD.SetCheck(cfg.GetValue("LameFE", "DefaultFromCD", TRUE));
	m_cFile.SetCheck(!cfg.GetValue("LameFE", "DefaultFromCD", TRUE));
	m_strPath = cfg.GetValue("LameFE", "WinampPluginPath", m_strWd + "\\Plugins");
	UpdateData(FALSE);
}

BOOL CSettingsDecoder::Save()
{

	UpdateData(TRUE);
	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);
	cfg.SetValue("LameFE", "DefaultFromCD", m_cCD.GetCheck());
	cfg.SetValue("LameFE", "WinampPluginPath", m_strPath);
	return TRUE;
}

void CSettingsDecoder::OnFile()
{

	m_cCD.SetCheck(!m_cFile.GetCheck());
}

void CSettingsDecoder::OnCD()
{

	m_cFile.SetCheck(!m_cCD.GetCheck());
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

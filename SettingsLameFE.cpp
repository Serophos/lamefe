// SettingsLameFE.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lamefe.h"
#include "SettingsLameFE.h"
#include "PathDialog.h"
#include "Ini.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsLameFE 


CSettingsLameFE::CSettingsLameFE(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsLameFE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsLameFE)
	m_playerPath = _T("[System default]");
	m_presetPath = _T("");
	//}}AFX_DATA_INIT
}


void CSettingsLameFE::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsLameFE)
	DDX_Control(pDX, IDC_ENQUEUE_FILES, c_enqueueFiles);
	DDX_Control(pDX, IDC_PLAYER_PATH, c_playerPath);
	DDX_Control(pDX, IDC_SHUTDOWN, c_shutdownOnFinished);
	DDX_Control(pDX, IDC_PLAY_FILES, c_playFiles);
	DDX_Control(pDX, IDC_M3U, c_m3u);
	DDX_Control(pDX, IDC_FINISH_DLG, c_finishedDialog);
	DDX_Control(pDX, IDC_EXIT_FINISHED, c_quitOnFinished);
	DDX_Control(pDX, IDC_BEEP, c_beep);
	DDX_Control(pDX, IDC_FOCUS, c_getfocus);
	DDX_Control(pDX, IDC_PLAY_SOUND, c_playSound);
	DDX_Text(pDX, IDC_PLAYER_PATH, m_playerPath);
	DDX_Text(pDX, IDC_PRESET_PATH_DISPLAY, m_presetPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsLameFE, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsLameFE)
	ON_BN_CLICKED(IDC_PLAYER, OnPlayer)
	ON_BN_CLICKED(IDC_PLAY_FILES, OnPlayFiles)
	ON_BN_CLICKED(IDC_PRESETPATH, OnPresetpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsLameFE 

void CSettingsLameFE::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	c_beep.SetCheck(cfg.GetValue("LameFE", "Beep", TRUE));
	c_finishedDialog.SetCheck(cfg.GetValue("LameFE", "Dialog", FALSE));
	c_m3u.SetCheck(cfg.GetValue("LameFE", "M3U", FALSE));
	c_playFiles.SetCheck(cfg.GetValue("LameFE", "PlayBack", FALSE));
	c_enqueueFiles.SetCheck(cfg.GetValue("LameFE", "Enqueue", FALSE));
	c_quitOnFinished.SetCheck(cfg.GetValue("LameFE", "ExitLameFE", FALSE));
	c_shutdownOnFinished.SetCheck(cfg.GetValue("LameFE", "Shutdown", FALSE));
	m_playerPath = cfg.GetValue("LameFE", "ExternalPlayer", "[System Default]");
	c_getfocus.SetCheck(cfg.GetValue("LameFE", "GetFocus",TRUE));
	c_playSound.SetCheck(cfg.GetValue("LameFE", "PlaySound", FALSE));
	m_presetPath = cfg.GetValue("LameFE", "PresetPath", m_strWd);

	UpdateData(FALSE);
	
	c_enqueueFiles.EnableWindow(c_playFiles.GetCheck());

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_beep, IDS_TOOL_BEEP);
		m_pToolTip->AddTool(&c_finishedDialog, IDS_TOOL_NOTIFICATION);
		m_pToolTip->AddTool(&c_m3u, IDS_TOOL_M3U);
		m_pToolTip->AddTool(&c_playFiles, IDS_TOOL_PLAYFILES);
		m_pToolTip->AddTool(&c_quitOnFinished, IDS_TOOL_QUIT);
		m_pToolTip->AddTool(&c_shutdownOnFinished, IDS_TOOL_SHUTDOWN);
		m_pToolTip->AddTool(&c_playerPath, IDS_TOOL_EXTPLAYER);
		m_pToolTip->AddTool(&c_getfocus, IDS_TOOL_GETFOCUS);
		m_pToolTip->AddTool(&c_enqueueFiles, IDS_TOOL_ENQUEUE);
		m_pToolTip->Activate(TRUE);
	}

}

BOOL CSettingsLameFE::Save()
{

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");

	cfg.SetValue("LameFE", "Beep", c_beep.GetCheck());
	cfg.SetValue("LameFE", "Dialog", c_finishedDialog.GetCheck());
	cfg.SetValue("LameFE", "M3U", c_m3u.GetCheck());
	cfg.SetValue("LameFE", "PlayBack", c_playFiles.GetCheck());
	cfg.SetValue("LameFE", "ExitLameFE", c_quitOnFinished.GetCheck());
	cfg.SetValue("LameFE", "Shutdown", c_shutdownOnFinished.GetCheck());
	cfg.SetValue("LameFE", "ExternalPlayer", m_playerPath);
	cfg.SetValue("LameFE", "Enqueue", c_enqueueFiles.GetCheck());
	cfg.SetValue("LameFE", "GetFocus", c_getfocus.GetCheck());
	cfg.SetValue("LameFE", "PlaySound", c_playSound.GetCheck());
	cfg.SetValue("LameFE", "PresetPath", m_presetPath);

	return TRUE;
}

void CSettingsLameFE::OnPlayer() 
{
	
	
	static char BASED_CODE szFilter[] = "Executable (*.exe)|*.exe|All files (*.*)|*.*|";
	
	CFileDialog FileOpen(TRUE, "*.exe", 0, OFN_FILEMUSTEXIST , szFilter, 0);
	FileOpen.m_ofn.lpstrInitialDir = "%PROGDIR%";
	
	if(FileOpen.DoModal() == IDOK){

		m_playerPath = FileOpen.GetPathName();
		UpdateData(FALSE);
	}
	
}

void CSettingsLameFE::OnPlayFiles() 
{

	c_enqueueFiles.EnableWindow(c_playFiles.GetCheck());
}


void CSettingsLameFE::OnPresetpath() 
{

	UpdateData(TRUE);

	CPathDialog dlg("Select preset folder", "Select directory where LameFE stores the presets.", m_presetPath);
	int nResult = dlg.DoModal();
	if(nResult == IDOK){

		if(m_presetPath != dlg.GetPathName()){

			m_presetPath = dlg.GetPathName();
			AfxMessageBox(IDS_RESTARTLAME, MB_OK+MB_ICONINFORMATION);
		}
		UpdateData(FALSE);
	}
}

/*
** Copyright (C) 2002 Thees Winkler
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "stdafx.h"
#include "lameFE.h"
#include "MainSettingsPage.h"
#include "cfgFile.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CMainSettingsPage 

IMPLEMENT_DYNCREATE(CMainSettingsPage, CPropertyPage)

CMainSettingsPage::CMainSettingsPage() : CPropertyPage(CMainSettingsPage::IDD)
{
	//{{AFX_DATA_INIT(CMainSettingsPage)
	m_outputPath = _T("");
	m_playerPath = _T("[System default]");
	//}}AFX_DATA_INIT
	m_pToolTip = NULL;

}

CMainSettingsPage::~CMainSettingsPage()
{

	delete m_pToolTip;
}

void CMainSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainSettingsPage)
	DDX_Control(pDX, IDC_HIDEMWND_ENC, c_hideMainWnd);
	DDX_Control(pDX, IDC_ENQUEUE_FILES, c_enqueueFiles);
	DDX_Control(pDX, IDC_CDTEXT_ALWAYSREAD, c_cdTextRead);
	DDX_Control(pDX, IDC_CPLAYERINI_ALWAYS_WRITE, c_writeCDPlayerIni);
	DDX_Control(pDX, IDC_CPLAYERINI_ALWAYS_READ, c_readCDPlayerIni);
	DDX_Control(pDX, IDC_PLAYER_PATH, c_playerPath);
	DDX_Control(pDX, IDC_SILENT, c_silent);
	DDX_Control(pDX, IDC_SHUTDOWN, c_shutdownOnFinished);
	DDX_Control(pDX, IDC_PLAY_FILES, c_playFiles);
	DDX_Control(pDX, IDC_M3U, c_m3u);
	DDX_Control(pDX, IDC_FINISH_DLG, c_finishedDialog);
	DDX_Control(pDX, IDC_EXIT_FINISHED, c_quitOnFinished);
	DDX_Control(pDX, IDC_BEEP, c_beep);
	DDX_Control(pDX, IDC_FOCUS, c_getfocus);
	DDX_Control(pDX, IDC_PLAY_SOUND, c_playSound);
	DDX_Control(pDX, IDC_SAVE_WINDOW_STATE, c_saveWinPos);
	DDX_Control(pDX, IDC_USE_HIGH_COLOR_ICONS, c_highColBar);
	DDX_Text(pDX, IDC_PLAYER_PATH, m_playerPath);
	DDX_Control(pDX, IDC_SHOW_SPLASH, c_showSplash);
	DDX_Control(pDX, IDC_ALBUMEDIT, c_showAlbumEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMainSettingsPage)
	//ON_BN_CLICKED(IDC_OUTPUT, OnOutput)
	ON_BN_CLICKED(IDC_PLAYER, OnPlayer)
	ON_BN_CLICKED(IDC_PLAY_FILES, OnPlayFiles)
	ON_BN_CLICKED(IDC_USE_HIGH_COLOR_ICONS, OnHighColorIcons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMainSettingsPage 

void CMainSettingsPage::init(CString wdir)
{
	wd = wdir;
}

BOOL CMainSettingsPage::OnInitDialog()
{

	CPropertyPage::OnInitDialog();

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	m_pToolTip->AddTool(&c_beep, IDS_TOOL_BEEP);
	m_pToolTip->AddTool(&c_finishedDialog, IDS_TOOL_NOTIFICATION);
	m_pToolTip->AddTool(&c_m3u, IDS_TOOL_M3U);
	m_pToolTip->AddTool(&c_playFiles, IDS_TOOL_PLAYFILES);
	m_pToolTip->AddTool(&c_quitOnFinished, IDS_TOOL_QUIT);
	m_pToolTip->AddTool(&c_shutdownOnFinished, IDS_TOOL_SHUTDOWN);
	m_pToolTip->AddTool(&c_silent, IDS_TOOL_SILENT);
	m_pToolTip->AddTool(&c_playerPath, IDS_TOOL_EXTPLAYER);
	m_pToolTip->AddTool(&c_readCDPlayerIni, IDS_TOOL_READCDPINI);
	m_pToolTip->AddTool(&c_writeCDPlayerIni, IDS_TOOL_WRITECDPINI);
	m_pToolTip->AddTool(&c_cdTextRead, IDS_TOOL_CDTEXTREAD);
	m_pToolTip->AddTool(&c_playSound, IDS_TOOL_PLAYSOUND);
	m_pToolTip->AddTool(&c_saveWinPos, IDS_TOOL_SAVEWINPOS);
	m_pToolTip->AddTool(&c_highColBar, IDS_TOOL_HIGHCOLOR);
	m_pToolTip->AddTool(&c_getfocus, IDS_TOOL_GETFOCUS);
	m_pToolTip->AddTool(&c_showSplash, IDS_TOOL_SHOWSPLASH);
	m_pToolTip->AddTool(&c_showAlbumEditor, IDS_TOOL_ALWAYSTAGEDIT);
	m_pToolTip->AddTool(&c_hideMainWnd, IDS_TOOL_HIDEMAINWND);
	m_pToolTip->AddTool(&c_enqueueFiles, IDS_TOOL_ENQUEUE);
	m_pToolTip->Activate(TRUE);

	
	cfgFile cfg(wd);

	c_beep.SetCheck(cfg.GetValue("beep"));
	c_finishedDialog.SetCheck(cfg.GetValue("dialog"));
	c_m3u.SetCheck(cfg.GetValue("m3u"));
	c_playFiles.SetCheck(cfg.GetValue("play"));
	c_enqueueFiles.SetCheck(cfg.GetValue("enqueue"));
	c_quitOnFinished.SetCheck(cfg.GetValue("quit"));
	c_shutdownOnFinished.SetCheck(cfg.GetValue("shutdown"));
	m_playerPath = cfg.GetStringValue("extplayer");
	c_silent.SetCheck(cfg.GetValue("silent"));
	c_readCDPlayerIni.SetCheck(cfg.GetValue("readcdplayerini"));
	c_writeCDPlayerIni.SetCheck(cfg.GetValue("savecdplayerini"));
	c_cdTextRead.SetCheck(cfg.GetValue("readcdtext"));
	c_getfocus.SetCheck(cfg.GetValue("getfocus"));
	c_playSound.SetCheck(cfg.GetValue("playsound"));
	c_saveWinPos.SetCheck(cfg.GetValue("savewinpos"));
	c_highColBar.SetCheck(cfg.GetValue("usehighcoloricons"));
	c_showSplash.SetCheck(cfg.GetValue("showsplashscr"));
	c_showAlbumEditor.SetCheck(cfg.GetValue("showtagedit"));
	c_hideMainWnd.SetCheck(cfg.GetValue("hideduringenc"));
	UpdateData(FALSE);
	
	c_enqueueFiles.EnableWindow(c_playFiles.GetCheck());
	
	return TRUE;
}

void CMainSettingsPage::OnOK()
{

	cfgFile cfg(wd);

	cfg.SetValue("beep", c_beep.GetCheck());
	cfg.SetValue("dialog", c_finishedDialog.GetCheck());
	cfg.SetValue("m3u", c_m3u.GetCheck());
	cfg.SetValue("play", c_playFiles.GetCheck());
	cfg.SetValue("quit", c_quitOnFinished.GetCheck());
	cfg.SetValue("shutdown", c_shutdownOnFinished.GetCheck());
	cfg.SetStringValue("extplayer", m_playerPath);
	cfg.SetValue("silent", c_silent.GetCheck());
	cfg.SetValue("readcdplayerini", c_readCDPlayerIni.GetCheck());
	cfg.SetValue("savecdplayerini", c_writeCDPlayerIni.GetCheck());
	cfg.SetValue("readcdtext", c_cdTextRead.GetCheck());
	cfg.SetValue("enqueue", c_enqueueFiles.GetCheck());
	cfg.SetValue("getfocus", c_getfocus.GetCheck());
	cfg.SetValue("playsound", c_playSound.GetCheck());
	cfg.SetValue("savewinpos", c_saveWinPos.GetCheck());
	cfg.SetValue("usehighcoloricons", c_highColBar.GetCheck());
	cfg.SetValue("showsplashscr", c_showSplash.GetCheck());
	cfg.SetValue("showtagedit", c_showAlbumEditor.GetCheck());
	cfg.SetValue("hideduringenc", c_hideMainWnd.GetCheck());
}


void CMainSettingsPage::OnPlayer() 
{
	
	
	static char BASED_CODE szFilter[] = "Executable (*.exe)|*.exe|All files (*.*)|*.*|";
	
	CFileDialog FileOpen(TRUE, "*.exe", 0, OFN_FILEMUSTEXIST , szFilter, 0);
	FileOpen.m_ofn.lpstrInitialDir = "%PROGDIR%";
	
	if(FileOpen.DoModal() == IDOK){

		m_playerPath = FileOpen.GetPathName();
		UpdateData(FALSE);
	}
	
}


BOOL CMainSettingsPage::PreTranslateMessage(MSG* pMsg) 
{
	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CMainSettingsPage::OnPlayFiles() 
{

	c_enqueueFiles.EnableWindow(c_playFiles.GetCheck());
}

void CMainSettingsPage::OnHighColorIcons()
{

	if(!Utils::CheckCOMTL32Dll()){

		AfxMessageBox(IDS_OLDCOMTL32DLL, MB_OK+MB_ICONEXCLAMATION);
		c_highColBar.SetCheck(FALSE);
		return;
	}

	AfxMessageBox(IDS_RESTARTLAME, MB_OK+MB_ICONINFORMATION);
}

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
#include "FolderDialog.h"

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
	m_playerPath = _T("");
	//}}AFX_DATA_INIT
}

CMainSettingsPage::~CMainSettingsPage()
{
}

void CMainSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainSettingsPage)
	DDX_Control(pDX, IDC_SILENT, c_silent);
	DDX_Control(pDX, IDC_SHUTDOWN, c_shutdownOnFinished);
	DDX_Control(pDX, IDC_PLAY_FILES, c_playFiles);
	DDX_Control(pDX, IDC_M3U, c_m3u);
	DDX_Control(pDX, IDC_FINISH_DLG, c_finishedDialog);
	DDX_Control(pDX, IDC_EXIT_FINISHED, c_quitOnFinished);
	DDX_Control(pDX, IDC_BEEP, c_beep);
	DDX_Text(pDX, IDC_OUTPUT_PATH, m_outputPath);
	DDX_Text(pDX, IDC_PLAYER_PATH, m_playerPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMainSettingsPage)
	ON_BN_CLICKED(IDC_OUTPUT, OnOutput)
	ON_BN_CLICKED(IDC_PLAYER, OnPlayer)
	ON_BN_CLICKED(IDC_PLUGINS, OnPlugins)
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

	cfgFile cfg(wd);

	c_beep.SetCheck(cfg.GetValue(BEEP));
	c_finishedDialog.SetCheck(cfg.GetValue(DIALOG));
	c_m3u.SetCheck(cfg.GetValue(M3U));
	c_playFiles.SetCheck(cfg.GetValue(PLAY));
	c_quitOnFinished.SetCheck(cfg.GetValue(QUIT));
	c_shutdownOnFinished.SetCheck(cfg.GetValue(SHUTDOWN));
	m_outputPath = cfg.GetStringValue(OUTPUT);
	m_playerPath = cfg.GetStringValue(PLAYER);
	c_silent.SetCheck(cfg.GetValue(SILENT));
	UpdateData(FALSE);

	return true;
}

void CMainSettingsPage::OnOK()
{

	cfgFile cfg(wd);

	cfg.SetValue(BEEP, (LPVOID)c_beep.GetCheck());
	cfg.SetValue(DIALOG, (LPVOID)c_finishedDialog.GetCheck());
	cfg.SetValue(M3U, (LPVOID)c_m3u.GetCheck());
	cfg.SetValue(PLAY, (LPVOID)c_playFiles.GetCheck());
	cfg.SetValue(QUIT, (LPVOID)c_quitOnFinished.GetCheck());
	cfg.SetValue(SHUTDOWN, (LPVOID)c_shutdownOnFinished.GetCheck());
	cfg.SetValue(OUTPUT, (LPVOID)m_outputPath.GetBuffer(10));
	m_outputPath.ReleaseBuffer();
	cfg.SetValue(PLAYER, (LPVOID)m_playerPath.GetBuffer(10));
	m_playerPath.ReleaseBuffer();
	cfg.SetValue(SILENT, (LPVOID)c_silent.GetCheck());
}

void CMainSettingsPage::OnOutput() 
{

	UpdateData(TRUE);

	CFolderDialog dlg(m_outputPath);
	int nResult = dlg.DoModal();
	if(nResult == IDOK){

		m_outputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}

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

void CMainSettingsPage::OnPlugins() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	
}

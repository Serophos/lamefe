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
#include "lamefe.h"
#include "SettingsLookNFeel.h"
#include "Ini.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsLookNFeel 


CSettingsLookNFeel::CSettingsLookNFeel(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsLookNFeel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsLookNFeel)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CSettingsLookNFeel::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsLookNFeel)
	DDX_Control(pDX, IDC_SHOW_TAGEDITOR_ONFILE, c_teditOnFile);
	DDX_Control(pDX, IDC_HIDEMWND_ENC, c_hideMainWnd);
	DDX_Control(pDX, IDC_CDTEXT_ALWAYSREAD, c_cdTextRead);
	DDX_Control(pDX, IDC_CPLAYERINI_ALWAYS_WRITE, c_writeCDPlayerIni);
	DDX_Control(pDX, IDC_CPLAYERINI_ALWAYS_READ, c_readCDPlayerIni);
	DDX_Control(pDX, IDC_SILENT, c_silent);
	DDX_Control(pDX, IDC_SAVE_WINDOW_STATE, c_saveWinPos);
	DDX_Control(pDX, IDC_USE_HIGH_COLOR_ICONS, c_highColBar);
	DDX_Control(pDX, IDC_SHOW_SPLASH, c_showSplash);
	DDX_Control(pDX, IDC_ALBUMEDIT, c_showAlbumEditor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsLookNFeel, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsLookNFeel)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	ON_BN_CLICKED(IDC_USE_HIGH_COLOR_ICONS, OnHighColorIcons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsLookNFeel 

void CSettingsLookNFeel::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	c_silent.SetCheck(cfg.GetValue("LameFE", "SilentMode", FALSE));
	c_readCDPlayerIni.SetCheck(cfg.GetValue("LameFE", "ReadCDPlayer.ini", TRUE));
	c_writeCDPlayerIni.SetCheck(cfg.GetValue("LameFE", "WriteCDPlayer.ini", TRUE));
	c_cdTextRead.SetCheck(cfg.GetValue("LameFE", "ReadCDText",FALSE));
	c_saveWinPos.SetCheck(cfg.GetValue("LameFE", "SaveWinPos", TRUE));
	c_highColBar.SetCheck(cfg.GetValue("LameFE", "UseHighColBar", TRUE));
	c_showSplash.SetCheck(cfg.GetValue("LameFE", "ShowSplash", TRUE));
	c_showAlbumEditor.SetCheck(cfg.GetValue("LameFE", "ShowAlbumEditor", FALSE));
	c_teditOnFile.SetCheck(cfg.GetValue("LameFE", "ShowAlbumEditoOnFile", FALSE));
	c_hideMainWnd.SetCheck(cfg.GetValue("LameFE", "HideMainWnd", TRUE));

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_readCDPlayerIni, IDS_TOOL_READCDPINI);
		m_pToolTip->AddTool(&c_writeCDPlayerIni, IDS_TOOL_WRITECDPINI);
		m_pToolTip->AddTool(&c_cdTextRead, IDS_TOOL_CDTEXTREAD);
		m_pToolTip->AddTool(&c_saveWinPos, IDS_TOOL_SAVEWINPOS);
		m_pToolTip->AddTool(&c_highColBar, IDS_TOOL_HIGHCOLOR);
		m_pToolTip->AddTool(&c_showSplash, IDS_TOOL_SHOWSPLASH);
		m_pToolTip->AddTool(&c_showAlbumEditor, IDS_TOOL_ALWAYSTAGEDIT);
		m_pToolTip->AddTool(&c_hideMainWnd, IDS_TOOL_HIDEMAINWND);
		m_pToolTip->Activate(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CSettingsLookNFeel::Save()
{

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	cfg.SetValue("LameFE", "SilentMode", c_silent.GetCheck());
	cfg.SetValue("LameFE", "ReadCDPlayer.ini", c_readCDPlayerIni.GetCheck());
	cfg.SetValue("LameFE", "WriteCDPlayer.ini", c_writeCDPlayerIni.GetCheck());
	cfg.SetValue("LameFE", "ReadCDText", c_cdTextRead.GetCheck());
	cfg.SetValue("LameFE", "SaveWinPos", c_saveWinPos.GetCheck());
	cfg.SetValue("LameFE", "UseHighColBar", c_highColBar.GetCheck());
	cfg.SetValue("LameFE", "ShowSplash", c_showSplash.GetCheck());
	cfg.SetValue("LameFE", "ShowAlbumEditor", c_showAlbumEditor.GetCheck());
	cfg.SetValue("LameFE", "ShowAlbumEditoOnFile", c_teditOnFile.GetCheck());
	cfg.SetValue("LameFE", "HideMainWnd", c_hideMainWnd.GetCheck());

	return TRUE;
}

void CSettingsLookNFeel::OnHighColorIcons()
{

	if(!Utils::CheckCOMTL32Dll()){

		AfxMessageBox(IDS_OLDCOMTL32DLL, MB_OK+MB_ICONEXCLAMATION);
		c_highColBar.SetCheck(FALSE);
		return;
	}

	AfxMessageBox(IDS_RESTARTLAME, MB_OK+MB_ICONINFORMATION);
}

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
#include "Settings.h"
#include "Utils.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CSettings g_sSettings;
extern CI18n	 g_iLang;

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
	DDX_Control(pDX, IDC_REMEMBERLASTENCODER, c_rememberEncoder);
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

	g_iLang.TranslateDialog(this, IDD_SETTINGS_LOOKNFEEL);

	c_silent.SetCheck(g_sSettings.GetSilentMode());
	c_readCDPlayerIni.SetCheck(g_sSettings.GetReadCDPlayerIni());
	c_writeCDPlayerIni.SetCheck(g_sSettings.GetWriteCDPlayerIni());
	c_cdTextRead.SetCheck(g_sSettings.GetReadCDTex());
	c_saveWinPos.SetCheck(g_sSettings.GetSaveWinPos());
	c_highColBar.SetCheck(g_sSettings.GetUseHighColBar());
	c_showSplash.SetCheck(g_sSettings.GetShowSplash());
	c_showAlbumEditor.SetCheck(g_sSettings.GetShowAlbumEditor());
	c_teditOnFile.SetCheck(g_sSettings.GetShowAlbumEditorOnFile());
	c_hideMainWnd.SetCheck(g_sSettings.GetHideMainWnd());
	c_rememberEncoder.SetCheck(g_sSettings.GetRememberEncoder());
	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_readCDPlayerIni, g_iLang.GetString(IDS_TOOL_READCDPINI));
		m_pToolTip->AddTool(&c_writeCDPlayerIni, g_iLang.GetString(IDS_TOOL_WRITECDPINI));
		m_pToolTip->AddTool(&c_silent, g_iLang.GetString(IDS_TOOL_SILENT));
		m_pToolTip->AddTool(&c_cdTextRead, g_iLang.GetString(IDS_TOOL_CDTEXTREAD));
		m_pToolTip->AddTool(&c_saveWinPos, g_iLang.GetString(IDS_TOOL_SAVEWINPOS));
		m_pToolTip->AddTool(&c_highColBar, g_iLang.GetString(IDS_TOOL_HIGHCOLOR));
		m_pToolTip->AddTool(&c_showSplash, g_iLang.GetString(IDS_TOOL_SHOWSPLASH));
		m_pToolTip->AddTool(&c_showAlbumEditor, g_iLang.GetString(IDS_TOOL_ALWAYSTAGEDIT));
		m_pToolTip->AddTool(&c_hideMainWnd, g_iLang.GetString(IDS_TOOL_HIDEMAINWND));
		m_pToolTip->AddTool(&c_rememberEncoder, g_iLang.GetString(IDS_TOOL_LASTENCODER));
		m_pToolTip->AddTool(&c_teditOnFile, g_iLang.GetString(IDS_TOOL_SHOWID3FILE));
		m_pToolTip->Activate(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CSettingsLookNFeel::Save()
{

	g_sSettings.SetSilentMode(c_silent.GetCheck());
	g_sSettings.SetReadCDPlayerIni(c_readCDPlayerIni.GetCheck());
	g_sSettings.SetWriteCDPlayerIni(c_writeCDPlayerIni.GetCheck());
	g_sSettings.SetReadCDTex(c_cdTextRead.GetCheck());
	g_sSettings.SetSaveWinPos(c_saveWinPos.GetCheck());
	g_sSettings.SetUseHighColBar(c_highColBar.GetCheck());
	g_sSettings.SetShowSplash(c_showSplash.GetCheck());
	g_sSettings.SetShowAlbumEditor(c_showAlbumEditor.GetCheck());
	g_sSettings.SetShowAlbumEditorOnFile( c_teditOnFile.GetCheck());
	g_sSettings.SetHideMainWnd(c_hideMainWnd.GetCheck());
	g_sSettings.SetRememberEncoder(c_rememberEncoder.GetCheck());
	g_sSettings.Save();

	return TRUE;
}

void CSettingsLookNFeel::OnHighColorIcons()
{

	if(!Utils::CheckCOMTL32Dll()){

		AfxMessageBox(g_iLang.GetString(IDS_OLDCOMTL32DLL), MB_OK+MB_ICONEXCLAMATION);
		c_highColBar.SetCheck(FALSE);
		return;
	}

	AfxMessageBox(g_iLang.GetString(IDS_RESTARTLAME), MB_OK+MB_ICONINFORMATION);
}

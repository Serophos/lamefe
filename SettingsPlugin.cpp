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
#include "SettingsPlugin.h"
#include "direct.h"
#include "LameFEPlugin.h"
#include "WinampPlugin.h"
#include "Settings.h"
#include "OutPlugin.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
extern CI18n	 g_iLang;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsPlugin 


CSettingsPlugin::CSettingsPlugin(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsPlugin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsPlugin)
	m_strAbout = _T("");
	m_strExt = _T("");
	m_strPluginname = _T("");
	//}}AFX_DATA_INIT
}


void CSettingsPlugin::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsPlugin)
	DDX_Text(pDX, IDC_ABOUTSTRING, m_strAbout);
	DDX_Text(pDX, IDC_EXTENSIONS, m_strExt);
	DDX_Text(pDX, IDC_PLUGINNAME, m_strPluginname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsPlugin, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsPlugin)
	ON_BN_CLICKED(IDC_CONFIGURATION, OnConfiguration)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsPlugin 

void CSettingsPlugin::OnConfiguration() 
{

	if(m_isInput){

		if(m_strPlugin.Right(7) == "_in.dll"){

			CLameFEPlugin plInput(m_strWd + "\\Plugins\\" + m_strPlugin);
			if(plInput.Load()){

				plInput.Configure();
			}
			plInput.Unload();
		}
		else{

			CWinampPlugin plInput(m_strPluginDir + "\\" +  m_strPlugin);
			if(plInput.Load(NULL)){
				
				plInput.Configure(0);
			}
			plInput.Unload();
		}
	}
	else{

		COutPlugin plOutput(m_strWd + "\\Plugins\\" + m_strPlugin);
		plOutput.Load();
		plOutput.Configure(NULL);
		plOutput.Unload();
	}
}

void CSettingsPlugin::OnAbout() 
{

	if(m_isInput){

		if(m_strPlugin.Right(7) == "_in.dll"){

			CLameFEPlugin plInput(m_strWd + "\\Plugins\\" + m_strPlugin);
			if(plInput.Load()){
				
				plInput.About();
			}
			plInput.Unload();
		}
		else{

			CWinampPlugin plInput(m_strPluginDir + "\\" +  m_strPlugin);
			if(plInput.Load(NULL)){

				plInput.About(0);
			}
			plInput.Unload();
		}
	}
	else{

		COutPlugin plOutput(m_strWd + "\\Plugins\\" + m_strPlugin);
		plOutput.Load();
		plOutput.About();
		plOutput.Unload();
	}
}

void CSettingsPlugin::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);
	
	g_iLang.TranslateDialog(this, IDD_SETTINGS_PLUGINCODEC);

	m_strPluginDir = g_sSettings.GetWinampPluginPath();
}

BOOL CSettingsPlugin::Save()
{

	return TRUE;
}

void CSettingsPlugin::LoadPlugin(CString strPlugin, BOOL isInput)
{

	m_isInput	= isInput;
	m_strPlugin = strPlugin;
	m_strPluginname.Format("Plugin :: %s", strPlugin);
	if(m_isInput){

		if(m_strPlugin.Right(7) == "_in.dll"){

			CLameFEPlugin plInput(m_strWd + "\\Plugins\\" + m_strPlugin);
			if(plInput.Load()){

				m_strAbout	= plInput.GetInfoString();
				m_strExt	= plInput.GetFileExtensions();
			}
			plInput.Unload();
		}
		else{

			CWinampPlugin plInput(m_strPluginDir + "\\" +  m_strPlugin);
			if(plInput.Load(NULL)){

				m_strAbout   = plInput.GetInfoString();
				m_strExt	 = plInput.GetExtension();
			}
			plInput.Unload();
		}
	}
	else{

		COutPlugin plOutput(m_strWd + "\\Plugins\\" + m_strPlugin);
		plOutput.Load();
		m_strAbout	= plOutput.GetInfoString();
		m_strExt	= plOutput.GetExtension();
		plOutput.Unload();
	}
	UpdateData(FALSE);
}

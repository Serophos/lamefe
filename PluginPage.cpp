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
#include "PluginPage.h"
#include "direct.h"
#include "LameFEPlugin.h"
#include "OutPlugin.h"
#include "cfgFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPluginPage 

IMPLEMENT_DYNCREATE(CPluginPage, CPropertyPage)

CPluginPage::CPluginPage()  : CPropertyPage(CPluginPage::IDD)
{
	//{{AFX_DATA_INIT(CPluginPage)
	m_pluginDet = _T("");
	m_pluginName = _T("");
	m_outName = _T("");
	//}}AFX_DATA_INIT

	hDLL = NULL;
}


void CPluginPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginPage)
	DDX_Control(pDX, IDC_INSTALLED_PLUGINS, m_lbPlugins);
	DDX_Control(pDX, IDC_OUT_PLUGINS, m_outPlugins);
	DDX_Text(pDX, IDC_PLUGIN_DETAILS, m_pluginDet);
	DDX_LBString(pDX, IDC_INSTALLED_PLUGINS, m_pluginName);
	DDX_LBString(pDX, IDC_OUT_PLUGINS, m_outName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginPage, CDialog)
	//{{AFX_MSG_MAP(CPluginPage)
	ON_LBN_SELCHANGE(IDC_INSTALLED_PLUGINS, OnSelchangeInstalledPlugins)
	ON_LBN_SELCHANGE(IDC_OUT_PLUGINS, OnSelchangePluginsOut)
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	ON_BN_CLICKED(IDC_ABOUT_PLUGIN, OnAboutPlugin)
	ON_BN_CLICKED(IDC_UNINSTALL, OnUninstall)
	ON_BN_CLICKED(IDC_CONFIGURE_OUT, OnConfigureOut)
	ON_BN_CLICKED(IDC_ABOUT_OUT, OnAboutOut)
	ON_BN_CLICKED(IDC_INSTALL, OnUnInstallOut)
	ON_LBN_DBLCLK(IDC_INSTALLED_PLUGINS, OnDblclkInstalledPlugins)
	ON_LBN_DBLCLK(IDC_OUT_PLUGINS, OnConfigureOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPluginPage::Init(CString wdir)
{

	wd = wdir;
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPluginPage 

BOOL CPluginPage::OnInitDialog()
{

	CPropertyPage::OnInitDialog();

	//m_lbPlugins.AddString("plugins\\wave_in.dll");
	if(CheckForPlugins()){
		
		if(m_lbPlugins.GetCount())
			m_lbPlugins.SetCurSel(0);
		if(m_outPlugins.GetCount())
			m_outPlugins.SetCurSel(0);
		//OnSelchangePluginsOut();
		OnSelchangeInstalledPlugins();
	}

	return TRUE;

}

void CPluginPage::OnConfigure() 
{

	if(!m_lbPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}

	CString name;
	m_lbPlugins.GetText(m_lbPlugins.GetCurSel(), name);

	if(name.Right(6) == "in.dll"){	//Is this a LameFE Plugin?

		CLameFEPlugin plugin(wd + "\\Plugins\\" + name);
		plugin.Load();
		plugin.Configure();
		plugin.Unload();
	}
}

void CPluginPage::OnAboutPlugin() 
{

	if(!m_lbPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}

	CString name;
	m_lbPlugins.GetText(m_lbPlugins.GetCurSel(), name);

	if(name.Right(6) == "in.dll"){	//Is this a LameFE Plugin?

		CLameFEPlugin plugin(wd + "\\Plugins\\" + name);
		plugin.Load();
		plugin.About();
		plugin.Unload();
	}

}

void CPluginPage::OnSelchangeInstalledPlugins() 
{

	if(!m_lbPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}
	CString name;
	m_lbPlugins.GetText(m_lbPlugins.GetCurSel(), name);
	
	if(name.Right(6) == "in.dll"){	//Is this a LameFE Plugin?

		CLameFEPlugin plugin(wd + "\\Plugins\\" + name);
		plugin.Load();
		m_pluginDet = plugin.GetInfoString();
		plugin.Unload();
	}
	UpdateData(FALSE);
}

BOOL CPluginPage::CheckForPlugins()
{

	CFileFind finder;

	BOOL bResult = finder.FindFile(wd + "\\Plugins\\*_in.dll");
	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s.\n", finder.GetFileName());
		m_lbPlugins.AddString(finder.GetFileName());
	}
	

	bResult = finder.FindFile(wd + "\\Plugins\\*_out.dll");

	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s. Uses Output!\n", finder.GetFileName());
		m_outPlugins.AddString(finder.GetFileName());
	}
	

	if(!m_lbPlugins.GetCount()){

		return FALSE;
	}

	return TRUE;
}

void CPluginPage::OnUninstall() 
{

	if(m_lbPlugins.GetSelCount() == 0){

		AfxMessageBox(IDS_PLUGIN_NOSEL, MB_OK+MB_ICONSTOP);
	}

	UpdateData(TRUE);

	CString msg, completePath;
	
	msg.Format(IDS_PLUGIN_DEINST, m_pluginName);
	completePath = wd + "\\Plugins\\" + m_pluginName;

	if(AfxMessageBox(msg, MB_YESNO+MB_ICONQUESTION) == IDYES){

		TRY{

			CFile::Remove(completePath);
		}
		CATCH(CFileException, e){

			msg.Format(IDS_PLUGIN_DEINSTFAILED, completePath);
			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);

		}END_CATCH;

		m_lbPlugins.ResetContent();
		if(CheckForPlugins()){
			
			m_lbPlugins.SetCurSel(0);
			OnSelchangeInstalledPlugins();
		}

	}


}

void CPluginPage::OnConfigureOut()
{

	if(!m_lbPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}

	CString name;
	m_outPlugins.GetText(m_outPlugins.GetCurSel(), name);
	
	if(name.Right(7) == "out.dll"){  // or an output Plugin?

		COutPlugin plugin(wd + "\\Plugins\\" + name, wd + "\\lameFE.ini");
		plugin.Load();
		plugin.Configure();
		plugin.Unload();
	}
}

void CPluginPage::OnAboutOut()
{

	if(!m_lbPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}

	CString name;
	m_outPlugins.GetText(m_outPlugins.GetCurSel(), name);

	if(name.Right(7) == "out.dll"){  // or an output Plugin?

		COutPlugin plugin(wd + "\\Plugins\\" + name, wd + "\\lameFE.ini");
		plugin.Load();
		plugin.About();
		plugin.Unload();
	}
}

void CPluginPage::OnUnInstallOut()
{

	if(m_lbPlugins.GetSelCount() == 0){

		AfxMessageBox(IDS_PLUGIN_NOSEL, MB_OK+MB_ICONSTOP);
	}

	UpdateData(TRUE);

	CString msg, completePath;
	
	msg.Format(IDS_PLUGIN_DEINST, m_outName);
	completePath = wd + "\\Plugins\\" + m_outName;

	if(AfxMessageBox(msg, MB_YESNO+MB_ICONQUESTION) == IDYES){

		TRY{

			CFile::Remove(completePath);
		}
		CATCH(CFileException, e){

			msg.Format(IDS_PLUGIN_DEINSTFAILED, completePath);
			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);

		}END_CATCH;

		m_lbPlugins.ResetContent();
		m_outPlugins.ResetContent();
		if(CheckForPlugins()){
			
			m_lbPlugins.SetCurSel(0);
			OnSelchangeInstalledPlugins();
		}

	}
}


void CPluginPage::OnOK(){

}

void CPluginPage::OnDblclkInstalledPlugins() 
{

	OnConfigure();
}

void CPluginPage::OnSelchangePluginsOut()
{

	if(!m_outPlugins.GetCount()){

		AfxMessageBox("No plugins installed", MB_OK+MB_ICONINFORMATION);
		return;
	}
	CString name;
	m_outPlugins.GetText(m_outPlugins.GetCurSel(), name);
	
	if(name.Right(7) == "out.dll"){  //  an output Plugin?
		
		COutPlugin plugin(wd + "\\Plugins\\" + name, wd + "\\lameFE.ini");
		plugin.Load();
		m_pluginDet = plugin.GetInfoString();
		plugin.Unload();
	}
	UpdateData(FALSE);
}

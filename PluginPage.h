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

#if !defined(AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_)
#define AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginPage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPluginPage 

class CPluginPage : public CPropertyPage
{

	DECLARE_DYNCREATE(CPluginPage)
// Konstruktion
public:
	void OnSelchangePluginsOut();
	void OnOK();
	void Init(CString wdir);
	CPluginPage();   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CPluginPage)
	enum { IDD = IDD_PLUGINPAGE };
	CListBox	m_lbPlugins;
	CListBox	m_outPlugins;
	CString	m_pluginDet;
	CString	m_pluginName;
	CString m_outName;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPluginPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPluginPage)
	afx_msg void OnConfigure();
	afx_msg void OnSelchangeInstalledPlugins();
	afx_msg void OnAboutPlugin();
	afx_msg void OnUninstall();
	afx_msg void OnDblclkInstalledPlugins();
	afx_msg void OnConfigureOut();
	afx_msg void OnAboutOut();
	afx_msg void OnUnInstallOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString wd;
	BOOL CheckForPlugins();
	HINSTANCE hDLL;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_

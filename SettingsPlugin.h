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

#if !defined(AFX_SETTINGSPLUGIN_H__3029643D_FA99_4F7A_A5EF_6F4566D94156__INCLUDED_)
#define AFX_SETTINGSPLUGIN_H__3029643D_FA99_4F7A_A5EF_6F4566D94156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsPlugin.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsPlugin 

#include "MySettingsPage.h"

class CSettingsPlugin : public CMySettingsPage
{
// Konstruktion
public:
	void LoadPlugin(CString strPlugin, BOOL isInput = TRUE);
	BOOL Save();
	void Init(CString strWd);
	CSettingsPlugin(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsPlugin)
	enum { IDD = IDD_SETTINGS_PLUGINCODEC };
	CString	m_strAbout;
	CString	m_strExt;
	CString	m_strPluginname;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsPlugin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CString m_strPluginDir;
	CString m_strPlugin;
	BOOL	m_isInput;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsPlugin)
	afx_msg void OnConfiguration();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSPLUGIN_H__3029643D_FA99_4F7A_A5EF_6F4566D94156__INCLUDED_

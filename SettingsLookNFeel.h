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

#if !defined(AFX_SETTINGSLOOKNFEEL_H__7D788703_DE26_47EC_BF36_5873989BE17C__INCLUDED_)
#define AFX_SETTINGSLOOKNFEEL_H__7D788703_DE26_47EC_BF36_5873989BE17C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsLookNFeel.h : Header-Datei
//
#include "MySettingsPage.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsLookNFeel 

class CSettingsLookNFeel : public CMySettingsPage
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CSettingsLookNFeel(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsLookNFeel)
	enum { IDD = IDD_SETTINGS_LOOKNFEEL };
	CButton	c_rememberEncoder;
	CButton	c_teditOnFile;
	CButton	c_hideMainWnd;
	CButton	c_cdTextRead;
	CButton	c_writeCDPlayerIni;
	CButton	c_readCDPlayerIni;
	CButton	c_silent;
	CButton c_saveWinPos;
	CButton c_highColBar;
	CButton c_showSplash;
	CButton c_showAlbumEditor;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsLookNFeel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsLookNFeel)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	afx_msg void OnHighColorIcons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSLOOKNFEEL_H__7D788703_DE26_47EC_BF36_5873989BE17C__INCLUDED_

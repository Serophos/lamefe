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

#if !defined(AFX_SETTINGSFILENAMES_H__04571F32_48F2_47EB_AC27_A542AA1299AB__INCLUDED_)
#define AFX_SETTINGSFILENAMES_H__04571F32_48F2_47EB_AC27_A542AA1299AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsFilenames.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsFilenames 

#include "MySettingsPage.h"
#include "MyEditCtrl.h"
#include "MFECToolTip.h"

class CSettingsFilenames : public CMySettingsPage
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CSettingsFilenames(CWnd* pParent = NULL);   // Standardkonstruktor

	void CreatePreview(CString &in, CString &out);
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	
// Dialogfelddaten
	//{{AFX_DATA(CSettingsFilenames)
	enum { IDD = IDD_SETTINGS_FILENAMES };
	CEdit	m_ctrlAlbum;
	CEdit	m_ctrlPlaylist;
	CEdit	m_ctrlFilename;
	CStatic m_ctrlPath;
	CString	m_strAlbumMode;
	CString	m_strAlbumPrev;
	CString	m_strFilenamePrev;
	CString	m_strFilename;
	CString	m_strPlaylist;
	CString	m_strPlaylistPrev;
	CString m_strOutputPath;
	BOOL	m_bRename;
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsFilenames)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	afx_msg void OnRename();
	afx_msg void OnChangeAlbumModeFilename();
	afx_msg void OnChangeFormat();
	afx_msg void OnChangePlaylistFilename();
	afx_msg void OnPath();
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CMFECToolTip	m_mToolTip;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsFilenames)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSFILENAMES_H__04571F32_48F2_47EB_AC27_A542AA1299AB__INCLUDED_

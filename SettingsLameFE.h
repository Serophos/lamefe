/*
** Copyright (C) 2002-2003 Thees Winkler
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

#if !defined(AFX_SETTINGSLAMEFE_H__2FFC4748_35D8_4651_93D1_D1FEB57E1FD1__INCLUDED_)
#define AFX_SETTINGSLAMEFE_H__2FFC4748_35D8_4651_93D1_D1FEB57E1FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsLameFE.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsLameFE 
#include "MySettingsPage.h"

class CSettingsLameFE : public CMySettingsPage
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CSettingsLameFE(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsLameFE)
	enum { IDD = IDD_SETTINGS_LAMEFE };
	CButton	c_enqueueFiles;
	CButton	c_shutdownOnFinished;
	CButton	c_playFiles;
	CButton	c_m3u;
	CButton	c_finishedDialog;
	CButton	c_quitOnFinished;
	CButton	c_beep;
	CButton c_getfocus;
	CButton c_playSound;
	CString	m_playerPath;
	CString	m_presetPath;
	CString	m_strTempPath;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsLameFE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsLameFE)
	afx_msg void OnPlayer();
	afx_msg void OnPlayFiles();
	afx_msg void OnPresetpath();
	afx_msg void OnTemppath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSLAMEFE_H__2FFC4748_35D8_4651_93D1_D1FEB57E1FD1__INCLUDED_

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

#if !defined(AFX_SETTINGSBATCHMODE_H__C87D0C82_2204_465A_9044_6342F6C3DF15__INCLUDED_)
#define AFX_SETTINGSBATCHMODE_H__C87D0C82_2204_465A_9044_6342F6C3DF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsBatchMode.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsBatchMode 
#include "MySettingsPage.h"
#include "MyEditCtrl.h"

class CSettingsBatchMode : public CMySettingsPage
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CSettingsBatchMode(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsBatchMode)
	enum { IDD = IDD_SETTINGS_BATCHMODE };
	CListBox	m_lAvailable;
	CListBox	m_lActive;
	CButton	c_appendDiscID;
	CEdit	c_nBatchTimeOut;
	CButton	c_batchTimeOut;
	CButton	c_batchAllDrives;
	int		m_nTimeOut;
	CString	m_strActive;
	CString	m_strAvailable;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsBatchMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsBatchMode)
	afx_msg void OnBatchtimeout();
	afx_msg void OnAddCommand();
	afx_msg void OnRemoveCommand();
	afx_msg void OnCommandUp();
	afx_msg void OnCommandDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSBATCHMODE_H__C87D0C82_2204_465A_9044_6342F6C3DF15__INCLUDED_

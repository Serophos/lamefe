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

#if !defined(AFX_MYSETTINGSPAGE_H__9AA4499A_5100_48AD_B604_B9C95F7AB174__INCLUDED_)
#define AFX_MYSETTINGSPAGE_H__9AA4499A_5100_48AD_B604_B9C95F7AB174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySettingsPage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CMySettingsPage 


class CMySettingsPage : public CDialog
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CMySettingsPage(UINT nIDTemplate = NULL, CWnd* pParentWnd = NULL );   // Standardkonstruktor
	~CMySettingsPage();
// Dialogfelddaten
	//{{AFX_DATA(CMySettingsPage)
	enum { IDD = NULL };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMySettingsPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CString			m_strWd;
	CToolTipCtrl*	m_pToolTip;

	void OnOK();
	void OnCancel();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CMySettingsPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//typedef CArray<CMySettingsPage, CMySettingsPage> CSettingsPageArray;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MYSETTINGSPAGE_H__9AA4499A_5100_48AD_B604_B9C95F7AB174__INCLUDED_

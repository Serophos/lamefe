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

#if !defined(AFX_FREEDBSTATUSDLG_H__BC4E3A2E_7362_4BD9_B156_862824C0DE4B__INCLUDED_)
#define AFX_FREEDBSTATUSDLG_H__BC4E3A2E_7362_4BD9_B156_862824C0DE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreeDBStatusDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld FreeDBStatusDlg 

class FreeDBStatusDlg : public CDialog
{
// Konstruktion
public:
	void SetMessage(UINT nID);
	void SetMessage(CString strMsg);
	FreeDBStatusDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(FreeDBStatusDlg)
	enum { IDD = IDD_FREEDB_STATUS };
	CString	m_strStatusMsg;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(FreeDBStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(FreeDBStatusDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FREEDBSTATUSDLG_H__BC4E3A2E_7362_4BD9_B156_862824C0DE4B__INCLUDED_

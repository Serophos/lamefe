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

#if !defined(AFX_CDDBQUERYDLG_H__64D50E33_2533_4AB2_9D78_F1120317F911__INCLUDED_)
#define AFX_CDDBQUERYDLG_H__64D50E33_2533_4AB2_9D78_F1120317F911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDdbQueryDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCDdbQueryDlg 

#include "mfccddb.h"
#include "CompactDisk.h"

class CCDdbQueryDlg : public CDialog
{
// Konstruktion
public:
	CCDdbQueryDlg(CWnd* pParent = NULL, CCompactDisc *cd = NULL, int nActiveCD = 0, BOOL bAutoSelect = FALSE);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CCDdbQueryDlg)
	enum { IDD = IDD_CDDB_QUERY };
	CButton	m_Start;
	CListBox	m_Protocoll;
	CButton	m_Cancel;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CCDdbQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual BOOL OnInitDialog();
	void	ErrMessageBox(CString strMsg, UINT nType);
	void	ErrMessageBox(UINT nID, UINT nType);
	BOOL	QueryCDDB();
	BOOL	GetTrackPositions(CArray<CCDDBTrackPosition, CCDDBTrackPosition&>& tracks);
	CString	GetServerString(int i);
	void	AddLine(CString msg);

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CCDdbQueryDlg)
	virtual void OnCancel();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	int				m_nActiveCD;
	CCompactDisc*	m_cd;
	BOOL			m_bAutoSelect;

	CCDDB			cddb;
	CCDDBSite		site;
	DWORD			m_dwDiscID;
	CArray<CCDDBQueryResult, CCDDBQueryResult&> results;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CDDBQUERYDLG_H__64D50E33_2533_4AB2_9D78_F1120317F911__INCLUDED_

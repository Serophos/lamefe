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

#if !defined(AFX_CHECKNEWVERSION_H__03166297_54BE_4410_8643_F9A48D5BF46D__INCLUDED_)
#define AFX_CHECKNEWVERSION_H__03166297_54BE_4410_8643_F9A48D5BF46D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckNewVersion.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCheckNewVersion 

class CCheckNewVersion : public CDialog
{
// Konstruktion
public:
	CCheckNewVersion(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CCheckNewVersion)
	enum { IDD = IDD_CHECKNEWVERSION };
	CButton	m_cDownload;
	CString	m_strVersionMsg;
	CString	m_strNewVersion;
	CString	m_strOldVersion;
	CString	m_strChangeLog;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CCheckNewVersion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CCheckNewVersion)
	virtual void OnOK();
	afx_msg void OnDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_CHECKNEWVERSION_H__03166297_54BE_4410_8643_F9A48D5BF46D__INCLUDED_

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

#if !defined(AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_)
#define AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlbumInfoCtrl.h : Header-Datei
//

#include "ID3Info.h"
#include "MyEditCtrl.h"
#include "MyComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAlbumInfoCtrl 


class CAlbumInfoCtrl : public CDialog
{
// Konstruktion
public:
	void OnOK();
	void OnCancel();
	void Clear();
	void EnableControls(BOOL bEnable);
	void SetInfo(CID3Info* info);
	CAlbumInfoCtrl(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CAlbumInfoCtrl)
	enum { IDD = IDD_ALBUMINFOCTRL };
	CMyEditCtrl	m_cYear;
	CMyEditCtrl	m_cTrackNumber;
	CMyEditCtrl	m_cSongTitle;
	CMyEditCtrl	m_cSongInterpret;
	CMyEditCtrl	m_cComment;
	CMyEditCtrl	m_cAlbumInfo;
	CMyComboBox	m_ctrlGenre;
	CString	m_strAlbum;
	CString	m_strComment;
	CString	m_strGenre;
	CString	m_strInterpret;
	CString	m_strSong;
	int		m_nTrack;
	int		m_nYear;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CAlbumInfoCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CAlbumInfoCtrl)
	afx_msg void OnChangeAlbumname();
	afx_msg void OnChangeAlbumInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL	m_bDataChanged;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_

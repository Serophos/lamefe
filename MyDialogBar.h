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

#if !defined(AFX_MYDIALOGBAR_H__57AE87EB_F3A8_409D_9F84_12DD8E90306E__INCLUDED_)
#define AFX_MYDIALOGBAR_H__57AE87EB_F3A8_409D_9F84_12DD8E90306E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDialogBar.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CMyDialogBar 


class CMyDialogBar : public CDialogBar
{
// Konstruktion
public:
	CMyDialogBar();

// Attribute
public:

// Operationen
public:

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CMyDialogBar)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMyDialogBar();
	virtual void OnInitDialogBar();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CMyDialogBar)
		// HINWEIS - Der Klassen-Assistent f�gt hier Member-Funktionen ein und entfernt diese.
	afx_msg void InitDialogBarHandler( WORD wParam , DWORD  lParam );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_MYDIALOGBAR_H__57AE87EB_F3A8_409D_9F84_12DD8E90306E__INCLUDED_

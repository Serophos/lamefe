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

#if !defined(AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_)
#define AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LameFESplash.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CLameFESplash 

class CLameFESplash : public CWnd
{
// Konstruktion
public:
	CLameFESplash(UINT nBitmapID, UINT nDuration = 2500);

// Attribute
public:
    BOOL Create();


// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CLameFESplash)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CLameFESplash();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CLameFESplash)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &pal);

protected:
	UINT m_nBitmapID;
	UINT m_nDuration;
	UINT m_nTimerID;
	CBitmap m_bitmap;
	CPalette m_pal;
	CWnd m_wndInvisible;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_

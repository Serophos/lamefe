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

#if !defined(AFX_MYEDITCTRL_H__C093A1D3_A608_4077_AFC7_DBC0BB25EED2__INCLUDED_)
#define AFX_MYEDITCTRL_H__C093A1D3_A608_4077_AFC7_DBC0BB25EED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEditCtrl.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CMyEditCtrl 

class CMyEditCtrl : public CEdit
{
// Konstruktion
public:
	CMyEditCtrl(BOOL bDrawSeperator = FALSE);
	void DrawEdit(int style, COLORREF clrTopLeft, COLORREF clrBottomRight);
	void SetIndent(int margin);

// Attribute
public:
	BOOL	 m_bLBtnDown;
	COLORREF m_clrHilite;
	COLORREF m_clrShadow;
	COLORREF m_clrDkShad;
	COLORREF m_clrButton;
	BOOL	 m_bHasFocus;	// TRUE if control has focus
	BOOL	 m_bDrawGripper;
	BOOL	 m_bDrawSeperator;
	BOOL	 m_bFlatStyle;	//We got 2 flat styles, true is to make it 'Microsoft Flat'
							// false is to make it look like the heavy border around check boxes that are flat

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMyEditCtrl)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMyEditCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CMyEditCtrl)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetFocus();
	afx_msg BOOL OnKillFocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MYEDITCTRL_H__C093A1D3_A608_4077_AFC7_DBC0BB25EED2__INCLUDED_

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

#if !defined(AFX_EXTLISTCTRL_H__1472B914_B54F_40F4_9DBE_4FEE3803D817__INCLUDED_)
#define AFX_EXTLISTCTRL_H__1472B914_B54F_40F4_9DBE_4FEE3803D817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtListCtrl.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CExtListCtrl 

class CExtListCtrl : public CListCtrl
{
// Konstruktion
public:
	CExtListCtrl();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExtListCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementierung
public:
	void InvertSelection();
	BOOL Init(BOOL bFlat = TRUE);
	int GetCurSel() const;
	void SelectAll();
	void SetSelItems(int iCnt, LPINT pIndex);
	int GetSelCount() const;
	int GetSelItems( int nMaxItems, LPINT rgIndex ) const;
	virtual ~CExtListCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	enum STATE { normal = 1, raised = 2, pressed = 3 };
	void DrawListCtrl(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight);
	BOOL		m_bFlatStyle;
	int			m_nOffset;		// offset used during paint.
	BOOL		m_bLBtnDown;	// TRUE if left mouse button is pressed
	BOOL		m_bPainted;		// used during paint operations
	BOOL		m_bHasFocus;	// TRUE if control has focus
	COLORREF	m_clrBtnHilite;	// set to the system color COLOR_BTNHILIGHT
	COLORREF	m_clrBtnShadow;	// set to the system color COLOR_BTNSHADOW
	COLORREF	m_clrBtnFace;	// set to the system color COLOR_BTNFACE

	//{{AFX_MSG(CExtListCtrl)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTLISTCTRL_H__1472B914_B54F_40F4_9DBE_4FEE3803D817__INCLUDED_

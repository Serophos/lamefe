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

#if !defined(AFX_MYCOMBOBOX_H__2FF196EE_629F_4C50_8035_873A0D981878__INCLUDED_)
#define AFX_MYCOMBOBOX_H__2FF196EE_629F_4C50_8035_873A0D981878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyComboBox.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CMyComboBox 

class CMyComboBox : public CComboBox
{
// Konstruktion
public:
	CMyComboBox();

// Attribute
public:

// Operationen
public:

// Implementierung
public:
	virtual ~CMyComboBox();
	void EnableAutoCompletion(BOOL bEnable=TRUE);

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	int			m_nOffset;		// offset used during paint.
	BOOL		m_bLBtnDown;	// TRUE if left mouse button is pressed
	BOOL		m_bPainted;		// used during paint operations
	BOOL		m_bHasFocus;	// TRUE if control has focus
	BOOL		m_bAutoComp;	// Used by Autocompleting.
	COLORREF	m_clrBtnHilite;	// set to the system color COLOR_BTNHILIGHT
	COLORREF	m_clrBtnShadow;	// set to the system color COLOR_BTNSHADOW
	COLORREF	m_clrBtnFace;	// set to the system color COLOR_BTNFACE
	enum STATE { normal = 1, raised = 2, pressed = 3 };

	void DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyComboBox)
	afx_msg BOOL OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetFocus();
	afx_msg BOOL OnKillFocus();
	afx_msg void OnEditUpdate();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MYCOMBOBOX_H__2FF196EE_629F_4C50_8035_873A0D981878__INCLUDED_

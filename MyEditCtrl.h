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
	CMyEditCtrl();
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus();
	afx_msg BOOL OnKillFocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MYEDITCTRL_H__C093A1D3_A608_4077_AFC7_DBC0BB25EED2__INCLUDED_

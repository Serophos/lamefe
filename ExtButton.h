#if !defined(AFX_EXTBUTTON_H__73C1B85B_BCC5_4C67_A2B1_B47F71B7E824__INCLUDED_)
#define AFX_EXTBUTTON_H__73C1B85B_BCC5_4C67_A2B1_B47F71B7E824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtButton.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CExtButton 

class CExtButton : public CButton
{
// Konstruktion
public:
	CExtButton();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExtButton)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CExtButton();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	enum STATE { normal = 1, raised = 2, pressed = 3 };
	void DrawButton(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight);
	int			m_nOffset;		// offset used during paint.
	BOOL		m_bLBtnDown;	// TRUE if left mouse button is pressed
	BOOL		m_bPainted;		// used during paint operations
	BOOL		m_bHasFocus;	// TRUE if control has focus
	BOOL		m_bAutoComp;	// Used by Autocompleting.
	COLORREF	m_clrBtnHilite;	// set to the system color COLOR_BTNHILIGHT
	COLORREF	m_clrBtnShadow;	// set to the system color COLOR_BTNSHADOW
	COLORREF	m_clrBtnFace;	// set to the system color COLOR_BTNFACE
	//{{AFX_MSG(CExtButton)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTBUTTON_H__73C1B85B_BCC5_4C67_A2B1_B47F71B7E824__INCLUDED_

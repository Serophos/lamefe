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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementierung
public:
	BOOL Init();
	int GetCurSel() const;
	void SelectAll();
	void SetSelItems(int iCnt, LPINT pIndex);
	int GetSelCount() const;
	int GetSelItems( int nMaxItems, LPINT rgIndex ) const;
	virtual ~CExtListCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CExtListCtrl)
	afx_msg void OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTLISTCTRL_H__1472B914_B54F_40F4_9DBE_4FEE3803D817__INCLUDED_

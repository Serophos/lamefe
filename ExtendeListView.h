#if !defined(AFX_EXTENDELISTVIEW_H__5ECC2AF5_144B_4C78_B58F_18C3CCED6F50__INCLUDED_)
#define AFX_EXTENDELISTVIEW_H__5ECC2AF5_144B_4C78_B58F_18C3CCED6F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtendeListView.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CExtendeListView 

class CExtendeListView : public CListCtrl
{
// Konstruktion
public:
	CExtendeListView();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExtendeListView)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CExtendeListView();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CExtendeListView)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	afx_msg void OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTENDELISTVIEW_H__5ECC2AF5_144B_4C78_B58F_18C3CCED6F50__INCLUDED_

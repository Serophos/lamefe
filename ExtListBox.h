#if !defined(AFX_EXTLISTBOX_H__80F70388_74EC_4350_A627_8707BA6B5A78__INCLUDED_)
#define AFX_EXTLISTBOX_H__80F70388_74EC_4350_A627_8707BA6B5A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtListBox.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CExtListBox 

class CExtListBox : public CListBox
{
// Konstruktion
public:
	CExtListBox();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExtListBox)
	protected:
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CExtListBox();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CExtListBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CMenu m_cmnMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTLISTBOX_H__80F70388_74EC_4350_A627_8707BA6B5A78__INCLUDED_

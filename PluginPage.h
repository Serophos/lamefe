#if !defined(AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_)
#define AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginPage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPluginPage 

class CPluginPage : public CPropertyPage
{

	DECLARE_DYNCREATE(CPluginPage)
// Konstruktion
public:
	CPluginPage();   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CPluginPage)
	enum { IDD = IDD_PLUGINPAGE };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPluginPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPluginPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_

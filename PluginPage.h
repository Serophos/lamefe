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
		// HINWEIS: Der Klassen-Assistent f�gt hier Datenelemente ein
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CPluginPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPluginPage)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_PLUGINPAGE_H__73DDCE17_B942_45F3_B67E_65083F2D02F5__INCLUDED_

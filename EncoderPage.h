#if !defined(AFX_ENCODERPAGE_H__DD76DDCE_FD7E_4D1C_8567_7F9A64B4B1FF__INCLUDED_)
#define AFX_ENCODERPAGE_H__DD76DDCE_FD7E_4D1C_8567_7F9A64B4B1FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EncoderPage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CEncoderPage 

class CEncoderPage : public CDialog
{
// Konstruktion
public:
	CEncoderPage(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CEncoderPage)
	enum { IDD = IDD_ENCODER_PAGE };
		// HINWEIS: Der Klassen-Assistent f�gt hier Datenelemente ein
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CEncoderPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CEncoderPage)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_ENCODERPAGE_H__DD76DDCE_FD7E_4D1C_8567_7F9A64B4B1FF__INCLUDED_

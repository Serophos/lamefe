#if !defined(AFX_CONFIGDIALOG_H__6CBD0EAC_AC3F_470D_836E_459F24141F51__INCLUDED_)
#define AFX_CONFIGDIALOG_H__6CBD0EAC_AC3F_470D_836E_459F24141F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDialog.h : Header-Datei
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDialog 

class CConfigDialog : public CDialog
{
// Konstruktion
public:
	virtual BOOL OnInitDialog();
	CConfigDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	int	m_nChannels;
	int	m_nSamplingRate;
	int	m_nCompression;

// Dialogfelddaten
	//{{AFX_DATA(CConfigDialog)
	enum { IDD = IDD_SETTINGS };
	CComboBox	m_cChannels;
	CComboBox	m_cSamplingRate;
	CComboBox	m_cCompression;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CConfigDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CConfigDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CONFIGDIALOG_H__6CBD0EAC_AC3F_470D_836E_459F24141F51__INCLUDED_

#if !defined(AFX_CONFIGDLG_H__E963BE5B_5513_4D39_8CA1_919F399017AF__INCLUDED_)
#define AFX_CONFIGDLG_H__E963BE5B_5513_4D39_8CA1_919F399017AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : Header-Datei
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDlg 

class CConfigDlg : public CDialog
{
// Konstruktion
public:
	virtual BOOL OnInitDialog();
	CConfigDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	CComboBox	m_cCompression;
	int		m_nCompressionLevel;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CConfigDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_CONFIGDLG_H__E963BE5B_5513_4D39_8CA1_919F399017AF__INCLUDED_

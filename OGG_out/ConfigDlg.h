#if !defined(AFX_CONFIGDLG_H__47592826_5837_40B3_97B8_738D7134D224__INCLUDED_)
#define AFX_CONFIGDLG_H__47592826_5837_40B3_97B8_738D7134D224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDlg 
#include "resource.h"

class CConfigDlg : public CDialog
{
// Konstruktion
public:
	virtual BOOL OnInitDialog();
	CConfigDlg(CWnd* pParent = NULL, CString wd = "");   // Standardkonstruktor
// Dialogfelddaten
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	CComboBox	m_cMinBitrate;
	CComboBox	m_cNomBitrate;
	CComboBox	m_cMaxBitrate;
	int		m_nMaxBitrate;
	int		m_nMinBitrate;
	int		m_nNomBitrate;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CConfigDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strWd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CONFIGDLG_H__47592826_5837_40B3_97B8_738D7134D224__INCLUDED_

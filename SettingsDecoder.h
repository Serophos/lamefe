#if !defined(AFX_SETTINGSDECODER_H__E1D12A80_5DCC_41C0_912F_8A1522EDC54C__INCLUDED_)
#define AFX_SETTINGSDECODER_H__E1D12A80_5DCC_41C0_912F_8A1522EDC54C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDecoder.h : Header-Datei
//
#include "MySettingsPage.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsDecoder 

class CSettingsDecoder : public CMySettingsPage
{
// Konstruktion
public:
	CSettingsDecoder(CWnd* pParent = NULL);   // Standardkonstruktor
	BOOL Save();
	void Init(CString strWd);

// Dialogfelddaten
	//{{AFX_DATA(CSettingsDecoder)
	enum { IDD = IDD_SETTINGS_DECODER };
	CButton	 m_cFile;
	CButton  m_cCD;
	CButton  m_cLastDec;
	CString	m_strPath;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsDecoder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsDecoder)
	afx_msg void OnCD();
	afx_msg void OnFile();
	afx_msg void OnWinampPath();
	afx_msg void OnRememberLast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSDECODER_H__E1D12A80_5DCC_41C0_912F_8A1522EDC54C__INCLUDED_

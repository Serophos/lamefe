/*
** Copyright (C) 2002 - 2003 Thees Winkler
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

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
#include "../out_plugin.h"

class CConfigDlg : public CDialog
{
// Konstruktion
public:
	void UpdateQuality();
	int m_nMode;
	virtual BOOL OnInitDialog();
	CConfigDlg(CWnd* pParent = NULL, LF_OUT* plf = NULL);   // Standardkonstruktor
// Dialogfelddaten
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	CSliderCtrl	m_slQuality;
	CComboBox	m_cMinBitrate;
	CComboBox	m_cNomBitrate;
	CComboBox	m_cMaxBitrate;
	int		m_nMaxBitrate;
	int		m_nMinBitrate;
	int		m_nNomBitrate;
	int		m_nQuality;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	LF_OUT* m_plfOut;
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CConfigDlg)
	virtual void OnOK();
	afx_msg void OnQualitymanaged();
	afx_msg void OnVariable();
	afx_msg void OnAverage();
	afx_msg void OnConstant();
	afx_msg void OnSelchangeNominalBitrate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//	CString m_strWd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CONFIGDLG_H__47592826_5837_40B3_97B8_738D7134D224__INCLUDED_

/*
** Copyright (C) 2002 Thees Winkler
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

#if !defined(AFX_CDROMPAGE_H__DEE6EDBF_77A8_4FD8_889B_0E0152C9FD2B__INCLUDED_)
#define AFX_CDROMPAGE_H__DEE6EDBF_77A8_4FD8_889B_0E0152C9FD2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDROMPage.h : Header-Datei
//

#include "MyEditCtrl.h"
#include "MyComboBox.h"


/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCDROMPage 

class CCDROMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCDROMPage)

// Konstruktion
public:
	BOOL m_bRipperPresent;
	virtual BOOL OnInitDialog();
	void OnCancel();
	void OnOK();
	void init(CString wdir, BOOL bRipperPresent);
	CCDROMPage();
	~CCDROMPage();

// Dialogfelddaten
	//{{AFX_DATA(CCDROMPage)
	enum { IDD = IDD_CDRIP_PAGE };
	CButton	c_checkNewCD;
	CMyComboBox	c_numBuffers;
	CMyEditCtrl	c_nBatchTimeOut;
	CButton	c_batchBeep;
	CButton	c_batchTimeOut;
	CButton	c_batchAllDrives;
	CButton	c_batchFreeDB;
	CMyEditCtrl	c_spinUpTime;
	CButton	c_nativeSCSI;
	CMyEditCtrl	c_cdSpeed;
	CButton	c_select;
	CButton	c_swapChannels;
	CMyComboBox	c_rippingMethod;
	CButton	c_lockDrive;
	CButton	c_ejectWhenFinished;
	CMyComboBox	c_cdromType;
	CMyComboBox	c_cdDrive;
	CString	m_cdRipInfo;
	CString	m_cdripVersion;
	int		m_cdSpeed;
	int		m_spinUpTime;
	BOOL	m_nativeSCSI;
	CButton c_cue;
	int		m_nTimeOut;
	int		m_numBuffers;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CCDROMPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CCDROMPage)
	afx_msg void OnNative();
	afx_msg void OnSelchangeCdDrive();
	afx_msg void OnBatchtimeout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CToolTipCtrl* m_pToolTip;
	void initControls();
	void SaveSettings();
	CString wd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CDROMPAGE_H__DEE6EDBF_77A8_4FD8_889B_0E0152C9FD2B__INCLUDED_

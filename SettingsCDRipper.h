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

#if !defined(AFX_SETTINGSCDRIPPER_H__FA093A8F_C0CC_4E3A_82EE_0FF4E175B970__INCLUDED_)
#define AFX_SETTINGSCDRIPPER_H__FA093A8F_C0CC_4E3A_82EE_0FF4E175B970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsCDRipper.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsCDRipper 
#include "MySettingsPage.h"
#include "MyEditCtrl.h"
#include "MyComboBox.h"

class CSettingsCDRipper : public CMySettingsPage
{
// Konstruktion
public:
	void SetControls();
	BOOL Save();
	void Init(CString strWd);
	CSettingsCDRipper(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsCDRipper)
	enum { IDD = IDD_SETTINGS_CDRIPPER };
	CButton	m_cUse2Phase;
	CButton	c_appendDiscID;
	CButton	c_checkNewCD;
	CEdit	c_spinUpTime;
	CButton	c_nativeSCSI;
	CEdit	c_cdSpeed;
	CButton	c_select;
	CButton	c_swapChannels;
	CComboBox	c_rippingMethod;
	CButton	c_lockDrive;
	CButton	c_ejectWhenFinished;
	CComboBox	c_cdromType;
	CComboBox	c_cdDrive;
	CString	m_cdRipInfo;
	int		m_cdSpeed;
	int		m_spinUpTime;
	BOOL	m_nativeSCSI;
	CButton c_cue;
	int		m_nBlockCompare;
	int		m_nEndOffset;
	int		m_nReadOverlap;
	int		m_nReadSectors;
	int		m_nStartOffset;
	BOOL	m_bJitter;
	BOOL	m_bLockDuringRead;
	BOOL	m_bSwapChannels;
	int		m_nRetries;
	BOOL	m_bUse2Phase;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsCDRipper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void InitControls();
	BOOL m_bRipperPresent;
	BOOL m_bAspiPosting;
	BOOL m_bUseCDText;
	BOOL m_bFirstBlockOnly;
	BOOL m_bMultipleRead;
	int m_nMultipleReadCount;
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsCDRipper)
	afx_msg void OnNative();
	afx_msg void OnSelchangeCdDrive();
	afx_msg void OnAutodetect();
	afx_msg void OnSelchangeRippingMethod();
	afx_msg void OnEnablejittercorrection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSCDRIPPER_H__FA093A8F_C0CC_4E3A_82EE_0FF4E175B970__INCLUDED_

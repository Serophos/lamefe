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

#if !defined(AFX_SETTINGSMP3_H__B8FB3CD5_BDF1_4D1A_92F0_6165AE4E6374__INCLUDED_)
#define AFX_SETTINGSMP3_H__B8FB3CD5_BDF1_4D1A_92F0_6165AE4E6374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsMP3.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsMP3 
#include "BladeMP3EncDLL.h"
#include "MySettingsPage.h"
#include "MyComboBox.h"
#include "MyEditCtrl.h"
#include "MFECToolTip.h"

class CSettingsMP3 : public CMySettingsPage
{
// Konstruktion
public:
	int			m_nLayer;
	void SetControls();
	BOOL Save();
	void Init(CString strWd);
	//virtual BOOL PreTranslateMessage( MSG* pMsg );
	CSettingsMP3(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsMP3)
	enum { IDD = IDD_SETTINGS_MP3 };
	CComboBox	c_OutSampleRate;
	CComboBox	m_cThreadPriority;
	CEdit	c_abr;
	CComboBox	c_maxBitrate;
	CComboBox	m_qualityPreset;
	CComboBox	c_vbrQuality;
	CComboBox	c_vbrMethod;
	CComboBox	c_MPEG;
	CButton	c_private;
	CButton	c_original;
	CButton	c_writeId3v1;
	CButton c_writeId3v2;
	CButton	c_copyright;
	CButton	c_checkSum;
	CComboBox	c_channels;
	CComboBox	c_bitsPerSample;
	CString	m_encoderVersion;
	int		m_abr;
	int		m_nMode;
	int		m_nVersion;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsMP3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
//	void LoadPreset(CString strPreset);
//	void InitPresets();
	virtual void FillMaxBitrateTable();
	virtual void FillMinBitrateTable();
	virtual void FillOutSampleRateCtrl();
	virtual int  GetMinBitrate();
	virtual int  GetMaxBitrate();
	virtual void SetMinBitrate(int nBitrate);
	virtual void SetMaxBitrate(int nBitrate);
	CMFECToolTip	m_mToolTip;
	BEVERSION beVersion;

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsMP3)
	afx_msg void OnSelchangeVbrMethod();
	afx_msg void OnSelchangeQuality();
	afx_msg void OnSetfocusThreadpriority();
	afx_msg void OnSelchangePresetmp3dlg();
	afx_msg void OnSelendokMpeg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSMP3_H__B8FB3CD5_BDF1_4D1A_92F0_6165AE4E6374__INCLUDED_

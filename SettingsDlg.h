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

#if !defined(AFX_SETTINGSDLG_H__D15668B1_6255_401D_AF0D_E0F2EAF6F242__INCLUDED_)
#define AFX_SETTINGSDLG_H__D15668B1_6255_401D_AF0D_E0F2EAF6F242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsDlg 

#include "SettingsBatchMode.h"
#include "SettingsCDRipper.h"
#include "SettingsDecoder.h"
#include "SettingsFilenames.h"
#include "SettingsFreeDB.h"
#include "SettingsLameFE.h"
#include "SettingsLookNFeel.h"
#include "SettingsMP3.h"
#include "SettingsPlugin.h"
#include "SettingsLanguages.h"

class CSettingsDlg : public CDialog
{
// Konstruktion
public:
	void ShowCDRipper();
	void ShowDecoders();
	void ShowMP3();
	CSettingsDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_SETTINGSDIALOG };
	CTreeCtrl	m_ctrlModules;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CString m_strLanguage;
	BOOL bShowDecoders;
	BOOL bShowMP3;
	BOOL bShowCDRipper;

	CString m_strWd;
	void CheckForEncoders(HTREEITEM hEncoder);
	void CheckForDecoders(HTREEITEM hDecoder);
	
	// Settings dialog members
	CSettingsBatchMode  m_dBatchmode;
	CSettingsCDRipper	m_dCDRipper;
	CSettingsDecoder	m_dDecoder;
	CSettingsFilenames	m_dFilenames;
	CSettingsFreeDB		m_dFreeDB;
	CSettingsLameFE		m_dLameFE;
	CSettingsLookNFeel	m_dLookNFeel;
	CSettingsMP3		m_dMP3;
	CSettingsPlugin		m_dPlugin;
	CSettingsLanguages	m_dLang;

	//other protected datamembers
	CImageList	m_cImageList;
	CString		m_strActiveDlg;
	CString		m_strDecoder;
	CString		m_strEncoder;
	CString		m_strAlbumInfo;
	CString		m_strBatchMode;
	CString		m_strFilenames;
	CString		m_strLookNFeel;
	CString		m_strPluginPath;

	BOOL InitSubDlgs();
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsDlg)
	afx_msg void OnSelchangedModules(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSDLG_H__D15668B1_6255_401D_AF0D_E0F2EAF6F242__INCLUDED_

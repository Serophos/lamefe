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

#if !defined(AFX_SETTINGSFREEDB_H__C937C37C_0989_44BE_92C3_5F441F203FF7__INCLUDED_)
#define AFX_SETTINGSFREEDB_H__C937C37C_0989_44BE_92C3_5F441F203FF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsFreeDB.h : Header-Datei
//

#include "MySettingsPage.h"
#include "MyEditCtrl.h"
#include "MyComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsFreeDB 

class CSettingsFreeDB : public CMySettingsPage
{
// Konstruktion
public:
	BOOL Save();
	void Init(CString strWd);
	CSettingsFreeDB(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSettingsFreeDB)
	enum { IDD = IDD_SETTINGS_FREEDB };
	CMyEditCtrl	c_port;
	CMyEditCtrl	c_address;
	CMyEditCtrl	c_path;
	CMyEditCtrl	c_location;
	CMyEditCtrl	m_cEMail;
	CMyEditCtrl	m_cTimeOut;
	CButton	c_authentication;
	CMyEditCtrl	c_userName;
	CMyEditCtrl	c_password;
	CMyEditCtrl	c_proxyPort;
	CMyEditCtrl	c_proxyAddress;
	CButton	c_useProxy;
	CMyComboBox	c_remoteServer;
	CString	m_address;
	CString	m_eMail;
	CString	m_location;
	CString	m_passwd;
	CString	m_path;
	CString	m_proxyAddress;
	CString	m_userName;
	BOOL	m_Authentication;
	int		m_timeOut;
	int		m_remotePort;
	int		m_proxyPort;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSettingsFreeDB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSettingsFreeDB)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	afx_msg void OnUseProxy();
	afx_msg void OnSelendokRemoteServer();
	afx_msg void OnUpdate();
	afx_msg void OnAuthentication();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL ReadServerList();
	BOOL WriteServerList();
	//void SaveSettings();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETTINGSFREEDB_H__C937C37C_0989_44BE_92C3_5F441F203FF7__INCLUDED_

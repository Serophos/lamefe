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

#if !defined(AFX_CDDBPAGE_H__D6480494_6C36_4E8F_88F8_76A6792D2499__INCLUDED_)
#define AFX_CDDBPAGE_H__D6480494_6C36_4E8F_88F8_76A6792D2499__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDDBPage.h : Header-Datei
//
#include "MyEditCtrl.h"
#include "MyComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCDDBPage 

class CCDDBPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCDDBPage)

// Konstruktion
public:
	void OnOK();
	virtual BOOL OnInitDialog();
	void init(CString wdir);
	CCDDBPage();
	~CCDDBPage();

// Dialogfelddaten
	//{{AFX_DATA(CCDDBPage)
	enum { IDD = IDD_CDDB_SETUP };
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
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CCDDBPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CCDDBPage)
	afx_msg void OnUseProxy();
	afx_msg void OnSelendokRemoteServer();
	afx_msg void OnUpdate();
	afx_msg void OnAuthentication();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL ReadServerList();
	BOOL WriteServerList();
	void SaveSettings();
	CString wd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_CDDBPAGE_H__D6480494_6C36_4E8F_88F8_76A6792D2499__INCLUDED_

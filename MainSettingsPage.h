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

#if !defined(AFX_MAINSETTINGSPAGE_H__BD36EC0C_4619_4132_9B93_71C9F5D32104__INCLUDED_)
#define AFX_MAINSETTINGSPAGE_H__BD36EC0C_4619_4132_9B93_71C9F5D32104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainSettingsPage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CMainSettingsPage 


class CMainSettingsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMainSettingsPage)

// Konstruktion
public:
	
	void OnHighColorIcons();
	void OnOK();
	virtual BOOL OnInitDialog();
	void init(CString wdir);
	CMainSettingsPage();
	~CMainSettingsPage();

// Dialogfelddaten
	//{{AFX_DATA(CMainSettingsPage)
	enum { IDD = IDD_LAMEFE_PAGE };
	CButton	c_enqueueFiles;
	CButton	c_cdTextRead;
	CButton	c_writeCDPlayerIni;
	CButton	c_readCDPlayerIni;
	CStatic	c_playerPath;
	CButton	c_silent;
	CButton	c_shutdownOnFinished;
	CButton	c_playFiles;
	CButton	c_m3u;
	CButton	c_finishedDialog;
	CButton	c_quitOnFinished;
	CButton	c_beep;
	CButton c_getfocus;
	CButton c_playSound;
	CButton c_saveWinPos;
	CButton c_highColBar;
	CString	m_outputPath;
	CString	m_playerPath;
	CButton c_showSplash;
	CButton c_showAlbumEditor;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMainSettingsPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CMainSettingsPage)
	afx_msg void OnPlayer();
	afx_msg void OnPlayFiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString wd;
	CToolTipCtrl* m_pToolTip;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MAINSETTINGSPAGE_H__BD36EC0C_4619_4132_9B93_71C9F5D32104__INCLUDED_

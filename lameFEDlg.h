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

#if !defined(AFX_LAMEFEDLG_H__43EAA7A7_1B94_4129_A8C3_1321A17CE900__INCLUDED_)
#define AFX_LAMEFEDLG_H__43EAA7A7_1B94_4129_A8C3_1321A17CE900__INCLUDED_

	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lameCmdl.h"
#include "EncodingStatusDlg.h"
#include "InternalPlayer.h"

/////////////////////////////////////////////////////////////////////////////
// CLameFEDlg Dialogfeld

class CLameFEDlg : public CDialog
{
// Konstruktion
public:

	CLameFEDlg(CWnd* pParent = NULL);	// Standard-Konstruktor
	virtual ~CLameFEDlg();

// Dialogfelddaten
	//{{AFX_DATA(CLameFEDlg)
	enum { IDD = IDD_LAMEFE_DIALOG };
	CButton	m_listTitle;
	CButton	m_Settings;
	CButton	m_id3Tag;
	CButton	m_Encode;
	CButton	m_CDdb;
	CButton	c_eject;
	CButton	c_refresh;
	CButton	c_wav2mp3;
	CButton	c_cd2wave;
	CButton	c_cd2mp3;
	CButton	m_next;
	CButton	m_play;
	CButton	m_stop;
	CButton	m_prev;
	CButton	m_pause;
	CListBox	m_ctrlList;
	CString	m_strList;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CLameFEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void readCDContents();
	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CLameFEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNewPreset();
	afx_msg void OnAddFile();
	afx_msg BOOL OnEncode();
	afx_msg void OnExit();
	afx_msg void OnRemoveFile();
	afx_msg void OnSettings();
	afx_msg void OnAbout();
	afx_msg void OnHelp();
	afx_msg void OnToolsEditid3tag();
	afx_msg void OnPlay();
	afx_msg void OnNext();
	afx_msg void OnPause();
	afx_msg void OnPrev();
	afx_msg void OnStop();
	afx_msg void OnCdMp3();
	afx_msg void OnCdWav();
	afx_msg void OnWavMp3();
	afx_msg void OnRefresh();
	afx_msg void OnCddbSettings();
	afx_msg void OnEjectCd();
	afx_msg void OnPreferencesCdreadersettings();
	afx_msg void OnPreferencesTagwritersettings();
	afx_msg void OnCddbReadfromremotecddbserver();
	virtual void OnCancel();
	afx_msg void OnEditId3();
	afx_msg void OnCddb();
	afx_msg void OnPreferencesPlugins();
	afx_msg void OnHelpLicense();
	afx_msg void OnHelpHelpindex();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	InternalPlayer	  *p;
	BOOL			  isEjected;
	CString			  wd;
	CPtrArray		  *files;
	HICON			  m_hIcon;
	EncodingStatusDlg *esdlg;


private:
	void SetActiveCD(int old);
	void SetPlayerStatus(BOOL bEnabled);
	void SetListTitle();
	void SelItems(int iCnt, int *pIndex);

	void SortArray(int *pIndex, int size);
	void ResetFileList();

	BOOL m_bPaused;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_LAMEFEDLG_H__43EAA7A7_1B94_4129_A8C3_1321A17CE900__INCLUDED_)

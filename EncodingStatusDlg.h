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

#if !defined(AFX_ENCODINGSTATUSDLG_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_)
#define AFX_ENCODINGSTATUSDLG_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EncodingStatusDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld EncodingStatusDlg 
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "TrayDialog.h"
#include "BladeMP3EncDLL.h"
#include "TimeStatus.h"
#include "MP3File.h"

#define  RIP_TO_MP3  0
#define  RIP_TO_WAVE 1
#define  WAVE_TO_MP3 2

class EncodingStatusDlg : public CTrayDialog
{
// Konstruktion
public:
	BOOL m_bResetTimer;

	EncodingStatusDlg(CWnd* pParent = NULL);   // Standardkonstruktor

	BOOL RipToMP3();
	BOOL RipToWave();
	BOOL WaveToMP3();

	static UINT encoderFunc(PVOID pParamas);
	void		setJob(int job);
	void		viewLog();
	BOOL		writeID3Tag(MP3File *file);
	CPtrArray*  m_files;
	void		parseFiles(CPtrArray *files, CString wdir, int preset);

// Dialogfelddaten
	//{{AFX_DATA(EncodingStatusDlg)
	enum { IDD = IDD_STATUS_DIALOG };
	CProgressCtrl	m_jitterPos;
	CButton	m_closeBtn;
	CProgressCtrl	m_listStatus;
	CProgressCtrl	m_fileStatus;
	CString	m_estSize;
	CString	m_list_errors;
	CString	m_fileMsg;
	CString	m_fileXofY;
	CString	m_listMsg;
	CString	m_inputSize;
	CString	m_in;
	CString	m_out;
	CString	m_strEstTime;
	CString	m_strElaTime;
	CString	m_strRemTime;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(EncodingStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
private:
	BOOL createDirs(CString filename);
	BOOL createSubDirs(CString basePath, CString artist, CString album);
	BOOL FileExists(CString filename);

	void finishedJobs();
	int GetJob();
	
	int         bufferPerc;
	int         filePerc;
	int			jitterPos;
	BOOL        m_bAbortEnc;
	int         job;
	int         m_preset;
	int         errors;
	CString     wd;
	CMutex		m_mLockControls;
	CWinThread*	m_pThread;
	CEvent		m_eThreadFinished;
	CTimeStatus m_tTimeStatus;


protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(EncodingStatusDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnStRestore();
	afx_msg void OnStExit();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ENCODINGSTATUSDLG_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_

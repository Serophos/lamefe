/*
** Copyright (C) 2002 Thees Winkler
**  
** Parts of this codes are based on code from CDEx (c) 1999-2002 by Albert L. Faber 
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

#if !defined(AFX_CEncodingStatusDlg_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_)
#define AFX_CEncodingStatusDlg_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrayDialog.h"
#include "TimeStatus.h"
#include "CompactDisk.h"	// Hinzugefügt von der Klassenansicht
#include "mmfile.h"
#include "LogFile.h"
#include "ExtListCtrl.h"

#define  RIP_TO_WAVE 0
#define  RIP_TO_MP3  1
#define  ANY_TO_MP3 2

typedef CArray<CMultimediaFile*, CMultimediaFile*>	CMMFArray;

class CEncodingStatusDlg : public CTrayDialog
{
// Konstruktion
public:

	CEncodingStatusDlg(CWnd* pParent = NULL, CString wd = "");   // Standardkonstruktor

	//Encoding Funcs
	static UINT EncoderFunc(PVOID pParamas);

	//Public Class Access
	void	SetFiles(CMMFArray *files);
	void	SetCDROM(CCompactDisc *cd);
	void	SetJob(int nJob, CString strOutputDevice  = "lame_enc.dll", BOOL bAlbumMode = FALSE);
	int		GetJob();
	int		GetAlbumMode(){ return m_bAlbumMode; }
	CImageList	m_cImageList;
	

	void	 OnLogPrint();
	void	 OnLogSave();

// Dialogfelddaten
	//{{AFX_DATA(CEncodingStatusDlg)
	enum { IDD = IDD_STATUS_DIALOG };
	CProgressCtrl	m_jitterPos;
	CButton			m_closeBtn;
	CButton			m_printBtn;
	CButton			m_saveBtn;
	CProgressCtrl	m_listStatus;
	CProgressCtrl	m_fileStatus;
	CString			m_estSize;
	CString			m_list_errors;
	//CString			m_fileMsg;
	//CString			m_fileXofY;
	//CString			m_listMsg;
	CString			m_inputSize;
	CString			m_in;
	CString			m_out;
	CString			m_strEstTime;
	CString			m_strElaTime;
	CString			m_strRemTime;
	CExtListCtrl	m_logOut;
	CString			m_strStatusText;
	CString			m_strDiscardLogMsg;
	//}}AFX_DATA

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CEncodingStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
private:
	// Encoding and file related member functions
	// Before encoding stuff

	// Private Encoding stuff
	BOOL	LameFEPlugin2MP3(CString plugin, CMultimediaFile *mFile, int nPos);
	BOOL	RipToAny();
	BOOL	RipToSingleFile();
	BOOL	AnyToMP3();

	// After encoding stuff
	BOOL	WriteID3Tag(CMultimediaFile *mFile);
	BOOL	WriteID3Tag(MMFILE_ALBUMINFO tmpAI);
	void	FinishedJobs();

	
	// Private Datamembers
	// File list
	CCompactDisc *m_cd;
	CMMFArray*  m_files;
	
	// Job-Details
	int         m_nJob;
	BOOL		m_bAlbumMode;
	CString		m_strInputDevice;
	CString		m_strOutputDevice;
	CString		m_strExtension;
	CString     m_strWd;

	// Status Info
	int         m_nBufferPerc;
	int         m_nFilePerc;
	int			m_nJitterPos;
	int         m_nErrors;
	CTimeStatus m_tTimeStatus;
	BOOL		m_bResetTimer;
	CLogFile	m_lLogFile;

	// Thread stuff	
	BOOL        m_bAbortEnc;
	CMutex		m_mLockControls;
	CEvent		m_eThreadFinished;
	CWinThread*	m_pThread;


protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CEncodingStatusDlg)
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

#endif // AFX_CEncodingStatusDlg_H__952DE9B3_279B_4077_8EAC_30F192CDCE61__INCLUDED_

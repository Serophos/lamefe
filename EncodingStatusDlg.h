/*
** Copyright (C) 2002-2003 Thees Winkler
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
#include "GenericEncoder.h"

#include <queue>
//using namespace std;

typedef CArray<CMultimediaFile*, CMultimediaFile*>	CMMFArray;
//extern enum modes {NORMALMODE = 0, ALBUMMODE, BATCHALBUMMODE, BATCHSINGLETRACKMODE};
enum ENC_MODE{

	LFE_RIP_SINGLE = 0x0004,
	LFE_RIP_ALBUM  = 0x0008,
	LFE_FILE_MODE  = 0x0010,
	LFE_AUTO_MODE  = 0x0001
};

class CEncodingStatusDlg : public CTrayDialog
{
// Konstruktion
public:
	BOOL RipToAny2Phase();
	void Phase2Ripping();
	static UINT EncoderFunc2Phase(PVOID pParamas);
	BOOL Init(CGenericEncoder* pEncoder, CCompactDisc* pCD, ENC_MODE eMode);
	BOOL m_bBatchAppendDiscID;
	BOOL RipBatchMode();

	CEncodingStatusDlg(CWnd* pParent = NULL);   // Standardkonstruktor

	//Encoding Funcs
	static UINT EncoderFunc(PVOID pParamas);

	//Public Class Access
	ENC_MODE	GetMode(){ return m_eMode;}
	//int		GetAlbumMode(){ return m_mEMode; }
	CImageList	m_cImageList;
	

	void	 OnLogSave();

// Dialogfelddaten
	//{{AFX_DATA(CEncodingStatusDlg)
	enum { IDD = IDD_STATUS_DIALOG };
	CProgressCtrl	m_phase2Status;
	CProgressCtrl	m_jitterPos;
	CButton			m_closeBtn;
	CButton			m_saveBtn;
	CProgressCtrl	m_listStatus;
	CProgressCtrl	m_fileStatus;
	CString			m_strEstTime;
	CString			m_strElaTime;
	CString			m_strRemTime;
	CExtListCtrl	m_logOut;
	CString			m_strStatusText;
	CString			m_strDiscardLogMsg;
	CString	m_strPhase2StatusText;
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
	BOOL	WinampPlugin2Encoder(CString plugin, CMultimediaFile *mFile, int nPos);
	BOOL	RipToAny();
	BOOL	RipToSingleFile();
	BOOL	AnyToEncoder();

	// After encoding stuff
	void	FinishedJobs();

	
	// Private Datamembers
	// File list
	CCompactDisc *m_pCD;
//	CMMFArray*  m_files;
	std::queue<CString> m_q2PhaseIn;
	std::queue<CString> m_q2PhaseOut;

	CRITICAL_SECTION m_csQueueLock;
	// Job-Details
	//int         m_nJob;
	ENC_MODE	m_eMode;
	CString		m_strInputDevice;
	CString		m_strOutputDevice;
	CString		m_strExtension;
	CGenericEncoder* m_pEncoder;

	// Status Info
	CString			m_strEstSize;
	CString			m_strInputSize;
	CString			m_strIn;
	CString			m_strOut;

	int         m_nBufferPerc;
	int         m_nFilePerc;
	int			m_nPhase2Perc;
	int			m_nJitterPos;
	int         m_nErrors;
	CTimeStatus m_tTimeStatus;
	BOOL		m_bResetTimer;
	CLogFile	m_lLogFile;

	// Thread stuff	
	BOOL        m_bAbortEnc;
	BOOL		m_bPhase1IsFinished;
	//CSingleLock* m_slLock;
	CMutex		m_mLockControls;
	CEvent		m_eThreadFinished;
	CWinThread*	m_pThread;
	
	CEvent		m_e2PhaseFinished;
	CWinThread* m_p2PhaseThread;


protected:
	void EnableCDControls(BOOL bEnable);

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

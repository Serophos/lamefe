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

#if !defined(AFX_LOGFILE_H__4EA41BE4_5B73_442E_8E15_5214A377C9AD__INCLUDED_)
#define AFX_LOGFILE_H__4EA41BE4_5B73_442E_8E15_5214A377C9AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <mmreg.h>
#include "ExtListCtrl.h"

class CLogFile  
{
public:
	void SetNotificationMessage(CString strMsg);
	void SetNotificationMessage(UINT nID);
	int nLog;
	void SetOutputWnd(CExtListCtrl *listCtrl);
	void SetInFormat(WAVEFORMATEX *wfx);
	BOOL GetState(int nPos);
	void SetWd(CString wd);
	void SetErrorMsg(int nEntry, UINT nID);
	void SetErrorMsg(int nEntry, CString strMessage);
	void FinalizeEntry();
	void StartEntry(CString strInFile, CString strOutFile, CString inModule = "CDRip.dll");
	void FinalizeSession();
	void StartSession(int numEntries, CString outModule);

	BOOL SaveLogAs(LPCSTR strFilename);
	BOOL WriteLog(LPCSTR strFilename);
	BOOL View();

	CLogFile();
	virtual ~CLogFile();

private:
	// Session
	CString		 m_strWd;
	CString		 m_strLogFilename;
	// %d = Date

	time_t		 m_tStartTime;
	time_t		 m_tEndTime;
	CString		 m_strStartTime;
	CString      m_strElapsedTime;
	CString      m_strDate;
	// %M = outMod
	CString		 m_strOutMod;
	int			 m_nNumEntries;
	int			 m_nNumErrors;  // %n
	int			 m_nCurrentEntry;
	// Entries
	time_t		m_tTmpStartTime;
	// %s = sourcefile
	//CStringArray m_staInFiles;
	// %o = outfile
	//CStringArray m_staOutFiles;
	// %f = sourceformat
	//CStringArray m_staInFormat;
	// %F = outformat
	//CStringArray m_staOutFormat;
	// %m = = inMod;
	//CStringArray m_staInModule;
	// %c = compressionratio
	//CStringArray m_staCompression;
	// %t = systemtime
	//CStringArray m_staSystemTime;
	// % T = Time Used
	//CStringArray m_staTimeUsed;
	// %e = errormessages
	CStringArray m_staErrors;
	//
	CExtListCtrl *m_msgOut;
};

#endif // !defined(AFX_LOGFILE_H__4EA41BE4_5B73_442E_8E15_5214A377C9AD__INCLUDED_)

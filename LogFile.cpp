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

#include "stdafx.h"
#include "stdafx.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{

	m_nCurrentEntry		= 0;
	m_nNumEntries		= 0;
	m_nNumErrors		= 0;
	nLog				= 0;
	m_strLogFilename	= "\\lameFE.log";
	m_strOutMod			= "";
	m_strWd				= "";
	m_strStartTime		= "";
	m_strElapsedTime	= "";
}

CLogFile::~CLogFile()
{

}

void CLogFile::StartSession(int numEntries, CString outModule)
{

	time(&m_tStartTime);

	m_nNumEntries	= numEntries;
	m_strOutMod		= outModule;

	struct tm* tLocalTime;
	tLocalTime = localtime(&m_tStartTime);

	CString	strTmp;
	strTmp.Format("%02d:%02d:%02d Starting Session (%02d tasks, %s)", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec, numEntries, outModule);
	
	m_msgOut->InsertItem(nLog, strTmp, 0);
	m_msgOut->EnsureVisible(nLog++, FALSE);
}

void CLogFile::FinalizeSession()
{

	double nElapsedSecs;
	struct tm* tLocalTime;

	time(&m_tEndTime);
	nElapsedSecs = difftime(m_tEndTime, m_tStartTime);

	tLocalTime = localtime(&m_tStartTime);
	m_strStartTime.Format("%02d:%02d:%02d", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec);
	m_strDate.Format("%04d/%02d/%02d", tLocalTime->tm_year + 1900, tLocalTime->tm_mon, tLocalTime->tm_wday);
	m_strElapsedTime.Format("%02.1f", nElapsedSecs / 60);

	tLocalTime = localtime(&m_tEndTime);
	CString strTmp;
	strTmp.Format("%02d:%02d:%02d Session Finished.", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec);
	m_msgOut->InsertItem(nLog, strTmp, 0);
	m_msgOut->EnsureVisible(nLog++, FALSE);

}

BOOL CLogFile::View()
{
	
	BOOL bResult = SaveLogAs(m_strWd + m_strLogFilename);

	if(bResult){

		ShellExecute(0,	"open", "\"" + m_strWd + m_strLogFilename + "\"", 0, 0, SW_SHOW);
	}
	else{

		return FALSE;
	}

	return TRUE;
}

void CLogFile::StartEntry(CString strInFile, CString strOutFile, CString inModule)
{

	time(&m_tTmpStartTime);

/*	m_staInModule.Add(inModule);
	m_staInFiles.Add(strInFile);
	m_staOutFiles.Add(strOutFile);*/
	m_staErrors.Add("");

	struct tm* tLocalTime;
	tLocalTime = localtime(&m_tTmpStartTime);

	CString	strTmp;
	strTmp.Format("%02d:%02d:%02d Starting File (%02d/%02d): %s (Module %s)", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec, m_nCurrentEntry + 1, m_nNumEntries, strInFile, inModule);
	
	m_msgOut->InsertItem(nLog++, strTmp, 0);
	m_msgOut->InsertItem(nLog, "Save as: " + strOutFile, 2);
	m_msgOut->EnsureVisible(nLog++, FALSE);
	m_msgOut->SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CLogFile::SetInFormat(WAVEFORMATEX *wfx)
{

	CString strTmp = "";
	strTmp.Format("%d Hz %d bit %d channels", wfx->nSamplesPerSec,
							wfx->wBitsPerSample,
							wfx->nChannels
				  );
//	m_staInFormat.Add(strTmp);

	strTmp.Format("File has format %d kHz %d bit %d channels", wfx->nSamplesPerSec, wfx->wBitsPerSample, wfx->nChannels);
	m_msgOut->InsertItem(nLog, strTmp, (wfx->wBitsPerSample == 16 ? 0 : 1));
	m_msgOut->EnsureVisible(nLog++, FALSE);
}

void CLogFile::FinalizeEntry()
{

	time_t		tTmpEndTime;
	struct tm*	tLocalTime;
	double		nElapsedSecs;
	CString		strTmp;

	time(&tTmpEndTime);
	nElapsedSecs = difftime(tTmpEndTime, m_tTmpStartTime);
	tLocalTime = localtime(&m_tTmpStartTime);

	strTmp.Format("%02d:%02d:%02d", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec);
/*	m_staSystemTime.Add(strTmp);
	strTmp.Format("%02.1f", nElapsedSecs / 60);
	m_staTimeUsed.Add(strTmp);

	double nFileSizeIn = 0;
	double nFileSizeOut = 0;
	double nCompressionRatio = 0;
	CFileStatus fStatus;

	CFile::GetStatus(m_staInFiles.GetAt(m_nCurrentEntry), fStatus);
	nFileSizeIn  = fStatus.m_size;
	CFile::GetStatus(m_staOutFiles.GetAt(m_nCurrentEntry), fStatus);
	nFileSizeOut = fStatus.m_size;
	nCompressionRatio = nFileSizeOut / (nFileSizeIn != 0 ? nFileSizeIn : 1) * 100;
	
	strTmp.Format("%03d%%", (int)nCompressionRatio);
	m_staCompression.Add(strTmp);
*/
	tLocalTime = localtime(&tTmpEndTime);
	strTmp.Format("%02d:%02d:%02d File Finished.", tLocalTime->tm_hour, tLocalTime->tm_min, tLocalTime->tm_sec);
	m_msgOut->InsertItem(nLog, strTmp, 0);
	m_msgOut->EnsureVisible(nLog++, FALSE);

	m_nCurrentEntry++;
}

void CLogFile::SetErrorMsg(int nEntry, CString strMessage)
{

	m_staErrors.SetAt(nEntry, strMessage);
	m_nNumErrors++;
	m_msgOut->InsertItem(nLog, strMessage, (strMessage == "Encoded successfully" ? 0 : 1));
	m_msgOut->EnsureVisible(nLog++, FALSE);
}

void CLogFile::SetErrorMsg(int nEntry, UINT nID)
{

	CString strTmp;
	strTmp.LoadString(nID);
	SetErrorMsg(nEntry, strTmp);
}

void CLogFile::SetNotificationMessage(CString strMsg)
{

	m_msgOut->InsertItem(nLog, strMsg, 3);
	m_msgOut->EnsureVisible(nLog++, FALSE);
}

void CLogFile::SetNotificationMessage(UINT nID)
{

	CString strTmp;
	strTmp.LoadString(nID);
	SetNotificationMessage(strTmp);
}

void CLogFile::SetWd(CString wd)
{

	m_strWd = wd;
}

BOOL CLogFile::GetState(int nPos)
{

	return m_staErrors.GetAt(nPos).IsEmpty();
}


void CLogFile::SetOutputWnd(CExtListCtrl *listCtrl)
{
	
	m_msgOut	= listCtrl;
}



BOOL CLogFile::WriteLog(LPCSTR strFilename)
{

	CStdioFile		fLogFile;
	BOOL			bResult;

	TRY{

		bResult = fLogFile.Open(
						strFilename, 
					    CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);

		if(bResult == 0){
			
			TRACE("Error Creating / Opening logfile\n");
			return FALSE;
		}
		
		fLogFile.SeekToEnd();
		
		for(int i = 0; i < m_msgOut->GetItemCount(); i++){

			fLogFile.WriteString(m_msgOut->GetItemText(i, 0) + "\n");
		}
		
		fLogFile.WriteString("--------------------------------------------------\n\n");
		fLogFile.Flush();
		fLogFile.Close();

	}
	CATCH(CFileException, e){
	
		return FALSE;
	}
	END_CATCH;

	return TRUE;
}

BOOL CLogFile::SaveLogAs(LPCSTR strFilename)
{

	return WriteLog(strFilename);
}



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
#include "lameFE.h"
#include "CDdbQueryDlg.h"
#include "mfccddb.h"
#include "cfgFile.h"
#include "ID3Info.h"
#include "MP3File.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACKSPERSEC 75
#define CB_CDDASECTOR 2352


int cddb_sum(int n)
{
	char	buf[12],*p;
	int	ret = 0;

	// For backward compatibility this algorithm must not change
	sprintf(buf, "%lu", n);
	for (p = buf; *p != '\0'; p++)
		ret += (*p - '0');

	return (ret);
}

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCDdbQueryDlg 


CCDdbQueryDlg::CCDdbQueryDlg(CWnd* pParent /*=NULL*/, CPtrArray *files, int activeCD, CString wdir)
	: CDialog(CCDdbQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCDdbQueryDlg)
	//}}AFX_DATA_INIT
	m_pFiles = files;
	nActiveCD = activeCD;
	wd = wdir;
}


void CCDdbQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDdbQueryDlg)
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_PROTOCOLL, m_Protocoll);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCDdbQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CCDdbQueryDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_LBN_DBLCLK(IDC_PROTOCOLL, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CCDdbQueryDlg 

void CCDdbQueryDlg::OnCancel() 
{

	CDialog::OnCancel();
}

BOOL CCDdbQueryDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	//Initialise the Winsock stack
    if (!AfxSocketInit()){

		MessageBox("Error initializing Winsock!");
		CDialog::OnCancel();
	}

	CString msg;
	cfgFile cfg(wd);

	discID = CalculateDiscID();
	

    if(cfg.GetValue(PROXY, FALSE)){

		cddb.SetProxyDetails(
				    cfg.GetStringValue(PROXYADDR), 
					cfg.GetValue(PROXYPORT, FALSE), 
					(cfg.GetValue(AUTH, FALSE) ? cfg.GetStringValue(USERNAME) : ""),
					(cfg.GetValue(AUTH, FALSE) ? cfg.GetStringValue(PASSWORD) : "")
				);
	}


	CString serverString = GetServerString(cfg.GetValue(CDDBSERVER, FALSE));

	CString sBuf;
	int iLen = serverString.GetLength();
	int iPosB = 0;

	//Address
	int iPosE = serverString.Find(',');
	site.m_sSite = serverString.Left(iPosE);
	iPosB = iPosE + 2;
	
	//Port
	iPosE = serverString.Find(',', iPosB);
	sBuf = serverString.Mid(iPosB, iPosE - iPosB);
	site.m_nPort = atoi(sBuf.GetBuffer(10));
	sBuf.ReleaseBuffer();
	iPosB = iPosE + 2;

	//Path
	iPosE = serverString.Find(',', iPosB);
	site.m_sAddress = serverString.Mid(iPosB, iPosE - iPosB);
	iPosB = iPosE + 2;

	CCDDBStatus status;
	BOOL bSuccess = cddb.Status(site, status);
	if (!bSuccess){

#ifdef _DEBUG
		TRACE(_T("Failed in call to get status\n"));
		TRACE(_T("Last Response :\n"));
		OutputDebugString(cddb.GetLastCommandResponse());
		TRACE(_T("\n"));
#endif
		MessageBox("Server isn't responding.", "lameFE CDdb(tm) Query Error");
		CDialog::OnCancel();
	}

	CArray<CCDDBTrackPosition, CCDDBTrackPosition&> tracks;
	
	bSuccess = GetTrackPositions(tracks);
	if (!bSuccess){
		
		MessageBox("cddb.GetTrackPositions(...) failed");
		CDialog::OnCancel();
	}

	
	bSuccess = cddb.Query(site, discID, tracks, results);
	if (bSuccess)
	{

		for (int i = 0; i < results.GetSize(); i++)
		{
		
			CCDDBQueryResult& result = results.ElementAt(i);
			msg.Format("%s / %s (ID: %08x)", result.m_sArtist, result.m_sTitle, result.m_dwDiscID);
			AddLine(msg);
		}
	}
	else
	{
#ifdef _DEBUG
		TRACE(_T("Failed in call to Query\n"));
		TRACE(_T("Last Response :\n"));
		TRACE(cddb.GetLastCommandResponse());
		TRACE(_T("\n"));
#endif
		CDialog::OnCancel();
	}
	
	m_Protocoll.SetCurSel(0);
	
	if(m_Protocoll.GetCount() == 1){

		QueryCDDB();
	}

	return TRUE;
}

BOOL CCDdbQueryDlg::QueryCDDB()
{


	BOOL bSuccess;
	CWaitCursor wc;

	int iSel = m_Protocoll.GetCurSel();
	if (iSel < 0 || iSel > results.GetSize()){
		
		MessageBox("Wählen Sie einen Eintrag aus dewr Liste!");
		return FALSE;
	}

	CCDDBQueryResult& result = results.ElementAt(iSel);
	
	m_Start.EnableWindow(FALSE);
	m_Protocoll.ResetContent();
	AddLine("Starting CDddb(tm) query. Stand by...");

	CCDDBRecord record;
	bSuccess = cddb.Read(site, result.m_dwDiscID, result.m_sCategory, record);
	if (!bSuccess){
		
		AddLine("Failed. Server is not responding.");
		m_Cancel.EnableWindow(TRUE);
		return FALSE;
	}
	
	CString tmp;

	for (int i = 0; i < m_pFiles->GetSize(); i++){

		((MP3File*)m_pFiles->GetAt(i))->getID3Info()->setAlbum(record.m_sTitle);
		((MP3File*)m_pFiles->GetAt(i))->getID3Info()->setArtist(record.m_sArtist);
		((MP3File*)m_pFiles->GetAt(i))->getID3Info()->setTrack(i + 1);
		((MP3File*)m_pFiles->GetAt(i))->getID3Info()->setSong(record.m_TrackTitles.ElementAt(i));
		tmp.Format("Updating information for Track%d", i + 1);
		AddLine(tmp);
	}

	AddLine("Finished Successfully");
	m_Protocoll.SetScrollPos(SB_VERT, m_Protocoll.GetScrollLimit(SB_VERT), TRUE);

#ifdef _DEBUG
	TRACE(_T("\nRetrieved the following album details\n"));
	TRACE(_T("Album Title: %s\n"), record.m_sTitle);
	TRACE(_T("Album Artist: %s\n"), record.m_sArtist);
	TRACE(_T("Disc Length (seconds): %d\n"), record.m_nDiskLength);
	TRACE(_T("Submitted via: %s %s %s\n"), record.m_sClientName, record.m_sClientVersion, record.m_sClientComments);
	TRACE(_T("Database Revision: %d\n"), record.m_nDatabaseRevision);
	TRACE(_T("DISCID: %x\n"), record.m_dwDiscID);

	TRACE(_T("\nExtended Track Data:\n"));
	for (i=0; i<record.m_ExtendedTrackData.GetSize(); i++)
		TRACE(_T("Track %d: %s\n"), i+1, record.m_ExtendedTrackData.ElementAt(i));

	TRACE(_T("\nExtended Data: %s\n"), record.m_ExtendedData);
#endif

	m_Cancel.EnableWindow(TRUE);

	if(bSuccess){

		OnCancel();
	}
	return TRUE;
}

void CCDdbQueryDlg::AddLine(CString msg)
{

	m_Protocoll.AddString(msg);
}

void CCDdbQueryDlg::OnStart() 
{

	QueryCDDB();
}


CString CCDdbQueryDlg::GetServerString(int i)
{

	CStdioFile		 server;
	CFileException	 e;
	CString			 serverString;
	CStringArray	 serverArray;

	TRY{

		server.Open(wd + "\\cddb.cfg", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeText, &e);

		while(server.GetPosition() != server.GetLength()){

			server.ReadString(serverString);
			serverArray.Add(serverString);
		}
		server.Close();
	}
	CATCH(CFileException, e){

		MessageBox("Error reading cddb.cfg. Goto CDDB Settings and set up servers first!");
		CDialog::OnCancel();
	}
	END_CATCH;

	return serverArray[i];
}

int CCDdbQueryDlg::CalculateDiscID()
{

	DWORD dwRet;
	DWORD t = 0;
	DWORD n = 0;
	DWORD	dwTwoSecOffset=0;

	// For backward compatibility this algorithm must not change
	dwTwoSecOffset = 2 * TRACKSPERSEC;

	for (int i = 0; i < m_pFiles->GetSize(); i++) 
	{
		
		//TRACE("i=%d, size=%d\n", i, m_pFiles->GetSize());
		// Keep in mind the two seconds offset
		DWORD dwSectors = ((MP3File*)m_pFiles->GetAt(i))->GetStartSector() + dwTwoSecOffset;

		n += cddb_sum(dwSectors/TRACKSPERSEC);

		// Keep in mind the two seconds offset
		DWORD dwSectorsNext = ((MP3File*)m_pFiles->GetAt(i))->GetEndSector() + dwTwoSecOffset;
		t += (dwSectorsNext/TRACKSPERSEC-dwSectors/TRACKSPERSEC);
	}

	dwRet=( (n % 0xff) << 24 | t << 8 | (DWORD)(m_pFiles->GetSize()));

	// Get total playing time
	//m_dwTotalSecs=(GetEndSector(m_nNumTracks)+1+150)/75;
	
	return dwRet;
}

BOOL CCDdbQueryDlg::GetTrackPositions(CArray<CCDDBTrackPosition, CCDDBTrackPosition&>& tracks){

	for (int i = 0; i < m_pFiles->GetSize(); i++){

		tracks.Add(((MP3File *)m_pFiles->GetAt(i))->getTrackDuration());
		//TRACE("%d:%d:%d -- %d\n", tracks[i].m_nMinute, tracks[i].m_nSecond, tracks[i].m_nFrame, ((MP3File *)m_pFiles->GetAt(i))->GetStartSector() );
	}

	//LeadOutPosition
	DWORD dwPos = ((MP3File *)m_pFiles->GetAt(i - 1))->GetEndSector() + 150;
	
	CCDDBTrackPosition trackPosition;
	trackPosition.m_nFrame = dwPos % 75;
	dwPos /= 75;
	trackPosition.m_nSecond = dwPos % 60;
	dwPos /= 60;
	trackPosition.m_nMinute = dwPos;
	
	tracks.Add(trackPosition);


	return TRUE;
}
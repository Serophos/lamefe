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
#include "Settings.h"
#include "ID3Info.h"
#include "Utils.h"
#include "FreeDBStatusDlg.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACKSPERSEC 75
#define CB_CDDASECTOR 2352

extern CSettings g_sSettings;
extern CI18n	 g_iLang;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCDdbQueryDlg 


CCDdbQueryDlg::CCDdbQueryDlg(CWnd* pParent /*=NULL*/, CCompactDisc *cd, int activeCD, BOOL bAutoSelect /* = FALSE */)
	: CDialog(CCDdbQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCDdbQueryDlg)
	//}}AFX_DATA_INIT
	m_cd          = cd;
	m_nActiveCD	  = activeCD;
	m_bAutoSelect = bAutoSelect;
	m_dwDiscID	  = 0;

}


void CCDdbQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDdbQueryDlg)
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_PROTOCOLL, m_Protocoll);
	DDX_Control(pDX, ID_CLOSE, m_Cancel);
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

	CString strMsg = "";

	g_iLang.TranslateDialog(this, IDD_CDDB_QUERY);

	FreeDBStatusDlg statusDlg;
	
	statusDlg.ShowWindow(SW_SHOWNORMAL);
	statusDlg.SetMessage(IDS_FDB_INITWINSOCK);
	
	//Initialise the Winsock stack
    if (!AfxSocketInit()){

		statusDlg.ShowWindow(FALSE);

		ErrMessageBox(IDS_CDDB_ERR_WINSOCK, MB_OK+MB_ICONEXCLAMATION);
		CDialog::OnCancel();
		return FALSE;
	}


	m_dwDiscID = m_cd->GetDiscID();
	

    if(g_sSettings.GetUseProxy()){

		statusDlg.SetMessage(IDS_FDB_PROXYCONNECT);
		cddb.SetProxyDetails(
				    g_sSettings.GetProxyAdress(), 
					g_sSettings.GetProxyPort(), 
					(g_sSettings.GetProxyAuthentication() ? g_sSettings.GetUsername() : ""),
					(g_sSettings.GetProxyAuthentication() ? Utils::DecryptString(g_sSettings.GetPassowrd()) : "")
				);
	}


	statusDlg.SetMessage(IDS_FDB_SERVERCONNECT);

	CString serverString = GetServerString(g_sSettings.GetFreeDBServer());

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
	site.m_nPort = atoi(sBuf);
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
		statusDlg.ShowWindow(FALSE);
		ErrMessageBox(IDS_ERR_NOSERVERRESPONSE, MB_OK+MB_ICONSTOP);
		CDialog::OnCancel();
		return FALSE;
	}

	CArray<CCDDBTrackPosition, CCDDBTrackPosition&> tracks;
	
	bSuccess = GetTrackPositions(tracks);
	if (!bSuccess){
		
		statusDlg.ShowWindow(FALSE);
		ErrMessageBox(IDS_ERR_GETTRACKPOSFAILED, MB_OK+MB_ICONEXCLAMATION);
		CDialog::OnCancel();
		return FALSE;
	}

	statusDlg.SetMessage(IDS_FDB_GETENTRIES);
	bSuccess = cddb.Query(site, m_dwDiscID, tracks, results);
	statusDlg.ShowWindow(FALSE);
	if (bSuccess)
	{

		for (int i = 0; i < results.GetSize(); i++)
		{
		
			CCDDBQueryResult& result = results.ElementAt(i);
			
			strMsg.Format("%s / %s;\tGenre: %s\t(Disc-ID: %08x)", result.m_sArtist, result.m_sTitle, result.m_sCategory, result.m_dwDiscID);
			AddLine(strMsg);
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
		CString out;
		out.Format(g_iLang.GetString(IDS_FDB_NOENTRY), m_dwDiscID);
		ErrMessageBox(out, MB_OK+MB_ICONINFORMATION );
		CDialog::OnCancel();
		return FALSE;
	}
	
	
	m_Protocoll.SetCurSel(0);
	
	if(m_Protocoll.GetCount() == 1 || m_bAutoSelect){

		QueryCDDB();
	}

	return TRUE;
}

BOOL CCDdbQueryDlg::QueryCDDB()
{


	BOOL bSuccess;
	CWaitCursor wc;

	FreeDBStatusDlg statusDlg;
	//statusDlg.Create(IDD_FREEDB_STATUS, NULL);
	statusDlg.ShowWindow(SW_SHOWNORMAL);
	
	int iSel = m_Protocoll.GetCurSel();

	if (iSel < 0 || iSel > results.GetSize()){
		
		ErrMessageBox(IDS_FDB_QUERYSTART, MB_OK+MB_ICONINFORMATION);
		return FALSE;
	}
	
	statusDlg.SetMessage(IDS_FDB_SELECTENTRY); 
	m_Protocoll.ResetContent();

	CCDDBQueryResult& result = results.ElementAt(iSel);
	
	m_Start.EnableWindow(FALSE);
	
	CCDDBRecord record;
	bSuccess = cddb.Read(site, result.m_dwDiscID, result.m_sCategory, record);
	if (!bSuccess){
		
		statusDlg.ShowWindow(FALSE);
		ErrMessageBox(IDS_ERR_NOSERVERRESPONSE, MB_OK+MB_ICONEXCLAMATION);
		m_Cancel.EnableWindow(TRUE);
		return FALSE;
	}
	
	CString tmp;
	statusDlg.SetMessage(IDS_FDB_TRACKUPDATE);
	for (int i = 0; i < m_cd->GetNumTracks(); i++){

		m_cd->GetCDTrack(i)->m_id3Info.SetAlbum(record.m_sTitle);
		m_cd->GetCDTrack(i)->m_id3Info.SetArtist(record.m_sArtist);
		m_cd->GetCDTrack(i)->m_id3Info.SetTrack(i + 1);
		m_cd->GetCDTrack(i)->m_id3Info.SetSong(record.m_TrackTitles.ElementAt(i));
		m_cd->GetCDTrack(i)->m_id3Info.SetGenre(result.m_sCategory);

		//tmp.Format("Updating information for Track%d", i + 1);
		//AddLine(tmp);
	}

	statusDlg.ShowWindow(FALSE);
	//AddLine("Finished Successfully");
	//m_Protocoll.SetScrollPos(SB_VERT, m_Protocoll.GetScrollLimit(SB_VERT), TRUE);

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

		OnOK();
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

		server.Open(g_sSettings.GetAppDir() + "\\cddb.cfg", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeText, &e);

		while(server.GetPosition() != server.GetLength()){

			server.ReadString(serverString);
			serverArray.Add(serverString);
		}
		server.Close();
	}
	CATCH(CFileException, e){

		ErrMessageBox(IDS_CDDB_ERR_CFGREAD, MB_OK+MB_ICONEXCLAMATION);
		CDialog::OnCancel();
	}
	END_CATCH;

	return serverArray[i];
}


BOOL CCDdbQueryDlg::GetTrackPositions(CArray<CCDDBTrackPosition, CCDDBTrackPosition&>& tracks){

	for (int i = 0; i < m_cd->GetNumTracks(); i++){

		CCDDBTrackPosition trackPos;

		DWORD dwSector = m_cd->GetCDTrack(i)->m_dwStartSector + 150;

		trackPos.m_nMinute	= (dwSector / (75 * 60));
		trackPos.m_nSecond	= (dwSector / 75) % 60;
		trackPos.m_nFrame	= (dwSector % 75);
		
		tracks.Add(trackPos);

		TRACE("%d:%d:%d -- %d\n", tracks[i].m_nMinute, tracks[i].m_nSecond, tracks[i].m_nFrame, m_cd->GetCDTrack(i)->m_dwStartSector );
	}

	//LeadOutPosition
	DWORD dwPos = m_cd->GetCDTrack(i)->m_dwStartSector + 150;
	
	CCDDBTrackPosition trackPosition;
	trackPosition.m_nFrame = dwPos % 75;
	dwPos /= 75;
	trackPosition.m_nSecond = dwPos % 60;
	dwPos /= 60;
	trackPosition.m_nMinute = dwPos;
	
	tracks.Add(trackPosition);


	return TRUE;
}

void CCDdbQueryDlg::ErrMessageBox(UINT nID, UINT nType)
{

	if(!m_bAutoSelect){

		AfxMessageBox(g_iLang.GetString(nID), nType);
	}
}

void CCDdbQueryDlg::ErrMessageBox(CString strMsg, UINT nType)
{

	if(!m_bAutoSelect){

		AfxMessageBox(strMsg, nType);
	}
}


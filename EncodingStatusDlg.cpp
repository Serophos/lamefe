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


#include "stdafx.h"
#include "lameFE.h"
#include "TimeStatus.h"
#include "EncodingStatusDlg.h"
#include "Encoder.h"
#include "EncoderFeeder.h"
#include "ID3Lib/include/id3.h"
#include "ID3Lib/include/id3/misc_support.h"
#include "plugin_api.h"
#include "LameFEPlugin.h"
#include "CDRip/CDRip.h"
#include "OutPlugin.h"
#include "Playlist.h"
#include "cfgFile.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define TIMERID    2


/////////////////////////////////////////////////////////////////////////////
// "dialog"feld CEncodingStatusDlg 


CEncodingStatusDlg::CEncodingStatusDlg(CWnd* pParent /*=NULL*/, CString wd)
	: CTrayDialog(CEncodingStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncodingStatusDlg)
	m_estSize = _T("");
	//m_list_errors = _T("");
	////m_fileMsg = _T("");
	////m_fileXofY = _T("");
	////m_listMsg = _T("");
	m_inputSize = _T("");
	m_in = _T("");
	m_out = _T("");
	m_strEstTime = _T("");
	m_strElaTime = _T("");
	m_strRemTime = _T("");
	//}}AFX_DATA_INIT
	m_strStatusText = "Status";
	m_nErrors		= 0;
	m_bAbortEnc		= FALSE;
	m_files			= 0;
	m_cd			= 0;
	m_strWd			= wd;
	m_nBufferPerc	= 0;
	m_nFilePerc		= 0;
	m_nJitterPos	= 0;
	m_nErrors		= 0;
	m_bResetTimer	= TRUE;
	m_strInputDevice = "CDRip.dll";
	m_strDiscardLogMsg = "";
	m_lLogFile.SetWd(wd);
}


void CEncodingStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncodingStatusDlg)
	DDX_Control(pDX, IDC_JITTERSTATUS, m_jitterPos);
	DDX_Control(pDX, IDCANCEL, m_closeBtn);
	DDX_Control(pDX, IDC_PRINT, m_printBtn);
	DDX_Control(pDX, IDC_SAVE_LOG, m_saveBtn);
	DDX_Control(pDX, IDC_LIST_STATUS_BAR, m_listStatus);
	DDX_Control(pDX, IDC_FILE_STATUS_BAR, m_fileStatus);
	DDX_Control(pDX, IDC_LOG, m_logOut);
	DDX_Text(pDX, IDC_EST_SIZE, m_estSize);
	DDX_Text(pDX, IDC_IN_SIZE, m_inputSize);
	DDX_Text(pDX, IDC_IN, m_in);
	DDX_Text(pDX, IDC_OUT, m_out);
	DDX_Text(pDX, IDC_EST_TIME, m_strEstTime);
	DDX_Text(pDX, IDC_ELA_TIME, m_strElaTime);
	DDX_Text(pDX, IDC_REM_TIME, m_strRemTime);
	DDX_Text(pDX, IDC_PRINTMSG, m_strDiscardLogMsg);
	DDX_Text(pDX, IDC_STATUS_STRING, m_strStatusText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncodingStatusDlg, CTrayDialog)
	//{{AFX_MSG_MAP(CEncodingStatusDlg)
	ON_WM_TIMER()
	ON_COMMAND(IDC_ST_RESTORE, OnStRestore)
	ON_COMMAND(IDC_ST_EXIT, OnStExit)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(IDC_PRINT, OnLogPrint)
	ON_COMMAND(IDC_SAVE_LOG, OnLogSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEncodingStatusDlg 

BOOL CEncodingStatusDlg::OnInitDialog()
{

	CTrayDialog::OnInitDialog();

	SetTimer(TIMERID, 1000, NULL);
	
	m_pThread = NULL;
	
	m_eThreadFinished.ResetEvent();
	// Fire of thread
	m_pThread = AfxBeginThread(EncoderFunc, (void*)this);
	
	TraySetIcon(IDR_MAINFRAME);
	TraySetToolTip("lameFE");
	TraySetMenu(IDR_SYSTRAY_MENU);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	SetFocus();
	SetForegroundWindow();

	m_logOut.InsertColumn(0, "Status Messages", LVCFMT_RIGHT, 400);
	m_logOut.Init(FALSE);
	// Create imaglist and attach it to the tracklist
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 3, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_LOG_IMAGES);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();

	m_logOut.SetImageList(&m_cImageList, LVSIL_SMALL);
	
	m_lLogFile.SetOutputWnd(&m_logOut);
	
	m_printBtn.EnableWindow(FALSE);
	m_saveBtn.EnableWindow(FALSE);
	
	return TRUE;
}

void CEncodingStatusDlg::OnTimer(UINT nIDEvent)
{

	m_mLockControls.Lock();
	
	m_fileStatus.SetPos(m_nFilePerc);
	m_listStatus.SetPos(m_nBufferPerc);
	m_strStatusText.Format("Status (%02d%%)", m_nFilePerc);

	if(m_nJob != ANY_TO_MP3){

		m_jitterPos.SetPos(m_nJitterPos);
	}

	if(m_bResetTimer){

		m_tTimeStatus.ReInit();
		m_bResetTimer = FALSE;
	}

	//Calcualte estTime
	m_tTimeStatus.Calculate(m_nFilePerc);
	m_strEstTime = m_tTimeStatus.GetEstimateString();
	m_strElaTime = m_tTimeStatus.GetElapsedString();
	m_strRemTime = m_tTimeStatus.GetRemainingString();

	UpdateData(FALSE);
	m_mLockControls.Unlock();

	CTrayDialog::OnTimer(nIDEvent);
}

void CEncodingStatusDlg::OnCancel() 
{
		
	TRACE0("Entering CEncodingStatusDialog::OnCancel()\n");

	m_bAbortEnc = TRUE;
	
	TRACE(">>> Waiting for Encoding-Thread finished Event...\n");
	WaitForSingleObject(m_eThreadFinished, INFINITE);

	m_logOut.InsertItem(m_logOut.GetItemCount(), "Encoding Process aborted by user.", 1);
	m_logOut.EnsureVisible(m_logOut.GetItemCount() - 1, FALSE);	
	CR_LockCD(FALSE);
	KillTimer(TIMERID);
	MessageBeep(MB_ICONEXCLAMATION);

	TRACE0("## Leaving CEncodingStatusDialog::OnCancel()\n");
	CTrayDialog::OnCancel();
}

void CEncodingStatusDlg::OnStRestore() 
{
	TRACE("OnStRestore()\n");
	ShowWindow(SW_SHOW);
	SetFocus();
	SetForegroundWindow( );

}

void CEncodingStatusDlg::OnStExit() 
{
	
	TRACE("OnStExit()\n");
	OnStRestore();
	OnCancel();
}

//Init funcs
void CEncodingStatusDlg::SetFiles(CMMFArray *files)
{

	m_files = files;
}

void CEncodingStatusDlg::SetCDROM(CCompactDisc *cd)
{

	m_cd	= cd;
}


//encoding Functions

BOOL CEncodingStatusDlg::WriteID3Tag(CMultimediaFile *mFile)
{

	MMFILE_ALBUMINFO	tmpAI;

	tmpAI.album		= mFile->m_id3Info.GetAlbum();
	tmpAI.artist	= mFile->m_id3Info.GetArtist();
	tmpAI.comment	= mFile->m_id3Info.GetComment();
	tmpAI.genre		= mFile->m_id3Info.GetGenre();
	tmpAI.song		= mFile->m_id3Info.GetSong();
	tmpAI.track		= mFile->m_id3Info.GetTrack();
	tmpAI.year		= mFile->m_id3Info.GetYear();

	return WriteID3Tag(tmpAI);
}

BOOL CEncodingStatusDlg::WriteID3Tag(MMFILE_ALBUMINFO tmpAI)
{

	PostMessage(WM_TIMER,0,0);

	cfgFile cfg(m_strWd);


	ID3_Tag		id3Tag;
	ID3_Frame	id3Frame;
	ID3_Frame	*buffer = NULL;

	id3Tag.Link(m_out, (cfg.GetValue("id3v1") ? ID3TT_ID3V1 : 0) | (cfg.GetValue("id3v2") ? ID3TT_ID3V2 : 0));

	if(cfg.GetValue("id3v2") || cfg.GetValue("id3v1")){

		
		if(tmpAI.song != "" && tmpAI.song != 0){

			buffer = id3Tag.Find(ID3FID_TITLE);
			if(buffer){
				// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
			id3Frame.SetID(ID3FID_TITLE);
			id3Frame.Field(ID3FN_TEXT).Set(tmpAI.song);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		//artist
		if(tmpAI.artist != "" && tmpAI.artist != 0){

			buffer = id3Tag.Find(ID3FID_LEADARTIST);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
			id3Frame.SetID(ID3FID_LEADARTIST);
			id3Frame.Field(ID3FN_TEXT).Set(tmpAI.artist);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		//album
		if(tmpAI.album != "" && tmpAI.album != 0){

			buffer = id3Tag.Find(ID3FID_ALBUM);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
			id3Frame.SetID(ID3FID_ALBUM);
			id3Frame.Field(ID3FN_TEXT).Set(tmpAI.album);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		//comment
		if(tmpAI.comment != "" && tmpAI.comment != 0){

			buffer = id3Tag.Find(ID3FID_COMMENT);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
			id3Frame.SetID(ID3FID_COMMENT);
			id3Frame.Field(ID3FN_TEXT).Set(tmpAI.comment);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		//year
		if(tmpAI.year != 0){

			buffer = id3Tag.Find(ID3FID_YEAR);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
		 	char year;
			itoa(tmpAI.year, &year, 10);

			id3Frame.SetID(ID3FID_YEAR);
			id3Frame.Field(ID3FN_TEXT).Set(year);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		//track
		if(tmpAI.track != 0){

			buffer = id3Tag.Find(ID3FID_TRACKNUM);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}

			char track;
 			itoa(tmpAI.track, &track, 10);

			id3Frame.SetID(ID3FID_TRACKNUM);
			id3Frame.Field(ID3FN_TEXT).Set(track);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

		if(tmpAI.genre != "" && tmpAI.genre != 0){

			buffer = id3Tag.Find(ID3FID_CONTENTTYPE);
			if(buffer){// Remove if already exists
				id3Tag.RemoveFrame(buffer);
			}
			id3Frame.SetID(ID3FID_CONTENTTYPE);
			id3Frame.Field(ID3FN_TEXT).Set(tmpAI.genre);
			id3Tag.AddFrame(&id3Frame);
			id3Tag.Update();
		}

	}
	
	if(cfg.GetValue("writetlen")){


	/*	LPCSTR ms;

		

		buffer = id3Tag.Find(ID3FID_SONGLEN);
		if(buffer){// Remove if already exists

			id3Tag.RemoveFrame(buffer);
		}

		id3Frame.SetID(ID3FID_SONGLEN);
		id3Frame.Field(ID3FN_TEXT).Set(ms);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();*/
	}

	PostMessage(WM_TIMER,0,0);
 
	return TRUE;
}

void CEncodingStatusDlg::SetJob(int nJob, CString strOutputDevice /* = "lame_enc.dll" */, BOOL bAlbumMode /* = FALSE */)
{
	
	m_nJob			  = nJob;
	m_strOutputDevice = strOutputDevice;
	m_bAlbumMode	  = bAlbumMode;

	if(m_strOutputDevice == "lame_enc.dll"){
		
		m_strExtension = ".mp3";
	}
	else{

		COutPlugin tmp(m_strWd + "\\Plugins\\" + m_strOutputDevice, m_strWd + "\\lameFE.ini");
		tmp.Load();
		m_strExtension = tmp.GetExtension();
		tmp.Unload();
	}

}

UINT CEncodingStatusDlg::EncoderFunc(PVOID pParams)
{
	
	CEncodingStatusDlg *esdlg = (CEncodingStatusDlg *)pParams;
	
	switch(esdlg->GetJob()){

	case RIP_TO_MP3:
	case RIP_TO_WAVE:
		if(esdlg->GetAlbumMode()){

			esdlg->RipToSingleFile();
		}
		else{
			
			esdlg->RipToAny();
		}
		break;
	case ANY_TO_MP3:
		esdlg->AnyToMP3();
		break;
	}

	esdlg->FinishedJobs();

	// Indicate thread status
	TRACE("## Thread is finished. Set Event!!\n");
	esdlg->m_eThreadFinished.SetEvent();
	esdlg->m_pThread = NULL;

	
	TRACE("------- Encoder Thread (EncodingStatusDialog::encoderFunc) is terminated\n ------------");
	return 0;
}

BOOL CEncodingStatusDlg::AnyToMP3()
{

	CString	  strPlugin = "";
	cfgFile   cfg(m_strWd);
	
	//setup controls
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	m_nErrors = 0;
	PostMessage(WM_TIMER,0,0);
	
	m_nJitterPos = 0;
	m_nBufferPerc = 100;
	
	m_lLogFile.StartSession(m_files->GetSize(), m_strOutputDevice);

	for(int i = 0; i < m_files->GetSize() &&  !m_bAbortEnc; i++){
		
		m_nFilePerc = 0;
		m_bResetTimer = TRUE;
		CMultimediaFile *mFile = m_files->GetAt(i);

		TRACE("Checking for existing file\n");
		m_in  = mFile->GetFileName();
		m_out = mFile->GetSaveAs(m_strWd, m_strExtension);

		m_lLogFile.StartEntry(m_in, m_out, m_strInputDevice);
		if(Utils::FileExists(m_out) && !cfg.GetValue("silent")){

			CString out;
			out.Format(IDS_ENC_FILEEXISTS, m_out);
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				m_lLogFile.SetErrorMsg(i, IDS_ENC_DENIEDOVWR);
				m_nErrors++;
				continue;
			}
		}
		
		TRACE("Creating non existing directories\n");
		if(!Utils::CreateDirs(m_out)){
				
			AfxMessageBox(IDS_ENC_ERRDIRCREATE, MB_ICONSTOP+MB_OK);
			m_lLogFile.SetErrorMsg(i, IDS_ENC_ERRDIRCREATE);
			m_nErrors++;
			continue;

		}

		if(CLameFEPlugin::FindPlugin(mFile->GetFileName().Right(3), strPlugin, m_strWd + "\\Plugins")){
		
			m_strInputDevice = strPlugin;
			LameFEPlugin2MP3(strPlugin, mFile, i);
			if(m_bAbortEnc){

				return FALSE;
			}
		}
		else{

			TRACE("No plugin found to handle this file\n");
			m_lLogFile.SetErrorMsg(i, IDS_ENC_NO_PLUGIN);
			mFile->GetWfx()->wBitsPerSample = 0;
			mFile->GetWfx()->nSamplesPerSec = 0;
			mFile->GetWfx()->nChannels		= 0;
			m_nErrors++;
		}
		
		m_lLogFile.FinalizeEntry();
	}	// end for

	m_lLogFile.FinalizeSession();
	
	return TRUE;
}


BOOL CEncodingStatusDlg::LameFEPlugin2MP3(CString plugin, CMultimediaFile *mFile, int nPos)
{


	PLF_IN    inModule	= NULL;
	HINSTANCE hDLL		= NULL;
	GETMODULE GetModule	= NULL;

	CFileStatus rStatus;
	CFile::GetStatus( mFile->GetFileName(), rStatus );
	float in_size = rStatus.m_size;
	float in_sizeKB = rStatus.m_size;
	cfgFile   cfg(m_strWd);

	hDLL = LoadLibrary(plugin);
	if(!hDLL){

		TRACE("Failed Loading Library\n");
		m_list_errors.Format(IDS_ENC_PLUG_LOADFAILED, plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	
	GetModule = (GETMODULE)GetProcAddress(hDLL, "GetLFINModule");
	if(!GetModule){

		TRACE("Failed getting proc address\n");
		m_list_errors.Format(IDS_ENC_PLUG_APIERROR, plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;

		return FALSE;
	}

	inModule = GetModule();
	if(!inModule){

		TRACE("Failed getting module\n");
		m_list_errors.Format(IDS_ENC_PLUG_APIERROR, plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;

		return FALSE;
	}
	
	inModule->hDllInstance = hDLL;
	inModule->hMainWindow  = NULL;

	inModule->Init();
	if(inModule->Open(mFile->GetFileName()) != 0){

		TRACE("Failed opening file\n");
		m_list_errors.Format(IDS_FILEOPENERR, mFile->GetFileName());
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}

	// Check for existing file

	TRACE("Setting up Encoder\n");

	PSHORT	pWAVBuffer = NULL;
	DWORD   dwRead     = 0;
	DWORD   dwDone	   = 0;

	CEncoder e(m_strWd);

	MMFILE_ALBUMINFO tmpAI;
	tmpAI.album   = mFile->m_id3Info.GetAlbum();
	tmpAI.artist  = mFile->m_id3Info.GetArtist();
	tmpAI.comment = mFile->m_id3Info.GetComment();
	tmpAI.genre   = mFile->m_id3Info.GetGenre();
	tmpAI.song    = mFile->m_id3Info.GetSong();
	tmpAI.track   = mFile->m_id3Info.GetTrack();
	tmpAI.year    = mFile->m_id3Info.GetYear();
	e.SetAlbumInfo(tmpAI);

	e.SetOutputFormat(m_strOutputDevice);
	e.Init();
	
	MMFILE_FORMAT mmFormat;
	inModule->GetMMFileFormat(&mmFormat);
	
	mFile->GetWfx()->wBitsPerSample = mmFormat.nBitsPerSample;
	mFile->GetWfx()->nSamplesPerSec = mmFormat.dwSampleRate;
	mFile->GetWfx()->nChannels		= mmFormat.nChannels;
	
	m_lLogFile.SetInFormat(mFile->GetWfx());

	if(mmFormat.nBitsPerSample != 16){

		TRACE("Invalid bitrate\n");
		m_list_errors.Format(IDS_INVBITRATE, mmFormat.nBitsPerSample);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}

	if(!e.PrepareEncoding(mFile->GetSaveAs(m_strWd, m_strExtension), mmFormat.nBitsPerSample, mmFormat.dwSampleRate, mmFormat.nBitsPerSample)){
		
		TRACE("Error preparing encoder\n");
		m_list_errors.LoadString(IDS_ENC_INITERR);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	
	//Setting up Controls
	m_mLockControls.Lock();
	CString strToolTip = "lameFE  | Working on file " + mFile->GetFileName();
	if(strToolTip.GetLength() > 63){


		strToolTip = strToolTip.Left(60);
		strToolTip += "...";
	}
	TraySetToolTip(strToolTip);

	if(TrayIsVisible()){

		TrayUpdate();
	}
	m_mLockControls.Unlock();


	double mp3_est_size = e.GetEstimatedSize(mmFormat.dwSampleRate, mmFormat.nBitsPerSample, mmFormat.nBitsPerSample, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_inputSize.Format("%2.2f", in_size);
	m_estSize.Format("%2.2f", mp3_est_size);

	//Check if there's enough free disk space
	double nFreeDiskSpace = 0;
	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(cfg.GetStringValue("output"));
	
	if((mp3_est_size / 1024 )>= nFreeDiskSpace){

		m_list_errors.Format
			(
			"Not enoug free disk space on drive %s. (Required space=%d, Free space%d (KB)).\nAborting Encoding process",
			cfg.GetStringValue("output").Left(2),
			m_estSize, nFreeDiskSpace
			);

		m_nErrors++;
		AfxMessageBox(m_list_errors, MB_OK+MB_ICONSTOP);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		e.DeInit();
		return FALSE;
	}
	
	DWORD sampleToRead = e.GetSamplesToRead();
	pWAVBuffer = new SHORT[sampleToRead];
	
	while (((dwRead = inModule->Read(pWAVBuffer, sampleToRead)) > 0) && !m_bAbortEnc)
	{

		e.PassBuffer((LPVOID)pWAVBuffer, dwRead);

		dwDone += dwRead * sizeof(SHORT);
		m_nFilePerc = (float)dwDone / (float)in_sizeKB * 100;
		
		
	} //File is done:-)



	e.DeInit();

	inModule->Close();

	FreeLibrary(hDLL);

	delete pWAVBuffer;

	if(m_bAbortEnc){
		
		TRACE("Encoding aborted by user\n");
		return FALSE;
	}
	
	
	if((cfg.GetValue("id3v1") || cfg.GetValue("id3v2")) && m_strOutputDevice == "lame_enc.dll"){
	
		if(!WriteID3Tag(mFile)){

			TRACE("Error writing id3Tag\n");
			m_lLogFile.SetErrorMsg(nPos, IDS_ENC_ID3FAILED);
			m_nErrors++;
		}
	}
	if(m_lLogFile.GetState(nPos)){

		m_lLogFile.SetErrorMsg(nPos, IDS_ENC_SUCCESS);
	}

	return TRUE;
}

BOOL CEncodingStatusDlg::RipToSingleFile(){

	TRACE("RipToSingleFile: Outputdevice is %s", m_strOutputDevice);
	cfgFile cfg(m_strWd);

	//setup controls
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	m_nErrors = 0;
	PostMessage(WM_TIMER,0,0);
	
	LONG     nBufferSize  = 0;
	BOOL     bAbort       = FALSE;
	int*	 pSel		  = NULL;
	int	 	 numTracks    = m_cd->GetNumSelTracks();

	pSel	= new int[numTracks];
	m_cd->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_in.Format("Audio CD %s", m_cd->GetStrVolID());
	m_out = m_cd->GetAlbumString(m_strWd, m_strExtension);

	m_lLogFile.StartSession(1, m_strOutputDevice);
	m_lLogFile.StartEntry(m_in, m_out);
	
	WAVEFORMATEX wfx;
	wfx.nChannels	    = 2;
	wfx.nSamplesPerSec  = 44100;
	wfx.wBitsPerSample	= 16;
	m_lLogFile.SetInFormat(&wfx);

	// Check for existing file
	TRACE("Checking for existing file\n");

	if(Utils::FileExists(m_out) && !cfg.GetValue("silent")){

		CString out;
		out.Format(IDS_ENC_FILEEXISTS, m_out);
		if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

			m_lLogFile.SetErrorMsg(0, IDS_ENC_DENIEDOVWR);
			m_nErrors++;
			return FALSE;
		}
	}
	
	TRACE("Creating non existing directories\n");
	if(!Utils::CreateDirs(m_out)){
			
		AfxMessageBox(IDS_ENC_ERRDIRCREATE, MB_ICONSTOP+MB_OK);
		m_lLogFile.SetErrorMsg(0, IDS_ENC_ERRDIRCREATE);
		m_nErrors++;
		return FALSE;

	}

	TRACE("Setting up Encoder\n");

	CEncoder e(m_strWd);

	MMFILE_ALBUMINFO tmpAI;
	tmpAI.album   = m_cd->GetCDTrack(0)->m_id3Info.GetAlbum();
	tmpAI.artist  = m_cd->GetCDTrack(0)->m_id3Info.GetArtist();
	tmpAI.comment = m_cd->GetCDTrack(0)->m_id3Info.GetComment();
	tmpAI.genre   = m_cd->GetCDTrack(0)->m_id3Info.GetGenre();
	tmpAI.song    = "Complete Album";
	tmpAI.track   = 0;
	tmpAI.year    = m_cd->GetCDTrack(0)->m_id3Info.GetYear();

	e.SetAlbumInfo(tmpAI);
	
	e.SetOutputFormat(m_strOutputDevice);
	e.Init();
	e.PrepareEncoding(m_out, 2, 44100, 16);

	unsigned long dwSampleBufferSize = e.GetSamplesToRead();
	unsigned long dwStartSector		 = m_cd->GetCDTrack(0)->m_dwStartSector;
	unsigned long dwEndSector        = m_cd->GetEndSector(m_cd->GetLastAudioTrack()+1);

	//Setting up Controls
	m_mLockControls.Lock();
	
	TraySetToolTip("lameFE | Working on CD Backup");
	TrayUpdate();

	float  in_size	    = (dwEndSector - dwStartSector) * 2352;
	double mp3_est_size = e.GetEstimatedSize(44100, 2, 16, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_inputSize.Format("%2.2f", in_size);
	m_estSize.Format("%2.2f", mp3_est_size);

	//Check if there's enough free disk space
	double nFreeDiskSpace = 0;

	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(cfg.GetStringValue("output")) / (1024.0*1024.0);

	PostMessage(WM_TIMER,0,0);

	if((mp3_est_size)>= nFreeDiskSpace){

		m_mLockControls.Lock();
		m_list_errors.Format("Not enoug free disk space on drive %s).\nAborting Encoding process", cfg.GetStringValue("output").Left(2));
		m_lLogFile.SetErrorMsg(0, m_list_errors);
		AfxMessageBox(m_list_errors, MB_OK+MB_ICONSTOP);
		m_nErrors++;
		m_mLockControls.Unlock();
		return FALSE;
	}
	m_mLockControls.Unlock();

	m_nFilePerc = 0;
	m_bResetTimer = TRUE;

	if (CR_OpenRipper(&nBufferSize, dwStartSector, dwEndSector) == CDEX_OK){
		
		LONG nNumBytesRead = 0;
		LONG nOffset=0;
		
		BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * sizeof(SHORT)];

		CDEX_ERR ripErr;
		CEncoderFeeder ef(&e, dwSampleBufferSize, 512, m_bAbortEnc);
		
		// Read all chunks
		while((ripErr=CR_RipChunk(pbtStream + nOffset, &nNumBytesRead, m_bAbortEnc)) != CDEX_RIPPING_DONE  && !m_bAbortEnc){
			
			SHORT*	psEncodeStream = (SHORT*)pbtStream;
			DWORD	dwSamplesToConvert = (nNumBytesRead + nOffset) / sizeof(SHORT); /// 2;

			//Check for jitter m_nErrors
			if (CDEX_JITTER_ERROR == ripErr)
			{
				DWORD dwStartSector, dwEndSector;

				// Get info where jitter error did occur
				CR_GetLastJitterErrorPosition(dwStartSector,dwEndSector);
				CString jErr;
				jErr.Format(IDS_ENC_JITTER_ERR, dwStartSector, dwEndSector);
				m_lLogFile.SetErrorMsg(0, jErr);
				m_nErrors++;
				PostMessage(WM_TIMER,0,0);
			}

			if (CDEX_ERROR == ripErr){
				
				m_nErrors++;
			
				m_list_errors.Format(IDS_ENC_NUMERRORS, m_nErrors);
				m_lLogFile.SetErrorMsg(0, m_list_errors);
				PostMessage(WM_TIMER,0,0);
				break;
			}
			
			
			while (dwSamplesToConvert >= dwSampleBufferSize){

				if(!ef.AddData(psEncodeStream, dwSampleBufferSize)){
				
					return FALSE; //this is fatal! Buffer error
				}

				dwSamplesToConvert -= dwSampleBufferSize;

				psEncodeStream += dwSampleBufferSize;
			}

			// Copy the remaing bytes up front, if necessary
			if (dwSamplesToConvert > 0){

				// Calculate the offset in bytes
				nOffset = dwSamplesToConvert * sizeof(SHORT);

				// Copy up front
				memcpy(pbtStream, psEncodeStream, nOffset);
			}
			else{
				nOffset=0;
			}


			m_nFilePerc   = CR_GetPercentCompleted();
			m_nJitterPos = CR_GetJitterPosition();
			m_nBufferPerc = ef.GetBufferStatus();
		}//end while Ripp

		//some bytes left in the buffer?
		if (nOffset && !m_bAbortEnc)
		{

			ef.AddData((SHORT*)pbtStream, nOffset / sizeof(SHORT));
		}

		m_nBufferPerc = ef.GetBufferStatus();

		ef.WaitForFinished();
		e.DeInit();

		delete pbtStream;
		
		CR_CloseRipper();

		if(m_bAbortEnc){

			TRACE("Encoding process aborted by user\n");
			return FALSE;
		}

		if((cfg.GetValue("id3v1") || cfg.GetValue("id3v2")) && m_strOutputDevice == "lame_enc.dll"){
	
			if(!WriteID3Tag(tmpAI)){

				m_lLogFile.SetErrorMsg(0, IDS_ENC_ID3FAILED);
				m_nErrors++;
			}

		}
		if(cfg.GetValue("writecue")){

			m_cd->WriteCueSheet(m_strWd, m_strExtension);
		}
	}
	
	m_lLogFile.FinalizeEntry();
	m_lLogFile.FinalizeSession();

	delete pSel;

	if(cfg.GetValue("lock", FALSE)){
		
		CR_LockCD(FALSE);
	}
	
	if(cfg.GetValue("eject", FALSE)){

		CR_EjectCD(TRUE);
	}
	return TRUE;
}		


BOOL CEncodingStatusDlg::RipToAny()
{
	
	TRACE("RipToAny: Mode is %d (0 = Output Wave 1 = Output MP3) Output = %s\n", m_nJob, m_strOutputDevice);
	cfgFile cfg(m_strWd);
	
	if(cfg.GetValue("lock", FALSE)){

		CR_LockCD(TRUE);
	}

	//setup controls
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	m_nErrors = 0;
	PostMessage(WM_TIMER,0,0);
	
	int      currentTrack = 0;
	LONG     nBufferSize  = 0;
	BOOL     bAbort       = FALSE;
	CCDTrack *cdTrack	  = NULL;
	int*	 pSel		  = NULL;
	int	 	 numTracks    = m_cd->GetNumSelTracks();

	pSel	= new int[numTracks];
	m_cd->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_lLogFile.StartSession(numTracks, "CDRip.dll");

	while(currentTrack < numTracks && !m_bAbortEnc){
	
		cdTrack = m_cd->GetCDTrack(pSel[currentTrack]);
		m_out.Format(m_cd->GetSaveAs(cdTrack->m_btTrack, m_strWd, m_strExtension));
	
		if(!cdTrack->IsAudioTrack()){
			
			TRACE("Shouldn't have happened :(\n");
			continue;
		}

		// Check for existing file
		TRACE("Checking for existing file\n");
		m_lLogFile.StartEntry(cdTrack->GetTrackname(), m_out, m_strOutputDevice);
		if(Utils::FileExists(m_out) && !cfg.GetValue("silent")){

			CString out;
			out.Format(IDS_ENC_FILEEXISTS, m_out);
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_DENIEDOVWR);
				currentTrack++;
				m_nErrors++;
				continue;
			}
		}
		
		TRACE("Creating non existing directories\n");
		if(!Utils::CreateDirs(m_out)){
				
			AfxMessageBox(IDS_ENC_ERRDIRCREATE, MB_ICONSTOP+MB_OK);
			m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_ERRDIRCREATE);
			m_nErrors++;
			currentTrack++;
			continue;

		}

		TRACE("Setting up Encoder\n");

		CEncoder e(m_strWd);

		MMFILE_ALBUMINFO tmpAI;
		tmpAI.album   = cdTrack->m_id3Info.GetAlbum();
		tmpAI.artist  = cdTrack->m_id3Info.GetArtist();
		tmpAI.comment = cdTrack->m_id3Info.GetComment();
		tmpAI.genre   = cdTrack->m_id3Info.GetGenre();
		tmpAI.song    = cdTrack->m_id3Info.GetSong();
		tmpAI.track   = cdTrack->m_id3Info.GetTrack();
		tmpAI.year    = cdTrack->m_id3Info.GetYear();
		e.SetAlbumInfo(tmpAI);
		
		e.SetOutputFormat(m_strOutputDevice);
		e.Init();
		e.PrepareEncoding(m_out, 2, 44100, 16);
		unsigned long dwSampleBufferSize = e.GetSamplesToRead();
		
		WAVEFORMATEX wfx;
		wfx.nChannels	    = 2;
		wfx.nSamplesPerSec  = 44100;
		wfx.wBitsPerSample	= 16;
		m_lLogFile.SetInFormat(&wfx);
		
		//Setting up Controls
		m_mLockControls.Lock();
		
		TraySetToolTip("lameFE | Working on file " + cdTrack->GetTrackname());
		TrayUpdate();

		float  in_size	    = (m_cd->GetCDTrack(cdTrack->m_btTrack)->m_dwStartSector - cdTrack->m_dwStartSector) * 2352;
		double mp3_est_size = e.GetEstimatedSize(44100, 2, 16, in_size);

		in_size		 /= (1024.0*1024.0);	//in MB
		mp3_est_size /= (1024.0*1024.0);	//in MB

		m_inputSize.Format("%2.2f", in_size);
		m_estSize.Format("%2.2f", mp3_est_size);
		
		//Check if there's enough free disk space
		double nFreeDiskSpace = 0;
		nFreeDiskSpace = Utils::GetMyFreeDiskSpace(cfg.GetStringValue("output")) / (1024.0*1024.0);
		PostMessage(WM_TIMER,0,0);
		if((mp3_est_size)>= nFreeDiskSpace){

			m_mLockControls.Lock();
			m_list_errors.Format("Not enoug free disk space on drive %s).\nAborting Encoding process",
				cfg.GetStringValue("output").Left(2));
			m_nErrors++;
			m_lLogFile.SetErrorMsg(currentTrack, m_list_errors);
			AfxMessageBox(m_list_errors, MB_OK+MB_ICONSTOP);
			m_mLockControls.Unlock();
			return FALSE;
		}

		m_in.Format(cdTrack->GetTrackname());

		m_nFilePerc = 0;
		m_bResetTimer = TRUE;
		m_mLockControls.Unlock();
		PostMessage(WM_TIMER,0,0);
		
		//end setting up controls

		if (CR_OpenRipper(
					&nBufferSize, 
					cdTrack->m_dwStartSector,
					m_cd->GetEndSector(pSel[currentTrack] + 1)
				 ) == CDEX_OK
			){
			
			LONG nNumBytesRead = 0;
			LONG nOffset=0;
			
			BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * sizeof(SHORT)];

			CDEX_ERR ripErr;
			CEncoderFeeder ef(&e, dwSampleBufferSize, 512, m_bAbortEnc);
			
			// Read all chunks
			while((ripErr=CR_RipChunk(pbtStream + nOffset, &nNumBytesRead, m_bAbortEnc)) != CDEX_RIPPING_DONE  && !m_bAbortEnc){
				
				SHORT*	psEncodeStream = (SHORT*)pbtStream;
				DWORD	dwSamplesToConvert = (nNumBytesRead + nOffset) / sizeof(SHORT); /// 2;

				//Check for jitter m_nErrors
				if (CDEX_JITTER_ERROR == ripErr)
				{
					DWORD dwStartSector, dwEndSector;

					// Get info where jitter error did occur
					CR_GetLastJitterErrorPosition(dwStartSector,dwEndSector);
					CString jErr;
					jErr.Format(IDS_ENC_JITTER_ERR, dwStartSector, dwEndSector);
					m_lLogFile.SetErrorMsg(currentTrack, jErr);
					m_nErrors++;
					PostMessage(WM_TIMER,0,0);
				}

				if (CDEX_ERROR == ripErr){
					
					currentTrack++;
					m_nErrors++;
					m_lLogFile.SetErrorMsg(currentTrack, "Error ripping audiotrack");
					
					m_list_errors.Format(IDS_ENC_NUMERRORS, m_nErrors);
					PostMessage(WM_TIMER,0,0);
					break;
				}
				
				
				while (dwSamplesToConvert >= dwSampleBufferSize){

					if(!ef.AddData(psEncodeStream, dwSampleBufferSize)){
					
						return FALSE; //this is fatal! Buffer error
					}

					dwSamplesToConvert -= dwSampleBufferSize;

					psEncodeStream += dwSampleBufferSize;
				}

				// Copy the remaing bytes up front, if necessary
				if (dwSamplesToConvert > 0){

					// Calculate the offset in bytes
					nOffset = dwSamplesToConvert * sizeof(SHORT);

					// Copy up front
					memcpy(pbtStream, psEncodeStream, nOffset);
				}
				else{
					nOffset=0;
				}


				m_nFilePerc   = CR_GetPercentCompleted();
				m_nJitterPos = CR_GetJitterPosition();
				m_nBufferPerc = ef.GetBufferStatus();
			}//end while Ripp

			//some bytes left in the buffer?
			if (nOffset && !m_bAbortEnc)
			{

				ef.AddData((SHORT*)pbtStream, nOffset / sizeof(SHORT));
			}

			m_nBufferPerc = ef.GetBufferStatus();

			ef.WaitForFinished();
			e.DeInit();
			
			delete pbtStream;
			
			CR_CloseRipper();
			
			if(m_bAbortEnc){

				TRACE("Encoding process aborted by user\n");
				return FALSE;
			}

			if((cfg.GetValue("id3v1") || cfg.GetValue("id3v2")) && m_strOutputDevice == "lame_enc.dll"){
		
				if(!WriteID3Tag(tmpAI)){

					m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_ID3FAILED);
					m_nErrors++;
				}
			}

			if(m_lLogFile.GetState(currentTrack)){

				m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_SUCCESS);
			}

			m_lLogFile.FinalizeEntry();

			currentTrack++;
		} //end for file
		else{

			return FALSE;
		}
		
	}
	
	delete pSel;
	m_lLogFile.FinalizeSession();

	if(cfg.GetValue("lock", FALSE)){
		
		CR_LockCD(FALSE);
	}
	
	if(cfg.GetValue("eject", FALSE)){

		CR_EjectCD(TRUE);
	}
	
	return TRUE;
}

int CEncodingStatusDlg::GetJob()
{

	return m_nJob;
}


void CEncodingStatusDlg::FinishedJobs()
{

	TRACE0("Entering FinishedJobs()\n");

	if(m_bAbortEnc){

		TRACE0("Don't run finished jobs. Encoding aborted!\n");
		return;
	}

	//Whe are done, reset Controls

	if(TrayIsVisible()){
		
		OnStRestore();
	}
	
	KillTimer(TIMERID);

	cfgFile lcfg(m_strWd);
	CString tmp;

	if(lcfg.GetValue("quit", FALSE) || lcfg.GetValue("shutdown", FALSE)){
		
		CTrayDialog::EndDialog (IDOK);
		return;
	}

	CString text;
	text.LoadString(IDS_ENC_CLOSE_BTN);
	m_closeBtn.SetWindowText(text);
	m_printBtn.EnableWindow(TRUE);
	m_saveBtn.EnableWindow(TRUE);
	m_strDiscardLogMsg.LoadString(IDS_ENC_DISCARD_LOG);
	m_estSize.Empty();
	m_inputSize.Empty();
	m_in.Empty();
	m_out.Empty();
	m_nFilePerc = 0;
	m_nBufferPerc = 0;
	m_nJitterPos = 0;
	PostMessage(WM_TIMER, 0, 0);
	

	TRACE("Starting finished Jobs\n");
	if(m_nErrors != 0){

		int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG_ERR, MB_YESNO + MB_ICONQUESTION);
		
		if(nResult == IDYES){
			
			m_lLogFile.View();
		}
	}
	else{

		//check what to be done after encoding

		if (lcfg.GetValue("beep")){  //"beep"

			
			Beep(1000,800);
		}

		if(lcfg.GetValue("playsound")){

			MessageBeep( MB_ICONSTOP );
		}
		
		if (lcfg.GetValue("getfocus")){

			OnStRestore();
		}

		if((lcfg.GetValue("dialog", FALSE)) == TRUE){ //show finished "dialog"
			

			int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG, MB_YESNO + MB_ICONQUESTION);
			
			if(nResult == IDYES){

				m_lLogFile.View();
			}
		}				
		
		if ((lcfg.GetValue("m3u", FALSE)) == TRUE){  //create "m3u"-""play""list with encoded files

			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_cd){

				playlist = new CPlayList(m_cd);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(m_strWd, m_strExtension, FALSE, m_bAlbumMode)){

			}
			else{

			}

			//delete playlist;
			PostMessage(WM_TIMER,0,0);
		}		

		if ((lcfg.GetValue("play", FALSE)) == TRUE){ //play encoded files



			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_cd){

				playlist = new CPlayList(m_cd);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(m_strWd, m_strExtension, TRUE,  m_bAlbumMode)){

			}
			else{

			}

			//delete playlist;
			tmp = lcfg.GetStringValue("extplayer");

			if(tmp != "[System default]"){  // use specified player

				if(lcfg.GetValue("enqueue")){

					tmp += " /ADD ";
				}

				WinExec(tmp + " \"" + m_strWd + "\\lameFE.m3u\"", SW_SHOW);
			}
			else{

				ShellExecute(GetSafeHwnd(), 
								"open", 
								"\"" + m_strWd + "\\lameFE.m3u\"",
								NULL,
								NULL,
								SW_SHOW
						);
			}

							
		}
	}

	PostMessage(WM_TIMER, 0, 0);
}


//DEL BOOL CEncodingStatusDlg::FileExists(CString filename)
//DEL {
//DEL 
//DEL 	TRY{
//DEL 		
//DEL 		CFileStatus rStatus;
//DEL 		if(CFile::GetStatus( filename, rStatus )){
//DEL 
//DEL 			return TRUE;
//DEL 		}
//DEL 		else{
//DEL 
//DEL 			return FALSE;
//DEL 		}
//DEL 	}
//DEL 	CATCH(CFileException, e){
//DEL 
//DEL 		return FALSE;
//DEL 	}
//DEL 	END_CATCH;
//DEL 
//DEL 	return FALSE;
//DEL }

//DEL BOOL CEncodingStatusDlg::CreateSubDirs(CString basePath, CString artist, CString album)
//DEL {
//DEL 
//DEL 	if(_chdir(basePath) != 0){
//DEL 
//DEL 		return FALSE;
//DEL 	}
//DEL 
//DEL 	if(_chdir(artist) != 0){
//DEL 
//DEL 		
//DEL 		if(mkdir(artist) != 0){
//DEL 
//DEL 			return FALSE;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	if(_chdir(basePath + "\\" + artist) != 0){
//DEL 
//DEL 		return FALSE;
//DEL 	}
//DEL 
//DEL 	if(_chdir(album) != 0){
//DEL 
//DEL 		
//DEL 		if(mkdir(album) != 0){
//DEL 
//DEL 			return FALSE;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }

//DEL BOOL CEncodingStatusDlg::CreateDirs(CString filename)
//DEL {
//DEL 
//DEL 	int nStart = 3;
//DEL 	int nEnd = 0;
//DEL 	
//DEL 	CString validated = filename.Left(2);
//DEL 	_chdir(validated);
//DEL 	
//DEL 	while(TRUE){
//DEL 
//DEL 		nEnd = filename.Find('\\', nStart);
//DEL 		if(nEnd == -1){
//DEL 
//DEL 			return TRUE;
//DEL 		}
//DEL 
//DEL 		if(_chdir(validated + "\\" + filename.Mid(nStart, nEnd - nStart)) != 0){
//DEL 
//DEL 			_chdir(validated);
//DEL 			if(_mkdir(filename.Mid(nStart, nEnd - nStart)) != 0){
//DEL 
//DEL 				return FALSE;
//DEL 			}
//DEL 			
//DEL 		}
//DEL 		validated += "\\" + filename.Mid(nStart, nEnd - nStart);
//DEL 		nStart = nEnd + 1;		
//DEL 	}
//DEL 
//DEL 	return FALSE;
//DEL }


//DEL DOUBLE CEncodingStatusDlg::GetMyFreeDiskSpace(CString& strPath)
//DEL {
//DEL 
//DEL 	CString strRootDir;
//DEL 	GFDPEX		GetDiskFreeSpaceOSR2=NULL;
//DEL 
//DEL 	// Check input parameter
//DEL 	if (strPath.IsEmpty())
//DEL 	{
//DEL 		ASSERT(FALSE);
//DEL 		return 0;
//DEL 	}
//DEL 
//DEL 	// Copy driverletter, colon and back slash
//DEL 	strRootDir=strPath.Left(3);
//DEL 
//DEL 	DOUBLE dFreeDiskSpace=0.0;
//DEL 
//DEL 	// Is this NT or OSR2, then call extented free disk space routine
//DEL 	if (GetDiskFreeSpaceOSR2!=NULL)
//DEL 	{
//DEL 		ULARGE_INTEGER	FreeBytes; 
//DEL 		ULARGE_INTEGER	TotalNumberOfBytes;
//DEL 		ULARGE_INTEGER	TotalNumberOfFreeBytes;
//DEL 
//DEL 		// Make call to kernel 32 dll
//DEL 		BOOL bReturn=GetDiskFreeSpaceOSR2(strRootDir,&FreeBytes,&TotalNumberOfBytes,&TotalNumberOfFreeBytes);
//DEL 
//DEL 		// If result is OK, then calculate result
//DEL 		if (bReturn)
//DEL 		{
//DEL 			dFreeDiskSpace=(DOUBLE)FreeBytes.LowPart;
//DEL 			dFreeDiskSpace+=((DOUBLE)FreeBytes.HighPart)*ULONG_MAX;
//DEL 		}
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// Standard Windows 95
//DEL  		DWORD dwSectorsPerCluster; 
//DEL 		DWORD dwBytesPerSector; 
//DEL 		DWORD dwNumberOfFreeClusters; 
//DEL 		DWORD dwTotalNumberOfClusters;
//DEL 
//DEL 		// Try the good old GetDiskFreeSpace
//DEL 		GetDiskFreeSpace(strRootDir,&dwSectorsPerCluster,&dwBytesPerSector,&dwNumberOfFreeClusters,&dwTotalNumberOfClusters);
//DEL  
//DEL 		// Calculate free disk space
//DEL 		dFreeDiskSpace=(DOUBLE)dwNumberOfFreeClusters*(DOUBLE)dwBytesPerSector*(DOUBLE)dwSectorsPerCluster;
//DEL 
//DEL 	// Get the free disc space as you already do.
//DEL 	}
//DEL 	return dFreeDiskSpace;
//DEL }

void CEncodingStatusDlg::OnLogSave()
{

	cfgFile cfg;

	CFileDialog dlg(FALSE, 0, 0, OFN_OVERWRITEPROMPT, "Textfile (*.txt) |*.txt|", 0);
	
	//dlg.m_ofn.lpstrInitialDir = (const char*)cfg.GetStringValue("output");
	if(dlg.DoModal() == IDOK){

		if(m_lLogFile.SaveLogAs(dlg.GetPathName() + ".txt")){

			CString strTmp;
			strTmp.Format(IDS_LOGSAVEDSUCCESS, dlg.GetPathName() + ".txt");
			AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
		}
	}
}

void CEncodingStatusDlg::OnLogPrint()
{

	AfxMessageBox("Not implemented, yet");
}

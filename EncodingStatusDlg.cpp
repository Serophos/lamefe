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

#include "stdafx.h"
#include <memory>
#include <direct.h>
#include "lameFE.h"
#include "TimeStatus.h"
#include "EncodingStatusDlg.h"
#include "Encoder.h"
#include "EncoderFeeder.h"
#include "ID3Lib/include/id3.h"
#include "ID3Lib/include/id3/misc_support.h"
#include "plugin_api.h"
#include "LameFEPlugin.h"
#include "WinampPlugin.h"
#include "CDRip/CDRip.h"
#include "OutPlugin.h"
#include "Playlist.h"
#include "Settings.h"
#include "Utils.h"
#include "I18n.h"

#include "CDPlayerIni.h"
#include "CDdbQueryDlg.h"
#include "MusicBrainz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID    2

extern CSettings g_sSettings;
extern CI18n	 g_iLang;


//////////////////////////////////////////////////////////////////////////////////
//
// Winamp Output plugin emulation. Poor, poor C with global variables and
// all this stuuf I hate, yuck :-(
//
//////////////////////////////////////////////////////////////////////////////////

// file info
int		g_samplerate;
int		g_numchannels;
int		g_bitspersamp;

// sample buffer
const int buffer_size = 8192;
int		  buf_fill;
short	  buffer_samples[8192];

// mutex to lock buffer access
HANDLE mutex;

// mutex functions

void mutex_create()
{
   mutex = ::CreateMutex(NULL, FALSE, NULL);
}

void mutex_delete()
{
   ::CloseHandle(mutex);
}

bool mutex_lock()
{
   DWORD ret = WaitForSingleObject(mutex,INFINITE);
   return ret == WAIT_OBJECT_0;
}

bool mutex_try_lock()
{
   DWORD ret = WaitForSingleObject(mutex,0);
   return ret == WAIT_OBJECT_0;
}

void mutex_unlock()
{
   ::ReleaseMutex(mutex);
}


//static members of winamp fake output "plugin"

void quit(void)
{
	TRACE("quit()\n");
}


int open(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms)
{

	TRACE("open()\n");
	TRACE("samplerate=%d numchannels=%d bitspersample=%d bufferlenms=%d prebufferms=%d", samplerate, numchannels, bitspersamp, bufferlenms, prebufferms);
	
	mutex_lock();

	g_samplerate  = samplerate;
	g_numchannels = numchannels;
	g_bitspersamp = bitspersamp;
	
	mutex_unlock();
	return 500;
}



// 0 on success. Len == bytes to write (<= 8192 always).buf is straight audio data.
// 1 returns not able to write (yet). Non-blocking always.
int write(char *buf, int len)
{

	TRACE("write %08x %d\n", buf, len);

	BOOL bLoop = FALSE;

	do
	{

		// lock mutex
		mutex_lock();

		if(buf_fill < 0){

			break;
		}

		if(buf_fill != 0){

			// do another loop
			bLoop = TRUE;
		}
		else{

			// copy samples to sample buffer
			memcpy(buffer_samples,buf,len);
			//buf_fill = len/g_numchannels/2;
			buf_fill = len;
		}

		mutex_unlock();

		if(bLoop){

			Sleep(0);
		}

	} while(bLoop);

	return 0;
}


int canwrite(void)
{

	int numbytes = 0;

	mutex_lock();

	if (buf_fill == 0){

	    numbytes = buffer_size;
	}

	mutex_unlock();

	return numbytes;
}

// called when input module stops
int isplaying(void)
{

	mutex_lock();

    buf_fill = -1;

    mutex_unlock();

    return 0; // 0 == not playing anymore
}


///////////////////////////////////////////////////
/// DUMMY functions for input module
///////////////////////////////////////////////////

void init(void){}
void close(void){}
void about(HWND hWnd){}
void config(HWND hWnd){}
void setvolume(int volume){}
void setpan(int pan){}
void flush(int t){}
int  getoutputtime(){return 0;}
int  getwrittentime(){return 0;}
int  pause(int pause){return 0;}
void setinfo(int bitrate, int srate, int stereo, int synched){}
void SAVSAinit(int maxlatency_in_ms, int srate){}
void SAVSAdeinit(){}
void SAaddpcmdata(void *pcmdata, int nch, int bps, int timestamp){}
int  SAgetmode(){return 1;}
void SAadd(void *data, int timestamp, int csa){}
void VSAaddpcmdata (void *pcmdata, int nch, int bps, int timestamp){}
int  VSAgetmode(int *specnch, int *wavench){return 0;}
void VSAadd(void *data, int timestamp){}
void vsasetinfo(int nch, int srate){}
int  dsp_isactive(){return 0;}
int  dsp_dosamples(short int *samples, int numsamples, int bps, int nch, int srate){return 0;}
void eqset(int on, char data[10], int preamp){}

///////////////////////////////////////////////////
/// END DUMMY functions for input module
///////////////////////////////////////////////////



// End Winamp Output plugin emulation
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// "dialog"feld CEncodingStatusDlg 


CEncodingStatusDlg::CEncodingStatusDlg(CWnd* pParent /*=NULL*/)
	: CTrayDialog(CEncodingStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncodingStatusDlg)
	m_strEstTime = _T("");
	m_strElaTime = _T("");
	m_strRemTime = _T("");
	m_strPhase2StatusText = _T("");
	//}}AFX_DATA_INIT
	m_strStatusText = "Status";
	m_nErrors		= 0;
	m_bAbortEnc		= FALSE;
//	m_files			= 0;
	m_pCD			= NULL;
	m_nBufferPerc	= 0;
	m_nFilePerc		= 0;
	m_nJitterPos	= 0;
	m_nErrors		= 0;
	m_bResetTimer	= TRUE;
	m_strInputDevice = "CDRip.dll";
	m_strDiscardLogMsg = "";
	m_bBatchAppendDiscID = FALSE;
	m_pEncoder		= NULL;
	m_bPhase1IsFinished = FALSE;
	m_nPhase2Perc = 0;

	//m_slLock = new CSingleLock(&m_mLockControls, FALSE);
}


void CEncodingStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncodingStatusDlg)
	DDX_Control(pDX, IDC_FILE_STATUSBAR_PHASE2, m_phase2Status);
	DDX_Control(pDX, IDC_JITTERSTATUS, m_jitterPos);
	DDX_Control(pDX, IDCANCEL, m_closeBtn);
	DDX_Control(pDX, IDC_SAVE_LOG, m_saveBtn);
	DDX_Control(pDX, IDC_LIST_STATUS_BAR, m_listStatus);
	DDX_Control(pDX, IDC_FILE_STATUS_BAR, m_fileStatus);
	DDX_Text(pDX, IDC_EST_TIME, m_strEstTime);
	DDX_Text(pDX, IDC_ELA_TIME, m_strElaTime);
	DDX_Text(pDX, IDC_REM_TIME, m_strRemTime);
	DDX_Control(pDX, IDC_LOG, m_logOut);
	DDX_Text(pDX, IDC_STATUS_STRING, m_strStatusText);
	DDX_Text(pDX, IDC_PRINTMSG, m_strDiscardLogMsg);
	DDX_Text(pDX, IDC_STATUS_STRING_PHASE2, m_strPhase2StatusText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncodingStatusDlg, CTrayDialog)
	//{{AFX_MSG_MAP(CEncodingStatusDlg)
	ON_WM_TIMER()
	ON_COMMAND(IDC_ST_RESTORE, OnStRestore)
	ON_COMMAND(IDC_ST_EXIT, OnStExit)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(IDC_SAVE_LOG, OnLogSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
//
// This function will be called before ShowDialog, to pass the Encoder ad CD
//
///////////////////////////////////////////////////////////////////////////
BOOL CEncodingStatusDlg::Init(CGenericEncoder* pEncoder, CCompactDisc* pCD, ENC_MODE eMode)
{

	if(pEncoder == NULL || pCD == NULL){

		return FALSE;
	}

	m_pEncoder = pEncoder;
	m_pCD	   = pCD;
	m_eMode    = eMode;

	m_strExtension = pEncoder->GetFileExtension();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEncodingStatusDlg 

BOOL CEncodingStatusDlg::OnInitDialog()
{

	CTrayDialog::OnInitDialog();
	
	g_iLang.TranslateDialog(this, IDD_STATUS_DIALOG);
	
	TraySetIcon(IDR_MAINFRAME);
	TraySetToolTip("LameFE");
	TraySetMenu(IDR_SYSTRAY_MENU);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	SetFocus();
	SetForegroundWindow();

	CString strTmp;
	strTmp = g_iLang.GetString(IDS_LOG_COLUMNCAPTION);
	m_logOut.Init(FALSE);
	m_logOut.InsertColumn(0, strTmp, LVCFMT_RIGHT, 400);

	// Create imaglist and attach it to the tracklist
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 4, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_LOG_IMAGES);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();

	m_logOut.SetImageList(&m_cImageList, LVSIL_SMALL);
	
	m_lLogFile.SetOutputWnd(&m_logOut);
	
	m_saveBtn.EnableWindow(FALSE);

	m_pThread = NULL;
	
	m_eThreadFinished.ResetEvent();
	
	// Fire of thread
	int nThreadPriority = THREAD_PRIORITY_NORMAL;

	switch(g_sSettings.GetThreadPriority()){

	case 0:
		nThreadPriority = THREAD_PRIORITY_LOWEST;
		break;
	case 1:
		nThreadPriority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case 2:
		nThreadPriority = THREAD_PRIORITY_NORMAL;
		break;
	case 3:
		nThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case 4:
		nThreadPriority = THREAD_PRIORITY_HIGHEST;
		break;
	}
	

	m_lLogFile.SetDebugMessage("Shooting of main encoding thread");
	m_pThread = AfxBeginThread(EncoderFunc, (void*)this, nThreadPriority);
	
	SetTimer(TIMERID, 1000, NULL);

	return TRUE;
}

void CEncodingStatusDlg::OnTimer(UINT nIDEvent)
{

	TRACE("Entering CEncodingStatusDialog::OnTimer()\n");
	
//	if(!slLock.IsLocked()){
	CSingleLock slLock(&m_mLockControls, FALSE);

	slLock.Lock();
	
	m_fileStatus.SetPos(m_nFilePerc);
	m_listStatus.SetPos(m_nBufferPerc);
	m_strStatusText.Format("Status (%02d%%)", m_nFilePerc);

	if(!(m_eMode & LFE_FILE_MODE)){

		m_jitterPos.SetPos(m_nJitterPos);
	}
	if((m_eMode & LFE_RIP_SINGLE) && g_sSettings.GetUse2Phase()){

		m_phase2Status.SetPos(m_nPhase2Perc);
		m_strPhase2StatusText.Format("Phase 2 (%02d%%)", m_nPhase2Perc);
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
	slLock.Unlock();

	CTrayDialog::OnTimer(nIDEvent);
	
	TRACE("Leaving CEncodingStatusDialog::OnTimer()\n");
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


UINT CEncodingStatusDlg::EncoderFunc(PVOID pParams)
{
	
	CEncodingStatusDlg *esdlg = (CEncodingStatusDlg *)pParams;
	
	if(esdlg->GetMode() & LFE_AUTO_MODE){

		esdlg->RipBatchMode();
	}
	else if(esdlg->GetMode() & LFE_RIP_SINGLE){

		if(g_sSettings.GetUse2Phase()){

			esdlg->RipToAny2Phase();
		}
		else{
			
			esdlg->RipToAny();
		}
	}
	else if(esdlg->GetMode() & LFE_RIP_ALBUM){

		esdlg->RipToSingleFile();
	}
	else if(esdlg->GetMode() & LFE_FILE_MODE){

		esdlg->AnyToEncoder();
	}
	else{

		ASSERT(FALSE);
	}
	// Indicate thread status
	TRACE("## Encoder Thread is finished. Set Event!!\n");
	esdlg->m_eThreadFinished.SetEvent();
	esdlg->m_pThread = NULL;
	TRACE("------- Encoder Thread is terminated ------------\n");
	return 0;
}

UINT CEncodingStatusDlg::EncoderFunc2Phase(PVOID pParams)
{
	
	CEncodingStatusDlg *esdlg = (CEncodingStatusDlg*)pParams;
	
	esdlg->Phase2Ripping();

	TRACE("## Second Phase Encoder Thread is finished. Set Event!!\n");
	esdlg->m_e2PhaseFinished.SetEvent();
	esdlg->m_p2PhaseThread = NULL;
	TRACE("------- Second Phase Encoding Thread is terminated ---------------\n");

	return 0;
}

BOOL CEncodingStatusDlg::AnyToEncoder()
{
	
	EnableCDControls(FALSE);
/*	CString	  strPlugin = "";

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
		m_strIn  = mFile->GetFileName();
		m_strOut = mFile->GetSaveAs(g_sSettings.GetWorkingDir(), m_strExtension);
		
		if(!CLameFEPlugin::FindPlugin(m_strIn.Right(3), m_strInputDevice, g_sSettings.GetAppDir() + "\\Plugins")){

			CWinampPlugin::FindPlugin(m_strIn.Right(3), m_strInputDevice, g_sSettings.GetWinampPluginPath());
		}


		m_strInputDevice = m_strInputDevice.Right(m_strInputDevice.GetLength() - m_strInputDevice.ReverseFind('\\') - 1);
		m_lLogFile.StartEntry(m_strIn, m_strOut, m_strInputDevice);
		if(Utils::FileExists(m_strOut) && !g_sSettings.GetSilentMode()){

			CString out;
			out.Format(g_iLang.GetString(IDS_ENC_FILEEXISTS), m_strOut);
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				m_lLogFile.SetErrorMsg(i, IDS_ENC_DENIEDOVWR);
				m_nErrors++;
				continue;
			}
		}
		
		TRACE("Creating non existing directories\n");
		if(!Utils::CreateDirs(m_strOut)){
				
			AfxMessageBox(g_iLang.GetString(IDS_ENC_ERRDIRCREATE), MB_ICONSTOP+MB_OK);
			m_lLogFile.SetErrorMsg(i, IDS_ENC_ERRDIRCREATE);
			m_nErrors++;
			continue;

		}

		// Look for a LameFE plugin to handle this file ...
		if(CLameFEPlugin::FindPlugin(m_strIn.Right(3), strPlugin, g_sSettings.GetAppDir() + "\\Plugins")){
		
			m_strInputDevice = strPlugin;
			LameFEPlugin2MP3(strPlugin, mFile, i);
			if(m_bAbortEnc){

				return FALSE;
			}
		}
		// ... check for a Winamp plugin afterwards if unsuccessfull
		else if(CWinampPlugin::FindPlugin(m_strIn.Right(3), strPlugin, g_sSettings.GetWinampPluginPath())){

			m_strInputDevice = strPlugin;
			WinampPlugin2Encoder(strPlugin, mFile, i);
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
	
	return TRUE;*/

	AfxMessageBox("LFE_FILE_MODE: Not implemented yet");
	return TRUE;
}


BOOL CEncodingStatusDlg::WinampPlugin2Encoder(CString plugin, CMultimediaFile *mFile, int nPos)
{
/*
	//This code is for testing only
	CWinampPlugin		wplugin(plugin);
	In_Module*			inMod;
	Out_Module			outMod;
	signed __int64		numsamples;
	char*				infilename = m_strIn.GetBuffer(10);
	m_strIn.ReleaseBuffer();


	if(!wplugin.Load(GetSafeHwnd())){

		TRACE("Error loading plugin %s\n", plugin);
		return FALSE;
	}

	inMod = wplugin.GetModule();
	
	// setup propper function calls
	outMod.version			= 0x10;
	outMod.description		= "LameFE virtual output plugin";
	outMod.id				= 9876543;

	outMod.Config			= config;
	outMod.About			= about;

	outMod.Init				= init;
	outMod.Quit				= quit;
	outMod.Open				= open;
	outMod.Close			= close;
	outMod.Write			= write;
	outMod.CanWrite			= canwrite;
	outMod.IsPlaying		= isplaying;
	outMod.Pause			= pause;
	outMod.SetPan			= setpan;
	outMod.SetVolume		= setvolume;
	outMod.Flush			= flush;
	outMod.GetOutputTime	= getoutputtime;
	outMod.GetWrittenTime	= getwrittentime;

	inMod->SetInfo			= setinfo;
	inMod->SAAddPCMData     = SAaddpcmdata;
	inMod->SAGetMode		= SAgetmode;
	inMod->SAAdd			= SAadd;
	inMod->VSAAddPCMData	= VSAaddpcmdata;
	inMod->VSAGetMode		= VSAgetmode;
	inMod->VSAAdd			= VSAadd;
	inMod->VSASetInfo		= vsasetinfo;
	inMod->dsp_isactive		= dsp_isactive;
	inMod->dsp_dosamples	= dsp_dosamples;
	inMod->EQSet			= eqset;
	inMod->SAVSADeInit      = SAVSAdeinit;
	inMod->SAVSAInit		= SAVSAinit;

	inMod->outMod			= &outMod;

	
	outMod.Init();

	g_samplerate = -1;
	g_numchannels = -1;
	g_bitspersamp = -1;
	buf_fill = -1; // disable buffer fill

	// create mutex to lock buffer access
	mutex_create();

	// try to retrieve sample rate and channel numbers
	int length_in_ms=0;

	inMod->IsOurFile(infilename);
	inMod->Play(infilename);

	BOOL bLoop = TRUE;
	
	do{

		mutex_lock();

		if (g_samplerate == -1){

			Sleep(10);
		}
		else{

			bLoop = FALSE;
		}
		mutex_unlock();
	}while(bLoop && !m_bAbortEnc);


	// calculate the number of samples we have to expect
	bLoop = TRUE;
	length_in_ms = -1;
	while(bLoop && !m_bAbortEnc){
	
		length_in_ms = inMod->GetLength();
		
		if(length_in_ms > 0){

			bLoop = FALSE;
		}
		else{

			Sleep(0);
		}			
	}

	inMod->SetOutputTime(0);
	inMod->Stop();

	buf_fill = 0; // enable buffer fill

	numsamples = (signed __int64)((length_in_ms/1000.0)*double(g_samplerate));

	// Now we can start to set up the encoder and controls :-)

	TRACE("Setting up Encoder\n");


	DWORD   dwRead     = 0;
	DWORD   dwDone	   = 0;

	CEncoder e(g_sSettings.GetAppDir());

	MMFILE_ALBUMINFO tmpAI;
	tmpAI.album   = mFile->m_id3Info.GetAlbum();
	tmpAI.artist  = mFile->m_id3Info.GetArtist();
	tmpAI.comment = mFile->m_id3Info.GetComment();
	tmpAI.genre   = mFile->m_id3Info.GetGenre();
	tmpAI.song    = mFile->m_id3Info.GetSong();
	tmpAI.track   = mFile->m_id3Info.GetTrackNum();
	tmpAI.year    = mFile->m_id3Info.GetYearNum();
	e.SetAlbumInfo(tmpAI);

	e.SetOutputFormat(m_strOutputDevice);
	e.Init();
	
	mFile->GetWfx()->wBitsPerSample = g_bitspersamp;
	mFile->GetWfx()->nSamplesPerSec = g_samplerate;
	mFile->GetWfx()->nChannels		= g_numchannels;
	
	m_lLogFile.SetInFormat(mFile->GetWfx());

	if(g_bitspersamp != 16){

		TRACE("Invalid bitrate\n");
		m_list_errors.Format(g_iLang.GetString(IDS_INVBITRATE), g_bitspersamp);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}

	if(!e.PrepareEncoding(mFile->GetSaveAs(g_sSettings.GetWorkingDir(), m_strExtension), g_numchannels, g_samplerate, g_bitspersamp)){
		
		TRACE("Error preparing encoder\n");
		m_list_errors = g_iLang.GetString(IDS_ENC_INITERR);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	
	//Setting up Controls
	slLock.Lock();
	CString strToolTip = "lameFE  | Working on file " + mFile->GetFileName();
	if(strToolTip.GetLength() > 63){


		strToolTip = strToolTip.Left(60);
		strToolTip += "...";
	}
	TraySetToolTip(strToolTip);

	if(TrayIsVisible()){

		TrayUpdate();
	}
	slLock.Unlock();

	float  in_size		= (float)numsamples * g_numchannels * g_bitspersamp;
	float  mp3_est_size = (float)e.GetEstimatedSize(g_samplerate, g_numchannels, g_bitspersamp, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_strInputSize.Format("%2.2f", in_size);
	m_estSize.Format("%2.2f", mp3_est_size);

	//Check if there's enough free disk space
	double nFreeDiskSpace = 0;
	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath());
	
	if((mp3_est_size / 1024 )>= nFreeDiskSpace){

		m_list_errors.Format
			(
			"Not enoug free disk space on drive %s. (Required space=%d, Free space%d (KB)).\nAborting Encoding process",
			g_sSettings.GetBasePath().Left(2),
			m_estSize, nFreeDiskSpace
			);

		m_nErrors++;
		AfxMessageBox(m_list_errors, MB_OK+MB_ICONSTOP);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		e.DeInit();
		return FALSE;
	}
	
	DWORD  dwSampleBufferSize = e.GetSamplesToRead();
	
	CEncoderFeeder ef(&e, dwSampleBufferSize, 256, m_bAbortEnc);

	// Now we are ready to start the Winamp plugin
    mutex_lock();

	int		nOffset		= 0;
	//int		samplesUsed	= 0;
	char*	pcStream	= new char[8196+dwSampleBufferSize];
	signed __int64		samples_converted	= 0;


   	inMod->IsOurFile(infilename);
	inMod->Play(infilename);
	Sleep(20);
    // release mutex
    mutex_unlock();
    Sleep(20);

	bLoop = TRUE;

	while((buf_fill != -1) && !m_bAbortEnc){ // buf_fill is -1 when file is done


		//permenantly look if there is data in the buffer
		do{	
			
			// lock mutex
			mutex_lock();

			// now we have exclusive access to the buffer
			if(buf_fill < 0){

				// stop processing
				bLoop = FALSE;
			}
			else if (buf_fill != 0){

				bLoop = FALSE;

				//copy the read bytes to the end of the encoding stream:
				
				memcpy(pcStream + nOffset, buffer_samples, buf_fill); //cOrigBuff, len);

				SHORT*   psEncodeStream     = (SHORT*)pcStream;
				DWORD    dwSamplesToConvert	= (buf_fill + nOffset) / sizeof(SHORT);

				while (dwSamplesToConvert >= dwSampleBufferSize){

					ef.AddData(psEncodeStream, dwSampleBufferSize);

					dwSamplesToConvert -= dwSampleBufferSize;

					psEncodeStream += dwSampleBufferSize;
				}

				// Copy the remaing bytes up front, if necessary
				if (dwSamplesToConvert > 0){

					// Calculate the offset in bytes
					nOffset = dwSamplesToConvert * sizeof(SHORT);

					// Copy up front
					memcpy(pcStream, psEncodeStream, nOffset);
				}
				else{
					nOffset=0;
				}
				
				m_nBufferPerc = ef.GetBufferStatus();
				
				samples_converted += dwSampleBufferSize / g_numchannels / sizeof(short);
				m_nFilePerc = (double)samples_converted / (double)numsamples * 100;


				buf_fill = 0;
			}

			// release mutex
			mutex_unlock();
		}while(bLoop && !m_bAbortEnc);
	}
	
	if(m_bAbortEnc){
		
		TRACE("Encoding was aborted, stop playback\n");
		inMod->Stop();
		wplugin.Unload();
		inMod = NULL;
	}

	ef.WaitForFinished();
	e.DeInit();
	delete pcStream;
	pcStream = NULL;

	if(m_bAbortEnc){
		
		TRACE("Encoding aborted by user, so return FALSE\n");

		return FALSE;
	}
	
	wplugin.Unload();

	inMod = NULL;
	
	if((g_sSettings.GetId3v1() || g_sSettings.GetId3v2()) && m_strOutputDevice == "lame_enc.dll"){
	
		if(!WriteID3Tag(mFile)){

			TRACE("Error writing id3Tag\n");
			m_lLogFile.SetErrorMsg(nPos, IDS_ENC_ID3FAILED);
			m_nErrors++;
		}
	}
	if(m_lLogFile.GetState(nPos)){

		m_lLogFile.SetErrorMsg(nPos, IDS_ENC_SUCCESS);
	}

*/
	return TRUE;
}

BOOL CEncodingStatusDlg::LameFEPlugin2MP3(CString plugin, CMultimediaFile *mFile, int nPos)
{


/*	PLF_IN    inModule	= NULL;
	HINSTANCE hDLL		= NULL;
	GETMODULE GetModule	= NULL;

	CFileStatus rStatus;
	CFile::GetStatus( mFile->GetFileName(), rStatus );
	float in_size = rStatus.m_size;
	float in_sizeKB = rStatus.m_size;

	hDLL = LoadLibrary(plugin);
	if(!hDLL){

		TRACE("Failed Loading Library\n");
		m_list_errors.Format(g_iLang.GetString(IDS_ENC_PLUG_LOADFAILED), plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	
	GetModule = (GETMODULE)GetProcAddress(hDLL, "GetLFINModule");
	if(!GetModule){

		TRACE("Failed getting proc address\n");
		m_list_errors.Format(g_iLang.GetString(IDS_ENC_PLUG_APIERROR), plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;

		return FALSE;
	}

	inModule = GetModule();
	if(!inModule){

		TRACE("Failed getting module\n");
		m_list_errors.Format(g_iLang.GetString(IDS_ENC_PLUG_APIERROR), plugin);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;

		return FALSE;
	}
	
	inModule->hDllInstance = hDLL;
	inModule->hMainWindow  = NULL;
	inModule->GetProfileString = CLameFEPlugin::GetProfileString;
	inModule->SetProfileString = CLameFEPlugin::SetProfileString;

	inModule->Init();
	if(inModule->Open(mFile->GetFileName()) != 0){

		TRACE("Failed opening file\n");
		m_list_errors.Format(g_iLang.GetString(IDS_FILEOPENERR), mFile->GetFileName());
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}


	TRACE("Setting up Encoder\n");

	PSHORT	pWAVBuffer = NULL;
	DWORD   dwRead     = 0;
	DWORD   dwDone	   = 0;

	CEncoder e(g_sSettings.GetAppDir());

	MMFILE_ALBUMINFO tmpAI;
	tmpAI.album   = mFile->m_id3Info.GetAlbum();
	tmpAI.artist  = mFile->m_id3Info.GetArtist();
	tmpAI.comment = mFile->m_id3Info.GetComment();
	tmpAI.genre   = mFile->m_id3Info.GetGenre();
	tmpAI.song    = mFile->m_id3Info.GetSong();
	tmpAI.track   = mFile->m_id3Info.GetTrackNum();
	tmpAI.year    = mFile->m_id3Info.GetYearNum();
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
		m_list_errors.Format(g_iLang.GetString(IDS_INVBITRATE), mmFormat.nBitsPerSample);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	if(!e.PrepareEncoding(mFile->GetSaveAs(g_sSettings.GetWorkingDir(), m_strExtension), mmFormat.nChannels, mmFormat.dwSampleRate, mmFormat.nBitsPerSample)){
		
		TRACE("Error preparing encoder\n");
		m_list_errors = g_iLang.GetString(IDS_ENC_INITERR);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}
	
	//Setting up Controls
	slLock.Lock();
	CString strToolTip = "lameFE  | Working on file " + mFile->GetFileName();
	if(strToolTip.GetLength() > 63){


		strToolTip = strToolTip.Left(60);
		strToolTip += "...";
	}
	TraySetToolTip(strToolTip);

	if(TrayIsVisible()){

		TrayUpdate();
	}
	slLock.Unlock();


	double mp3_est_size = e.GetEstimatedSize(mmFormat.dwSampleRate, mmFormat.nChannels, mmFormat.nBitsPerSample, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_strInputSize.Format("%2.2f", in_size);
	m_estSize.Format("%2.2f", mp3_est_size);

	//Check if there's enough free disk space
	double nFreeDiskSpace = 0;
	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath());
	
	if((mp3_est_size / 1024 )>= nFreeDiskSpace){

		m_list_errors.Format
			(
			"Not enoug free disk space on drive %s. (Required space=%d, Free space%d (KB)).\nAborting Encoding process",
			g_sSettings.GetBasePath().Left(2),
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
	

	while(((dwRead = inModule->Read(pWAVBuffer, sampleToRead)) > 0) && !m_bAbortEnc)
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
	
	
	if((g_sSettings.GetId3v1() || g_sSettings.GetId3v2()) && m_strOutputDevice == "lame_enc.dll"){
	
		if(!WriteID3Tag(mFile)){

			TRACE("Error writing id3Tag\n");
			m_lLogFile.SetErrorMsg(nPos, IDS_ENC_ID3FAILED);
			m_nErrors++;
		}
	}
	if(m_lLogFile.GetState(nPos)){

		m_lLogFile.SetErrorMsg(nPos, IDS_ENC_SUCCESS);
	}
*/
	return TRUE;
}

BOOL CEncodingStatusDlg::RipBatchMode()
{
	BOOL	bTimesOut	 = g_sSettings.GetBatchTimesOut();
	BOOL    bTimedOut	 = FALSE;
	BOOL	bSuccessfull = FALSE;
	CString strTmp	     = g_sSettings.GetBatchScript();
	int     i;
	long nTimeOutSec =  0;
	int nNumCDDrives =  CR_GetNumCDROM();
	int nOrgActCD	 =  CR_GetActiveCDROM();
	int nActiveCD	 =  nOrgActCD;
	CString strDrive =  "";

	CDROMPARAMS cdParams;
	strcpy(cdParams.lpszCDROMID, "");

	CStringArray aScript;
	
	while((i = strTmp.Find("|", 0)) > 0){

		aScript.Add(strTmp.Left(i));
		strTmp = strTmp.Right(strTmp.GetLength() - i - 1);
	}

	// Main Batch Loop
	while(!m_bAbortEnc && !bTimedOut){

		// Main Encoding Loop
		for(int i = 0; (i < aScript.GetSize()) && !m_bAbortEnc && !bTimedOut; i++){

			if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_BEEP)){

				Beep(2000, 200);
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_IFERROR)){
				
				if(bSuccessfull) i++;
				continue;
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_CDPLAYERINI)){
				
				CCDPlayerIni cdp(m_pCD);
				bSuccessfull = cdp.Read();
				
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_CDTEXT)){

				m_pCD->ReadCDText();
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_MUSICBRAINZ)){

				CMusicBrainz mz(m_pCD);		
				bSuccessfull = mz.QueryDB();
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_FREEDB)){

				CCDdbQueryDlg dlg(this, m_pCD, nActiveCD, TRUE);
				bSuccessfull = (dlg.DoModal() == IDOK);
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_RIP)){

				m_bBatchAppendDiscID = !bSuccessfull;
				if(m_eMode & LFE_RIP_SINGLE){

					if(g_sSettings.GetUse2Phase()){

						RipToAny2Phase();
					}
					else{
						
						RipToAny();
					}
				}
				else if(LFE_RIP_ALBUM){

					RipToSingleFile();
				}
				else{

					ASSERT(FALSE);
				}
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_EJECT)){

				m_pCD->Eject();
			}
			else if(aScript.GetAt(i) == g_iLang.GetString(IDS_NBATCH_FINISH)){
				
				i = aScript.GetSize();
			}
		} // End Script Loop

		if(m_bAbortEnc){

			return FALSE;
		}
		//Reset status stuff
		strDrive.Format(g_iLang.GetString(IDS_BATCHNEXTCD), cdParams.lpszCDROMID);
		m_lLogFile.SetNotificationMessage(strDrive);
		m_fileStatus.SetRange(0, 100);
		m_fileStatus.SetPos(0);
		m_listStatus.SetRange(0,100);
		m_listStatus.SetPos(0);
		m_nErrors = 0;
		m_strIn.Empty();
		m_strOut.Empty();
		m_nBufferPerc = 0;
		m_nJitterPos = 0;
		m_nFilePerc = 0;
		//m_inputSize.Empty();
		//m_estSize.Empty();
		m_bResetTimer = TRUE;
		PostMessage(WM_TIMER,0,0);

		//Unlock drive and eject cd
		CR_LockCD(FALSE);
		CR_EjectCD(TRUE);
		m_pCD->Eject();
		m_bBatchAppendDiscID = FALSE;
	
		if(g_sSettings.GetBatchUseAllDrives()){

			//switch to next drive 
			//CR_SaveSettings();
			CR_SetActiveCDROM((nActiveCD+1 < nNumCDDrives ? ++nActiveCD : (nActiveCD = 0)));
			CR_GetCDROMParameters(&cdParams);
			strDrive.Format(g_iLang.GetString(IDS_BATCHCDDRIVE), cdParams.lpszCDROMID);
			m_lLogFile.SetNotificationMessage(strDrive);
			CR_SaveSettings();
			CR_LoadSettings();
		}

		CDMEDIASTATUS cdMediaStatus = CDMEDIA_NOT_PRESENT;
		CR_IsMediaLoaded(cdMediaStatus);
		while((cdMediaStatus != CDMEDIA_PRESENT) && !bTimedOut && !m_bAbortEnc){

			Sleep(5000);
			if(bTimesOut){

				nTimeOutSec -= 5;
				if(nTimeOutSec <= 0){
					
					TRACE("Batchmode timed out\n");
					m_lLogFile.SetNotificationMessage(g_iLang.GetString(IDS_BATCHTIMEDOUT));
					bTimedOut = TRUE;
					return FALSE;
				}
			}			
			CR_IsMediaLoaded(cdMediaStatus);
		}	
		m_pCD->Init();
	} // End Batch-Loop

	return TRUE;
}

BOOL CEncodingStatusDlg::RipToSingleFile(){

	CSingleLock slLock(&m_mLockControls, FALSE);
	TRACE("RipToSingleFile: Outputdevice is %s", m_strOutputDevice);

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
	int	 	 numTracks    = m_pCD->GetNumSelTracks();

	int nNumBuffers = g_sSettings.GetNumReadBuffers();
	switch(nNumBuffers){

	case 0:
		nNumBuffers = 256;
		break;
	case 1:
		nNumBuffers = 1024;
		break;
	case 2:
		nNumBuffers = 6144;
		break;
	case 3:
		nNumBuffers = 16384;
		break;
	default:
		nNumBuffers = 256;
	}

	pSel	= new int[numTracks];
	m_pCD->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_strIn.Format("Audio CD %s", m_pCD->GetStrVolID());
	m_strOut = m_pCD->GetAlbumString(g_sSettings.GetWorkingDir(), m_strExtension, m_bBatchAppendDiscID);

	m_lLogFile.StartSession(1, m_strOutputDevice);
	m_lLogFile.StartEntry(m_strIn, m_strOut);
	
	WAVEFORMATEX wfx;
	wfx.nChannels	    = 2;
	wfx.nSamplesPerSec  = 44100;
	wfx.wBitsPerSample	= 16;
	m_lLogFile.SetInFormat(&wfx);

	// Check for existing file
	TRACE("Checking for existing file\n");

	if(Utils::FileExists(m_strOut) && !g_sSettings.GetSilentMode()){

		CString out;
		out.Format(g_iLang.GetString(IDS_ENC_FILEEXISTS), m_strOut);
		if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

			m_lLogFile.SetErrorMsg(0, g_iLang.GetString(IDS_ENC_DENIEDOVWR));
			m_nErrors++;
			return FALSE;
		}
	}
	
	TRACE("Creating non existing directories\n");
	if(!Utils::CreateDirs(m_strOut)){
			
		AfxMessageBox(g_iLang.GetString(IDS_ENC_ERRDIRCREATE), MB_ICONSTOP+MB_OK);
		m_lLogFile.SetErrorMsg(0, g_iLang.GetString(IDS_ENC_ERRDIRCREATE));
		m_nErrors++;
		return FALSE;

	}

	TRACE("Setting up Encoder\n");

	//CEncoder e(g_sSettings.GetAppDir());

	/*MMFILE_ALBUMINFO tmpAI;
	tmpAI.album   = m_pCD->GetCDTrack(0)->m_id3Info.GetAlbum();
	tmpAI.artist  = m_pCD->GetCDTrack(0)->m_id3Info.GetArtist();
	tmpAI.comment = m_pCD->GetCDTrack(0)->m_id3Info.GetComment();
	tmpAI.genre   = m_pCD->GetCDTrack(0)->m_id3Info.GetGenre();
	tmpAI.song    = "Complete Album";
	tmpAI.track   = 0;
	tmpAI.year    = m_pCD->GetCDTrack(0)->m_id3Info.GetYearNum();

	e.SetAlbumInfo(tmpAI);
	
	e.SetOutputFormat(m_strOutputDevice);
	e.Init();
	e.PrepareEncoding(m_strOut, 2, 44100, 16);
*/
	m_pEncoder->PrepareEncoding(m_pCD->GetCDTrack(0)->m_id3Info, m_strOut, 44100, 16, 2);

	unsigned long dwSampleBufferSize = m_pEncoder->GetSamplesToRead();
	unsigned long dwStartSector		 = m_pCD->GetCDTrack(0)->m_dwStartSector;
	unsigned long dwEndSector        = m_pCD->GetEndSector(m_pCD->GetLastAudioTrack()+1);

	//Setting up Controls
	slLock.Lock();
	
	TraySetToolTip(g_iLang.GetString(IDS_WORKING_ON_ALBUM));
	TrayUpdate();

	float  in_size	    = (dwEndSector - dwStartSector) * 2352;
	double mp3_est_size = m_pEncoder->GetEstimatedSize(in_size, 2, 44100, 16);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	//m_strInputSize.Format("%2.2f", in_size);
	//m_strEstSize.Format("%2.2f", mp3_est_size);

	//Check if there's enough free disk space
	double nFreeDiskSpace = 0;

	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath()) / (1024.0*1024.0);

	PostMessage(WM_TIMER,0,0);

	if((mp3_est_size)>= nFreeDiskSpace){

		slLock.Lock();
		CString strTmp;
		strTmp.Format(g_iLang.GetString(IDS_NOTENOUGHDISKSPACE), g_sSettings.GetBasePath());
		m_lLogFile.SetErrorMsg(0, strTmp);
		AfxMessageBox(strTmp, MB_OK+MB_ICONSTOP);
		m_nErrors++;
		slLock.Unlock();
		return FALSE;
	}
	slLock.Unlock();

	m_nFilePerc = 0;
	m_bResetTimer = TRUE;

	if (CR_OpenRipper(&nBufferSize, dwStartSector, dwEndSector) == CDEX_OK){
		
		LONG nNumBytesRead = 0;
		LONG nOffset=0;
		
		BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * sizeof(SHORT)];

		CDEX_ERR ripErr;
		CEncoderFeeder ef(m_pEncoder, dwSampleBufferSize, nNumBuffers, m_bAbortEnc);
		
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
				jErr.Format(g_iLang.GetString(IDS_ENC_JITTER_ERR), dwStartSector, dwEndSector);
				m_lLogFile.SetErrorMsg(0, jErr);
				m_nErrors++;
				PostMessage(WM_TIMER,0,0);
			}

			if (CDEX_ERROR == ripErr){
				
				m_nErrors++;
			
				CString strTmp;
				strTmp.Format(g_iLang.GetString(IDS_ENC_NUMERRORS), m_nErrors);
				m_lLogFile.SetErrorMsg(0, strTmp);
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
		m_pEncoder->CloseStream();

		delete pbtStream;
		
		CR_CloseRipper();

		if(m_bAbortEnc){

			TRACE("Encoding process aborted by user\n");
			return FALSE;
		}


		if(g_sSettings.GetWriteCue()){

			m_pCD->WriteCueSheet(g_sSettings.GetWorkingDir(), m_strExtension);
		}
	}
	
	m_lLogFile.FinalizeEntry();
	m_lLogFile.FinalizeSession();

	delete pSel;

	if(g_sSettings.GetLock() == TRUE){
		
		CR_LockCD(FALSE);
	}
	
	if(g_sSettings.GetEject() && !(m_eMode & LFE_AUTO_MODE)){

		CR_EjectCD(TRUE);
	}
	return TRUE;
}		


void CEncodingStatusDlg::Phase2Ripping()
{

	TRACE("Entering CEncodingStatusDlg::Phase2Ripping()\n");

	m_lLogFile.SetInformationMsg(g_iLang.GetString(IDS_STARTINGPHASE2));
	
	m_phase2Status.SetRange(0, 100);
	m_nPhase2Perc = 0;

	FILE *pFileIn		  = NULL;
	PSHORT psDataBuffer	  = NULL;
	CString strTmpFolder;

	EnterCriticalSection(&m_csQueueLock);
	{	
		
		strTmpFolder = m_q2PhaseIn.front();
	}
	LeaveCriticalSection(&m_csQueueLock);

	strTmpFolder = strTmpFolder.Left(strTmpFolder.ReverseFind('\\'));

	while(!m_bPhase1IsFinished && !m_bAbortEnc){
		

		EnterCriticalSection(&m_csQueueLock);
		if(m_q2PhaseIn.empty()){
			
			LeaveCriticalSection(&m_csQueueLock);
			TRACE("Waiting for next file...\n");
			Sleep(50);
			continue;
		}
		else{
			
			LeaveCriticalSection(&m_csQueueLock);
		}

		DWORD dwSamplesToRead = 0;
		DWORD dwSize          = 0;
		DWORD dwRead		  = 0;
		DWORD dwDone		  = 0;

		
		int nTrackNum = 0;
		CString strTmp;
		EnterCriticalSection(&m_csQueueLock);
		{	
			strTmp = m_q2PhaseIn.front();
		}
		LeaveCriticalSection(&m_csQueueLock);
		
		pFileIn = fopen(strTmp, "rb");
		if(!pFileIn){

			TRACE("Failed opening file\n");
			continue;
		}
		
		m_lLogFile.SetInformationMsg(g_iLang.GetString(IDS_ENCODINGTEMPFILE) + strTmp);

		fseek(pFileIn, 0, SEEK_END);
		dwSize = ftell(pFileIn);

		fseek(pFileIn, 0, SEEK_SET);
		strTmp = strTmp.Mid(strTmp.GetLength() - 6, 2);
		
		nTrackNum = atoi(strTmp);
		
		EnterCriticalSection(&m_csQueueLock);
		{	
			m_pEncoder->PrepareEncoding(m_pCD->GetCDTrack(nTrackNum-1)->m_id3Info, m_q2PhaseOut.front(),44100, 16, 2);
		}
		LeaveCriticalSection(&m_csQueueLock);


		dwSamplesToRead = m_pEncoder->GetSamplesToRead();
		psDataBuffer = new SHORT[dwSamplesToRead];
		
		while((dwRead = fread(psDataBuffer, sizeof(SHORT), dwSamplesToRead, pFileIn)) > 0){

			//TRACE("%03d%%\n", m_nPhase2Perc);
			m_pEncoder->PassBuffer(psDataBuffer, dwSamplesToRead);
			dwDone += dwRead * sizeof(SHORT);
			m_nPhase2Perc = 100*(float)dwDone/(float)dwSize;
		}
		
		fclose(pFileIn);
		m_pEncoder->CloseStream();
		delete psDataBuffer;
		psDataBuffer = NULL;

		EnterCriticalSection(&m_csQueueLock);
		{	

			remove(m_q2PhaseIn.front());
			m_q2PhaseIn.pop();
			m_q2PhaseOut.pop();
		}
		LeaveCriticalSection(&m_csQueueLock);
	}
	
	_rmdir(strTmpFolder);

	m_lLogFile.SetNotificationMessage(g_iLang.GetString(IDS_PHASE2FINISHED));
	m_lLogFile.FinalizeSession();
	
	TRACE("Leaving CEncodingStatusDlg::Phase2Ripping()\n");
}

BOOL CEncodingStatusDlg::RipToAny2Phase()
{

	m_lLogFile.SetDebugMessage("Entering CEncodingStatusDlg::RipToAny()");
	InitializeCriticalSection(&m_csQueueLock);

	EnableCDControls(TRUE);		
	if(g_sSettings.GetLock()){

		CR_LockCD(TRUE);
	}

	//setup controls
	GetDlgItem(IDC_STATUS_STRING_PHASE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_2PHASE_100)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_2PHASE_0)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILE_STATUSBAR_PHASE2)->ShowWindow(SW_SHOW);
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
	int	 	 numTracks    = m_pCD->GetNumSelTracks();
	FILE*	 pTempFile	  = NULL;

	int nNumBuffers = g_sSettings.GetNumReadBuffers();
	switch(nNumBuffers){

	case 0:
		nNumBuffers = 256;
		break;
	case 1:
		nNumBuffers = 1024;
		break;
	case 2:
		nNumBuffers = 6144;
		break;
	case 3:
		nNumBuffers = 16384;
		break;
	default:
		nNumBuffers = 256;
	}

	pSel	= new int[numTracks];
	m_pCD->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	DWORD	dwSampleBufferSize = 2400;

	m_lLogFile.StartSession(numTracks, "CDRip.dll");

	while(currentTrack < numTracks && !m_bAbortEnc){
	
		//////////////////////////////////
		// Lock Mutex

		cdTrack = m_pCD->GetCDTrack(pSel[currentTrack]);
		m_strOut = m_pCD->GetSaveAs(cdTrack->m_btTrack, g_sSettings.GetWorkingDir(), m_strExtension, m_bBatchAppendDiscID);

		if(!cdTrack->IsAudioTrack()){
			
			TRACE("Shouldn't have happened :(\n");
			continue;
		}

		m_lLogFile.StartEntry(cdTrack->GetTrackname(), m_strOut, m_strOutputDevice);


		if(Utils::FileExists(m_strOut) && !g_sSettings.GetSilentMode()){

			CString out;
			out.Format(g_iLang.GetString(IDS_ENC_FILEEXISTS), m_strOut);
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_DENIEDOVWR);
				currentTrack++;
				m_nErrors++;
				continue;
			}
		}

		
		if(!Utils::CreateDirs(m_strOut)){
				
			AfxMessageBox(g_iLang.GetString(IDS_ENC_ERRDIRCREATE), MB_ICONSTOP+MB_OK);
			m_lLogFile.SetErrorMsg(currentTrack, IDS_ENC_ERRDIRCREATE);
			m_nErrors++;
			currentTrack++;
			continue;

		}

		WAVEFORMATEX wfx;
		wfx.nChannels	    = 2;
		wfx.nSamplesPerSec  = 44100;
		wfx.wBitsPerSample	= 16;
		m_lLogFile.SetInFormat(&wfx);
		

		//Setting up Controls
		
		TraySetToolTip(g_iLang.GetString(IDS_WORKING_ON_FILE) + cdTrack->GetTrackname());
		TrayUpdate();

		float  in_size	    = m_pCD->GetTrackSize(currentTrack);
		float  mp3_est_size = m_pEncoder->GetEstimatedSize(in_size, 2, 44100, 16);

		in_size		 /= (1024.0*1024.0);	//in MB
		mp3_est_size /= (1024.0*1024.0);	//in MB

		m_strInputSize.Format("%2.2f", in_size);
		m_strEstSize.Format("%2.2f", mp3_est_size);
		
		//Check if there's enough free disk space
		double nFreeDiskSpace = 0;
		nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath()) / (1024.0*1024.0);

		if(mp3_est_size >= nFreeDiskSpace){

			m_nErrors++;
			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_NOTENOUGHDISKSPACE), g_sSettings.GetBasePath().Left(2));
			m_lLogFile.SetErrorMsg(currentTrack, strTmp);
			AfxMessageBox(strTmp, MB_OK+MB_ICONSTOP);
			return FALSE;
		}

		nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetTempPath());
		
		if(m_pCD->GetTrackSize(currentTrack) >= nFreeDiskSpace){

			m_nErrors++;
			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_NOTENOUGHDISKSPACE), g_sSettings.GetBasePath().Left(2));
			m_lLogFile.SetErrorMsg(currentTrack, strTmp);
			AfxMessageBox(strTmp, MB_OK+MB_ICONSTOP);
			return FALSE;
		}
		m_strIn.Format(cdTrack->GetTrackname());

		m_nFilePerc = 0;
		m_bResetTimer = TRUE;

		CString strTmpFile;
		strTmpFile.Format(g_sSettings.GetTempPath() + "\\%X\\Track_%02d.raw", m_pCD->GetDiscID(), cdTrack->m_btTrack);
		
		m_lLogFile.SetInformationMsg(g_iLang.GetString(IDS_OPENTEMPFILE) + strTmpFile);
		

		Utils::CreateDirs(strTmpFile);
		pTempFile = fopen(strTmpFile, "wb+");

		PostMessage(WM_TIMER,0,0);

				
		//end setting up controls

		if (CR_OpenRipper(
					&nBufferSize, 
					cdTrack->m_dwStartSector,
					m_pCD->GetEndSector(pSel[currentTrack] + 1)
				 ) == CDEX_OK
			){
			
			LONG nNumBytesRead = 0;
			LONG nOffset=0;
			
			BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * sizeof(SHORT)];

			CDEX_ERR ripErr;
			
			// Read all chunks
			while((ripErr=CR_RipChunk(pbtStream + nOffset, &nNumBytesRead, m_bAbortEnc)) != CDEX_RIPPING_DONE  && !m_bAbortEnc){
				
				SHORT*	psEncodeStream     = (SHORT*)pbtStream;
				DWORD	dwSamplesToConvert = (nNumBytesRead + nOffset) / sizeof(SHORT); /// 2;

				//Check for jitter m_nErrors
				if (CDEX_JITTER_ERROR == ripErr)
				{
					DWORD dwStartSector, dwEndSector;

					// Get info where jitter error did occur
					CR_GetLastJitterErrorPosition(dwStartSector,dwEndSector);
					CString jErr;
					jErr.Format(g_iLang.GetString(IDS_ENC_JITTER_ERR), dwStartSector, dwEndSector);
					m_lLogFile.SetErrorMsg(currentTrack, jErr);
					m_nErrors++;
					PostMessage(WM_TIMER,0,0);
				}

				if (CDEX_ERROR == ripErr){
					
					currentTrack++;
					m_nErrors++;
					m_lLogFile.SetErrorMsg(currentTrack, g_iLang.GetString(IDS_ENC_RIPPERROR));
					
					PostMessage(WM_TIMER,0,0);
					break;
				}
				
				fwrite(psEncodeStream, nNumBytesRead, 1, pTempFile);

				m_nFilePerc   = CR_GetPercentCompleted();
				m_nJitterPos  = CR_GetJitterPosition();
				m_nBufferPerc = 100;
			}//end while Ripp
			
			delete pbtStream;
			
			CR_CloseRipper();
			
			if(m_bAbortEnc){

				TRACE("Encoding process aborted by user\n");
				return FALSE;
			}


			if(m_lLogFile.GetState(currentTrack)){

				m_lLogFile.SetInformationMsg(g_iLang.GetString(IDS_ENC_PHASE1_SUCCESS));
			}

			m_lLogFile.FinalizeEntry();
			
			fclose(pTempFile);
			pTempFile = NULL;


			EnterCriticalSection(&m_csQueueLock);
			{

				m_q2PhaseIn.push(strTmpFile);
				m_q2PhaseOut.push(m_strOut);
			}
			LeaveCriticalSection(&m_csQueueLock);

			if(currentTrack == 0){

				m_p2PhaseThread = AfxBeginThread(EncoderFunc2Phase, (void*)this, THREAD_PRIORITY_NORMAL);
			}
			currentTrack++;
		} //end for file
		else{

			return FALSE;
		}
		
	}
	
	delete pSel;
	//m_lLogFile.FinalizeSession();
	m_nFilePerc   = 0;
	m_nJitterPos  = 0;
	m_nBufferPerc = 0;
	EnableCDControls(FALSE);
	Sleep(100);

	if(g_sSettings.GetLock()){
		
		CR_LockCD(FALSE);
	}
	
	if(g_sSettings.GetEject() && !(m_eMode & LFE_AUTO_MODE)){

		CR_EjectCD(TRUE);
	}
	
	m_bPhase1IsFinished = TRUE;

	TRACE("--- Waiting for Phase 2 Encoding thread to finish ----\n");
	WaitForSingleObject(m_e2PhaseFinished, INFINITE);
	TRACE("OK, We are done, now exit the main encoding thread\n");
	DeleteCriticalSection(&m_csQueueLock);

	return TRUE;
}

BOOL CEncodingStatusDlg::RipToAny()
{
	
	CSingleLock slLock(&m_mLockControls, FALSE);

	if(g_sSettings.GetLock()){

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
	int	 	 numTracks    = m_pCD->GetNumSelTracks();
	FILE*	 pTempFile	  = NULL;

	int nNumBuffers = g_sSettings.GetNumReadBuffers();
	switch(nNumBuffers){

	case 0:
		nNumBuffers = 256;
		break;
	case 1:
		nNumBuffers = 1024;
		break;
	case 2:
		nNumBuffers = 6144;
		break;
	case 3:
		nNumBuffers = 16384;
		break;
	default:
		nNumBuffers = 256;
	}

	pSel	= new int[numTracks];
	m_pCD->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_lLogFile.StartSession(numTracks, "CDRip.dll");

	while(currentTrack < numTracks && !m_bAbortEnc){
	
		cdTrack = m_pCD->GetCDTrack(pSel[currentTrack]);
		m_strOut = m_pCD->GetSaveAs(cdTrack->m_btTrack, g_sSettings.GetWorkingDir(), m_strExtension, m_bBatchAppendDiscID);
	
		if(!cdTrack->IsAudioTrack()){
			
			TRACE("Shouldn't have happened :(\n");
			ASSERT(FALSE);
			continue;
		}

		// Check for existing file
		m_lLogFile.StartEntry(cdTrack->GetTrackname(), m_strOut, m_strOutputDevice);
		if(Utils::FileExists(m_strOut) && !g_sSettings.GetSilentMode()){

			CString out;
			out.Format(g_iLang.GetString(IDS_ENC_FILEEXISTS), m_strOut);
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				m_lLogFile.SetErrorMsg(currentTrack, g_iLang.GetString(IDS_ENC_DENIEDOVWR));
				currentTrack++;
				m_nErrors++;
				continue;
			}
		}
		
		m_lLogFile.SetDebugMessage("Creating non existing directories\n");
		if(!Utils::CreateDirs(m_strOut)){
				
			AfxMessageBox(g_iLang.GetString(IDS_ENC_ERRDIRCREATE), MB_ICONSTOP+MB_OK);
			m_lLogFile.SetErrorMsg(currentTrack, g_iLang.GetString(IDS_ENC_ERRDIRCREATE));
			m_nErrors++;
			currentTrack++;
			continue;

		}

		m_lLogFile.SetDebugMessage("Setting up Encoder\n");

		m_pEncoder->PrepareEncoding(cdTrack->m_id3Info, m_strOut, 44100, 16, 2);

		DWORD dwSampleBufferSize = m_pEncoder->GetSamplesToRead();
		
		WAVEFORMATEX wfx;
		wfx.nChannels	    = 2;
		wfx.nSamplesPerSec  = 44100;
		wfx.wBitsPerSample	= 16;
		m_lLogFile.SetInFormat(&wfx);
		

		//Setting up Controls
		slLock.Lock();
		
		TraySetToolTip(g_iLang.GetString(IDS_WORKING_ON_FILE) + cdTrack->GetTrackname());
		TrayUpdate();

		float  in_size	    = m_pCD->GetTrackSize(currentTrack);
		float  mp3_est_size = m_pEncoder->GetEstimatedSize(in_size, 2, 44100, 16);

		in_size		 /= (1024.0*1024.0);	//in MB
		mp3_est_size /= (1024.0*1024.0);	//in MB

		m_strInputSize.Format("%2.2f", in_size);
		m_strEstSize.Format("%2.2f", mp3_est_size);
		
		//Check if there's enough free disk space
		double nFreeDiskSpace = 0;
		nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath()) / (1024.0*1024.0);

		if((mp3_est_size)>= nFreeDiskSpace){

			m_nErrors++;
			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_NOTENOUGHDISKSPACE), g_sSettings.GetBasePath().Left(2));
			m_lLogFile.SetErrorMsg(currentTrack, strTmp);
			AfxMessageBox(strTmp, MB_OK+MB_ICONSTOP);
			return FALSE;
		}

		m_strIn.Format(cdTrack->GetTrackname());

		m_nFilePerc = 0;
		m_bResetTimer = TRUE;
		slLock.Unlock();
		PostMessage(WM_TIMER,0,0);
		
		
		//end setting up controls

		if (CR_OpenRipper(
					&nBufferSize, 
					cdTrack->m_dwStartSector,
					m_pCD->GetEndSector(pSel[currentTrack] + 1)
				 ) == CDEX_OK
			){
			
			LONG nNumBytesRead = 0;
			LONG nOffset=0;
			
			BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * sizeof(SHORT)];

			CDEX_ERR ripErr;
			CEncoderFeeder ef(m_pEncoder, dwSampleBufferSize, nNumBuffers, m_bAbortEnc);
			
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
					jErr.Format(g_iLang.GetString(IDS_ENC_JITTER_ERR), dwStartSector, dwEndSector);
					m_lLogFile.SetErrorMsg(currentTrack, jErr);
					m_nErrors++;
					PostMessage(WM_TIMER,0,0);
				}

				if (CDEX_ERROR == ripErr){
					
					currentTrack++;
					m_nErrors++;
					m_lLogFile.SetErrorMsg(currentTrack, g_iLang.GetString(IDS_ENC_RIPPERROR));
					
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
			m_pEncoder->CloseStream();
			
			delete pbtStream;
			
			CR_CloseRipper();
			
			if(m_bAbortEnc){

				TRACE("Encoding process aborted by user\n");
				return FALSE;
			}

			if(m_lLogFile.GetState(currentTrack)){

				m_lLogFile.SetErrorMsg(currentTrack, g_iLang.GetString(IDS_ENC_SUCCESS));
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

	if(g_sSettings.GetLock()){
		
		CR_LockCD(FALSE);
	}
	
	if(g_sSettings.GetEject() && !(m_eMode & LFE_AUTO_MODE)){

		CR_EjectCD(TRUE);
	}

	return TRUE;
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

	CString tmp;

	if(g_sSettings.GetExitLameFE() || g_sSettings.GetShutdown()){
		
		CTrayDialog::EndDialog (IDOK);
		return;
	}

	CString text;
	text = g_iLang.GetString(IDS_ENC_CLOSE_BTN);
	m_closeBtn.SetWindowText(text);
	m_saveBtn.EnableWindow(TRUE);
	m_strDiscardLogMsg = g_iLang.GetString(IDS_ENC_DISCARD_LOG);
	m_strEstSize.Empty();
	m_strInputSize.Empty();
	m_strIn.Empty();
	m_strOut.Empty();
	m_nFilePerc = 0;
	m_nBufferPerc = 0;
	m_nJitterPos = 0;
	m_strElaTime.Empty();
	m_strEstTime.Empty();
	m_strRemTime.Empty();

	PostMessage(WM_TIMER, 0, 0);
/*	
	if((m_mEMode == BATCHALBUMMODE) || (m_mEMode == BATCHSINGLETRACKMODE)){

		TRACE0("Leave finished jobs here as BATCHMODE timed out");
		return;
	}

	TRACE("Starting finished Jobs\n");
	if(m_nErrors != 0){

		int nResult = AfxMessageBox(g_iLang.GetString(IDS_ENC_VIEW_LOG_ERR), MB_YESNO + MB_ICONQUESTION);
		
		if(nResult == IDYES){
			
			m_lLogFile.View();
		}
	}
	else{

		//check what to be done after encoding

		if (g_sSettings.GetBeep()){  //"beep"

			
			Beep(1000,800);
		}

		if(g_sSettings.GetPlaySound()){

			MessageBeep(MB_ICONSTOP);
		}
		
		if (g_sSettings.GetFocus()){

			OnStRestore();
		}

		if(g_sSettings.GetDialog() == TRUE){ //show finished "dialog"
			

			int nResult = AfxMessageBox(g_iLang.GetString(IDS_ENC_VIEW_LOG), MB_YESNO + MB_ICONQUESTION);
			
			if(nResult == IDYES){

				m_lLogFile.View();
			}
		}				
		
		if (g_sSettings.GetM3U() == TRUE){  //create "m3u"-""play""list with encoded files

			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_pCD){

				playlist = new CPlayList(m_pCD);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(g_sSettings.GetWorkingDir(), m_strExtension, FALSE, m_mEMode)){

			}
			else{

			}

			//delete playlist;
			PostMessage(WM_TIMER,0,0);
		}		

		if (g_sSettings.GetPlayBack() == TRUE){ //play encoded files



			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_pCD){

				playlist = new CPlayList(m_pCD);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(g_sSettings.GetWorkingDir(), m_strExtension, TRUE,  m_mEMode)){

			}
			else{

			}

			tmp = g_sSettings.GetExternalPlayer();

			if(tmp != "[System default]"){  // use specified player

				if(g_sSettings.GetEnqueue()){

					tmp += " /ADD ";
				}

				WinExec(tmp + " \"" + g_sSettings.GetWorkingDir() + "\\lameFE.m3u\"", SW_SHOW);
			}
			else{

				ShellExecute(GetSafeHwnd(), 
								"open", 
								"\"" + g_sSettings.GetWorkingDir() + "\\lameFE.m3u\"",
								NULL,
								NULL,
								SW_SHOW
						);
			}

							
		}
	}

	PostMessage(WM_TIMER, 0, 0);*/
}



void CEncodingStatusDlg::OnLogSave()
{


	CFileDialog dlg(FALSE, 0, 0, OFN_OVERWRITEPROMPT, "Textfile (*.txt) |*.txt|", 0);
	
	if(dlg.DoModal() == IDOK){

		if(m_lLogFile.SaveLogAs(dlg.GetPathName() + ".txt")){

			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_LOGSAVEDSUCCESS), dlg.GetPathName() + ".txt");
			AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
		}
	}
}


void CEncodingStatusDlg::EnableCDControls(BOOL bEnable)
{

	GetDlgItem(IDC_JITTERSTATUS)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_MAX)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_MIN)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_NOMINAL)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_READRECT)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_FULL)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_EMPTY)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_BUFFERSTATUS)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_STATIC_JITTERSTATUS)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
	GetDlgItem(IDC_LIST_STATUS_BAR)->ShowWindow((bEnable ? SW_SHOW : SW_HIDE));
}

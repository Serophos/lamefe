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

#include "CDPlayerIni.h"
#include "CDdbQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID    2

extern CSettings g_sSettings;


//////////////////////////////////////////////////////////////////////////////////
//
// Winamp Output plugin emulation. Poor, poor C with global variables and
// all this stuuf I hate, yuck :-(
// 2003-04-27: Grrr, I think I should clean a bit up
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
	m_bBatchAppendDiscID = FALSE;
}


void CEncodingStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncodingStatusDlg)
	DDX_Control(pDX, IDC_JITTERSTATUS, m_jitterPos);
	DDX_Control(pDX, IDCANCEL, m_closeBtn);
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
	ON_COMMAND(IDC_SAVE_LOG, OnLogSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEncodingStatusDlg 

BOOL CEncodingStatusDlg::OnInitDialog()
{

	CTrayDialog::OnInitDialog();
	
	
	SetTimer(TIMERID, 1000, NULL);
	
	TraySetIcon(IDR_MAINFRAME);
	TraySetToolTip("LameFE");
	TraySetMenu(IDR_SYSTRAY_MENU);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	SetFocus();
	SetForegroundWindow();

	CString strTmp;
	strTmp.LoadString(IDS_LOG_COLUMNCAPTION);
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

	// Only set thread priority if we are on Windows NT and using lame
	// to be on the save side
	if(Utils::IsWindowsNT() && (m_strOutputDevice == "lame_enc.dll")){

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
	}

	m_pThread = AfxBeginThread(EncoderFunc, (void*)this, nThreadPriority);

	return TRUE;
}

void CEncodingStatusDlg::OnTimer(UINT nIDEvent)
{

	m_mLockControls.Lock();
	
	m_fileStatus.SetPos(m_nFilePerc);
	m_listStatus.SetPos(m_nBufferPerc);
	m_strStatusText.Format("Status (%02d%%)", m_nFilePerc);

	if(m_nJob != ANY_TO_ENCODER){

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
	SetForegroundWindow();
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



	ID3_Tag			id3Tag;
	ID3_Frame		id3Frame;
	ID3_Frame		*buffer = NULL;
	unsigned short	sFlags  = 0;
	BOOL			bV1     = FALSE;
	BOOL			bV2		= FALSE;

	bV1	= g_sSettings.GetId3v1();
	bV2 = g_sSettings.GetId3v2();
	
	if(bV1 && bV2){

		sFlags = ID3TT_ID3V1 || ID3TT_ID3V2;
	}
	else if(bV1 & !bV2){

		sFlags = ID3TT_ID3V1;
	}
	else{

		sFlags = ID3TT_ID3V2;
	}

	id3Tag.Link(m_out, sFlags);

	if(bV1 || bV2){


		if(tmpAI.song != "" && tmpAI.song != 0){

			ID3_AddTitle(&id3Tag, tmpAI.song, true);
		}

		//artist
		if(tmpAI.artist != "" && tmpAI.artist != 0){

			ID3_AddArtist(&id3Tag, tmpAI.artist, true);
		}

		//album
		if(tmpAI.album != "" && tmpAI.album != 0){

			ID3_AddAlbum(&id3Tag, tmpAI.album, true);
		}

		//comment
		if(tmpAI.comment != "" && tmpAI.comment != 0){

			ID3_AddComment(&id3Tag, tmpAI.comment, "", true);
		}

		//year
	 	char *_year = new char[4];
		//unsigned char *_track = new unsigned char[4];

		if(tmpAI.year != 0){
	
			itoa(tmpAI.year, _year, 10);
			ID3_AddYear(&id3Tag, _year, true);
		}

		//track
		if(tmpAI.track != 0){

 			//itoa(tmpAI.track, (char*)_track, 10);
			ID3_AddTrack(&id3Tag, tmpAI.track, 1, true);
		}

		if(tmpAI.genre != "" && tmpAI.genre != 0){

			ID3_AddGenre(&id3Tag, (char*)tmpAI.genre, true);
		}
		
		if(bV1){

			id3Tag.Update(ID3TT_ID3V1);
		}
		if(bV2){

			id3Tag.Update(ID3TT_ID3V2);
		}

		//delete _track;
		delete _year;
	}
	
/*	if(cfg.GetValue("L.A.M.E.", "WriteTLENtag", FALSE)){


	}

*/
	PostMessage(WM_TIMER,0,0);
 
	return TRUE;
}

void CEncodingStatusDlg::SetJob(int nJob, CString strOutputDevice /* = "lame_enc.dll" */, modes mEMode  /* = NORMALMODE */)
{
	
	m_nJob			  = nJob;
	m_strOutputDevice = strOutputDevice;
	m_mEMode		  = mEMode;

	if(m_strOutputDevice == "lame_enc.dll"){
		
		m_strExtension = ".mp3";
	}
	else{

		COutPlugin tmp(m_strWd + "\\Plugins\\" + m_strOutputDevice, g_sSettings.GetIniFilename());
		tmp.Load();
		m_strExtension = tmp.GetExtension();
		tmp.Unload();
	}

}

UINT CEncodingStatusDlg::EncoderFunc(PVOID pParams)
{
	
	CEncodingStatusDlg *esdlg = (CEncodingStatusDlg *)pParams;
	
	switch(esdlg->GetJob()){

	//case RIP_TO_MP3:
	case RIP_TO_ENCODER:
		if(esdlg->GetAlbumMode() == ALBUMMODE){

			esdlg->RipToSingleFile();
		}
		else if((esdlg->GetAlbumMode() == BATCHALBUMMODE) || (esdlg->GetAlbumMode() == BATCHSINGLETRACKMODE)){

			esdlg->RipBatchMode();
		}
		else{
			
			esdlg->RipToAny();
		}
		break;
	case ANY_TO_ENCODER:
		esdlg->AnyToEncoder();
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

BOOL CEncodingStatusDlg::AnyToEncoder()
{

	CString	  strPlugin = "";

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
		
		if(!CLameFEPlugin::FindPlugin(m_in.Right(3), m_strInputDevice, m_strWd + "\\Plugins")){

			CWinampPlugin::FindPlugin(m_in.Right(3), m_strInputDevice, g_sSettings.GetWinampPluginPath());
		}


		m_strInputDevice = m_strInputDevice.Right(m_strInputDevice.GetLength() - m_strInputDevice.ReverseFind('\\') - 1);
		m_lLogFile.StartEntry(m_in, m_out, m_strInputDevice);
		if(Utils::FileExists(m_out) && !g_sSettings.GetSilentMode()){

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

		// Look for a LameFE plugin to handle this file ...
		if(CLameFEPlugin::FindPlugin(m_in.Right(3), strPlugin, m_strWd + "\\Plugins")){
		
			m_strInputDevice = strPlugin;
			LameFEPlugin2MP3(strPlugin, mFile, i);
			if(m_bAbortEnc){

				return FALSE;
			}
		}
		// ... check for a Winamp plugin afterwards if unsuccessfull
		else if(CWinampPlugin::FindPlugin(m_in.Right(3), strPlugin, g_sSettings.GetWinampPluginPath())){

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
	
	return TRUE;
}


BOOL CEncodingStatusDlg::WinampPlugin2Encoder(CString plugin, CMultimediaFile *mFile, int nPos)
{

	CWinampPlugin		wplugin(plugin);
	In_Module*			inMod;
	Out_Module			outMod;
	signed __int64		numsamples;
	char*				infilename = m_in.GetBuffer(10);
	m_in.ReleaseBuffer();


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
	
	mFile->GetWfx()->wBitsPerSample = g_bitspersamp;
	mFile->GetWfx()->nSamplesPerSec = g_samplerate;
	mFile->GetWfx()->nChannels		= g_numchannels;
	
	m_lLogFile.SetInFormat(mFile->GetWfx());

	if(g_bitspersamp != 16){

		TRACE("Invalid bitrate\n");
		m_list_errors.Format(IDS_INVBITRATE, g_bitspersamp);
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}

	if(!e.PrepareEncoding(mFile->GetSaveAs(m_strWd, m_strExtension), g_numchannels, g_samplerate, g_bitspersamp)){
		
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

	float  in_size		= (float)numsamples * g_numchannels * g_bitspersamp;
	float  mp3_est_size = (float)e.GetEstimatedSize(g_samplerate, g_numchannels, g_bitspersamp, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_inputSize.Format("%2.2f", in_size);
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
	inModule->GetProfileString = CLameFEPlugin::GetProfileString;
	inModule->SetProfileString = CLameFEPlugin::SetProfileString;

	inModule->Init();
	if(inModule->Open(mFile->GetFileName()) != 0){

		TRACE("Failed opening file\n");
		m_list_errors.Format(IDS_FILEOPENERR, mFile->GetFileName());
		m_lLogFile.SetErrorMsg(nPos, m_list_errors);
		m_nErrors++;
		return FALSE;
	}


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
	if(!e.PrepareEncoding(mFile->GetSaveAs(m_strWd, m_strExtension), mmFormat.nChannels, mmFormat.dwSampleRate, mmFormat.nBitsPerSample)){
		
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


	double mp3_est_size = e.GetEstimatedSize(mmFormat.dwSampleRate, mmFormat.nChannels, mmFormat.nBitsPerSample, in_size);

	in_size		 /= (1024.0*1024.0);	//in MB
	mp3_est_size /= (1024.0*1024.0);	//in MB

	m_inputSize.Format("%2.2f", in_size);
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

	return TRUE;
}

BOOL CEncodingStatusDlg::RipBatchMode()
{

	TRACE("Entering CEncodingStatusDlg::RipBatchMode()\n");


	BOOL bTimesOut	 =  g_sSettings.GetBatchTimesOut();
	BOOL bTimedOut	 =  FALSE;
	long nTimeOutSec =  0;
	int nNumCDDrives =  CR_GetNumCDROM();
	int nOrgActCD	 =  CR_GetActiveCDROM();
	int nActiveCD	 =  nOrgActCD;
	CString strDrive =  "";
	CDROMPARAMS cdParams;
	strcpy(cdParams.lpszCDROMID, "");

	while(!bTimedOut && !m_bAbortEnc){
		
		nTimeOutSec =  g_sSettings.GetBatchTimesOutAfterMin() * 60;

		//OK is this an audio cd?
		if(m_cd->GetNumAudioTracks() != 0){

			if(m_mEMode == BATCHSINGLETRACKMODE){

				RipToAny();
			}
			else{

				RipToSingleFile();
			}
			if(g_sSettings.GetM3U()  && !m_bAbortEnc){

				CPlayList playlist(m_cd);
				playlist.WriteToDisc(m_strWd, m_strExtension, FALSE, (m_mEMode == BATCHSINGLETRACKMODE));
			}

		}

		if(m_bAbortEnc){
			
			return FALSE;
		}

		//Reset status stuff
		strDrive.Format(IDS_BATCHNEXTCD, cdParams.lpszCDROMID);
		m_lLogFile.SetNotificationMessage(strDrive);
		m_fileStatus.SetRange(0, 100);
		m_fileStatus.SetPos(0);
		m_listStatus.SetRange(0,100);
		m_listStatus.SetPos(0);
		m_nErrors = 0;
		m_in.Empty();
		m_out.Empty();
		m_nBufferPerc = 0;
		m_nJitterPos = 0;
		m_nFilePerc = 0;
		m_inputSize.Empty();
		m_estSize.Empty();
		m_bResetTimer = TRUE;
		PostMessage(WM_TIMER,0,0);

		//Unlock drive and eject cd
		CR_LockCD(FALSE);
		CR_EjectCD(TRUE);
		m_cd->Eject();
		m_bBatchAppendDiscID = FALSE;
		
		if(g_sSettings.GetBatchBeepOnFinished()){

			Beep(1000,800);
		}


		if(g_sSettings.GetBatchUseAllDrives()){

			//switch to next drive 
			//CR_SaveSettings();
			CR_SetActiveCDROM((nActiveCD+1 < nNumCDDrives ? ++nActiveCD : (nActiveCD = 0)));
			CR_GetCDROMParameters(&cdParams);
			strDrive.Format(IDS_BATCHCDDRIVE, cdParams.lpszCDROMID);
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
					m_lLogFile.SetNotificationMessage(IDS_BATCHTIMEDOUT);
					bTimedOut = TRUE;
					return FALSE;
				}
			}			
			CR_IsMediaLoaded(cdMediaStatus);
		}
		
		if(m_bAbortEnc){
			
			return FALSE;
		}



		BOOL bSuccess = FALSE;
		m_cd->Init();

		if(m_cd->GetNumAudioTracks() == 0){

			TRACE("No AudioCD\n");
			m_lLogFile.SetNotificationMessage(IDS_MAIN_NOAUDIOCD);
			continue;
		}

		CCDPlayerIni cdp(m_cd);
		
		bSuccess = cdp.Read();
		
		if(!bSuccess){

			bSuccess = m_cd->ReadCDText();
		}

		if(!bSuccess && g_sSettings.GetBatchFreeDB()){
			
			CCDdbQueryDlg cddbDlg(this, m_cd, CR_GetActiveCDROM(), m_strWd, TRUE);
			
			int nResult = cddbDlg.DoModal();
			if(nResult == IDCANCEL){

				//We query again, because sometimes the server just times out
				nResult = cddbDlg.DoModal();
			}
			if(nResult == IDCANCEL){
				
				// Enough tries, no album info is available, so append disc id
				TRACE("No Albuminformation available!\n");
				if(g_sSettings.GetBatchAppendDiscID()){

					m_bBatchAppendDiscID = TRUE;
				}
			}
			if(nResult == IDOK){

				bSuccess = TRUE;
			}
		}
		
		if(bSuccess){

			m_bBatchAppendDiscID = FALSE;
		}

		// Select All Tracks for ripping
		int iAudioCnt = 0, iCnt = 0, j = 0;
		int *pAudioVerified;
		iCnt = m_cd->GetNumTracks();

		iAudioCnt	   = m_cd->GetNumAudioTracks();
		pAudioVerified = new int[iAudioCnt];
		
		for(int i = 0; i < iCnt; i++){

			if(m_cd->GetCDTrack(i)->IsAudioTrack()){

				TRACE("Batchmode: Track %d is an Audiotrack and has been selected\n", i+1);
				pAudioVerified[j++] = i;
			}
		}
		m_cd->SetSelItems(iAudioCnt, pAudioVerified);
		
		delete pAudioVerified;

	} // end main loop
	
	TRACE("Leaving CEncodingStatusDlg::RipBatchMode()\n");
	return TRUE;
}

BOOL CEncodingStatusDlg::RipToSingleFile(){

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
	int	 	 numTracks    = m_cd->GetNumSelTracks();

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
	m_cd->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_in.Format("Audio CD %s", m_cd->GetStrVolID());
	m_out = m_cd->GetAlbumString(m_strWd, m_strExtension, m_bBatchAppendDiscID);

	m_lLogFile.StartSession(1, m_strOutputDevice);
	m_lLogFile.StartEntry(m_in, m_out);
	
	WAVEFORMATEX wfx;
	wfx.nChannels	    = 2;
	wfx.nSamplesPerSec  = 44100;
	wfx.wBitsPerSample	= 16;
	m_lLogFile.SetInFormat(&wfx);

	// Check for existing file
	TRACE("Checking for existing file\n");

	if(Utils::FileExists(m_out) && !g_sSettings.GetSilentMode()){

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

	nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath()) / (1024.0*1024.0);

	PostMessage(WM_TIMER,0,0);

	if((mp3_est_size)>= nFreeDiskSpace){

		m_mLockControls.Lock();
		m_list_errors.Format("Not enoug free disk space on drive %s).\nAborting Encoding process", g_sSettings.GetBasePath());
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
		CEncoderFeeder ef(&e, dwSampleBufferSize, nNumBuffers, m_bAbortEnc);
		
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

		if((g_sSettings.GetId3v1() || g_sSettings.GetId3v2()) && m_strOutputDevice == "lame_enc.dll"){
	
			if(!WriteID3Tag(tmpAI)){

				m_lLogFile.SetErrorMsg(0, IDS_ENC_ID3FAILED);
				m_nErrors++;
			}

		}
		if(g_sSettings.GetWriteCue()){

			m_cd->WriteCueSheet(m_strWd, m_strExtension);
		}
	}
	
	m_lLogFile.FinalizeEntry();
	m_lLogFile.FinalizeSession();

	delete pSel;

	if(g_sSettings.GetLock() == TRUE){
		
		CR_LockCD(FALSE);
	}
	
	if(g_sSettings.GetEject() && (m_mEMode != BATCHALBUMMODE)){

		CR_EjectCD(TRUE);
	}
	return TRUE;
}		


BOOL CEncodingStatusDlg::RipToAny()
{
	
	TRACE("RipToAny: Mode is %d (0 = Output Wave 1 = Output MP3) Output = %s\n", m_nJob, m_strOutputDevice);

	
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
	int	 	 numTracks    = m_cd->GetNumSelTracks();

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
	m_cd->GetSelItems(numTracks, pSel);

	DWORD   dwRead     = 0;
	
	m_lLogFile.StartSession(numTracks, "CDRip.dll");

	while(currentTrack < numTracks && !m_bAbortEnc){
	
		cdTrack = m_cd->GetCDTrack(pSel[currentTrack]);
		m_out = m_cd->GetSaveAs(cdTrack->m_btTrack, m_strWd, m_strExtension, m_bBatchAppendDiscID);
	
		if(!cdTrack->IsAudioTrack()){
			
			TRACE("Shouldn't have happened :(\n");
			continue;
		}

		// Check for existing file
		TRACE("Checking for existing file\n");
		m_lLogFile.StartEntry(cdTrack->GetTrackname(), m_out, m_strOutputDevice);
		if(Utils::FileExists(m_out) && !g_sSettings.GetSilentMode()){

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
		nFreeDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath()) / (1024.0*1024.0);
		PostMessage(WM_TIMER,0,0);
		if((mp3_est_size)>= nFreeDiskSpace){

			m_mLockControls.Lock();
			m_list_errors.Format("Not enoug free disk space on drive %s).\nAborting Encoding process",
				g_sSettings.GetBasePath().Left(2));
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
			CEncoderFeeder ef(&e, dwSampleBufferSize, nNumBuffers, m_bAbortEnc);
			
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

			if((g_sSettings.GetId3v1() || g_sSettings.GetId3v2()) && m_strOutputDevice == "lame_enc.dll"){
		
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

	if(g_sSettings.GetLock()){
		
		CR_LockCD(FALSE);
	}
	
	if(g_sSettings.GetEject()){

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

	CString tmp;

	if(g_sSettings.GetExitLameFE() || g_sSettings.GetShutdown()){
		
		CTrayDialog::EndDialog (IDOK);
		return;
	}

	CString text;
	text.LoadString(IDS_ENC_CLOSE_BTN);
	m_closeBtn.SetWindowText(text);
	m_saveBtn.EnableWindow(TRUE);
	m_strDiscardLogMsg.LoadString(IDS_ENC_DISCARD_LOG);
	m_estSize.Empty();
	m_inputSize.Empty();
	m_in.Empty();
	m_out.Empty();
	m_nFilePerc = 0;
	m_nBufferPerc = 0;
	m_nJitterPos = 0;
	m_strElaTime.Empty();
	m_strEstTime.Empty();
	m_strRemTime.Empty();

	PostMessage(WM_TIMER, 0, 0);
	
	if((m_mEMode == BATCHALBUMMODE) || (m_mEMode == BATCHSINGLETRACKMODE)){

		TRACE0("Leave finished jobs here as BATCHMODE timed out");
		return;
	}

	TRACE("Starting finished Jobs\n");
	if(m_nErrors != 0){

		int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG_ERR, MB_YESNO + MB_ICONQUESTION);
		
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
			

			int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG, MB_YESNO + MB_ICONQUESTION);
			
			if(nResult == IDYES){

				m_lLogFile.View();
			}
		}				
		
		if (g_sSettings.GetM3U() == TRUE){  //create "m3u"-""play""list with encoded files

			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_cd){

				playlist = new CPlayList(m_cd);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(m_strWd, m_strExtension, FALSE, m_mEMode)){

			}
			else{

			}

			//delete playlist;
			PostMessage(WM_TIMER,0,0);
		}		

		if (g_sSettings.GetPlayBack() == TRUE){ //play encoded files



			PostMessage(WM_TIMER,0,0);

			CPlayList *playlist = NULL;
			
			if(m_cd){

				playlist = new CPlayList(m_cd);
			}
			else{

				playlist = new CPlayList(m_files);
			}

			if(playlist->WriteToDisc(m_strWd, m_strExtension, TRUE,  m_mEMode)){

			}
			else{

			}

			tmp = g_sSettings.GetExternalPlayer();

			if(tmp != "[System default]"){  // use specified player

				if(g_sSettings.GetEnqueue()){

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



void CEncodingStatusDlg::OnLogSave()
{


	CFileDialog dlg(FALSE, 0, 0, OFN_OVERWRITEPROMPT, "Textfile (*.txt) |*.txt|", 0);
	
	if(dlg.DoModal() == IDOK){

		if(m_lLogFile.SaveLogAs(dlg.GetPathName() + ".txt")){

			CString strTmp;
			strTmp.Format(IDS_LOGSAVEDSUCCESS, dlg.GetPathName() + ".txt");
			AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
		}
	}
}


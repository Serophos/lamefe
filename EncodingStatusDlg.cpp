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
#include "TimeStatus.h"
#include "EncodingStatusDlg.h"
#include "Encoder.h"
#include "EncoderFeeder.h"
#include "cfgFile.h"
#include "tagWriter/tagWriter.h"

#include <stdio.h>
#include <direct.h>
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define TIMERID    2

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld EncodingStatusDlg 


EncodingStatusDlg::EncodingStatusDlg(CWnd* pParent /*=NULL*/)
	: CTrayDialog(EncodingStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EncodingStatusDlg)
	m_estSize = _T("");
	m_list_errors = _T("");
	m_fileMsg = _T("");
	m_fileXofY = _T("");
	m_listMsg = _T("");
	m_inputSize = _T("");
	m_in = _T("");
	m_out = _T("");
	m_strEstTime = _T("");
	m_strElaTime = _T("");
	m_strRemTime = _T("");
	//}}AFX_DATA_INIT

	errors = 0;
	m_bAbortEnc = FALSE;

}


void EncodingStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EncodingStatusDlg)
	DDX_Control(pDX, IDC_JITTERSTATUS, m_jitterPos);
	DDX_Control(pDX, IDCANCEL, m_closeBtn);
	DDX_Control(pDX, IDC_LIST_STATUS_BAR, m_listStatus);
	DDX_Control(pDX, IDC_FILE_STATUS_BAR, m_fileStatus);
	DDX_Text(pDX, IDC_EST_SIZE, m_estSize);
	DDX_Text(pDX, IDC_ERRORS, m_list_errors);
	DDX_Text(pDX, IDC_FILE_MSG, m_fileMsg);
	DDX_Text(pDX, IDC_FILE_X_OF_Y, m_fileXofY);
	DDX_Text(pDX, IDC_LIST_MSG, m_listMsg);
	DDX_Text(pDX, IDC_IN_SIZE, m_inputSize);
	DDX_Text(pDX, IDC_IN, m_in);
	DDX_Text(pDX, IDC_OUT, m_out);
	DDX_Text(pDX, IDC_EST_TIME, m_strEstTime);
	DDX_Text(pDX, IDC_ELA_TIME, m_strElaTime);
	DDX_Text(pDX, IDC_REM_TIME, m_strRemTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EncodingStatusDlg, CTrayDialog)
	//{{AFX_MSG_MAP(EncodingStatusDlg)
	ON_WM_TIMER()
	ON_COMMAND(IDC_ST_RESTORE, OnStRestore)
	ON_COMMAND(IDC_ST_EXIT, OnStExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten EncodingStatusDlg 

BOOL EncodingStatusDlg::OnInitDialog()
{

	CTrayDialog::OnInitDialog();

	SetTimer(TIMERID, 1000, NULL);
	
	m_pThread = NULL;
	
	m_eThreadFinished.ResetEvent();
	// Fire of thread
	m_pThread = AfxBeginThread( encoderFunc, (void*)this);
	
	TraySetIcon(IDR_MAINFRAME);
	TraySetToolTip("lameFE 2.0");
	TraySetMenu(IDR_SYSTRAY_MENU);

	return true;
}

void EncodingStatusDlg::OnTimer(UINT nIDEvent)
{

	m_mLockControls.Lock();
	
	m_fileStatus.SetPos(filePerc);
	m_listStatus.SetPos(bufferPerc);

	if(job != WAVE_TO_MP3){

		m_jitterPos.SetPos(jitterPos);
	}

	if(m_bResetTimer){

		m_tTimeStatus.ReInit();
		m_bResetTimer = FALSE;
	}

	//Calcualte estTime
	m_tTimeStatus.Calculate(filePerc);
	m_strEstTime = m_tTimeStatus.GetEstimateString();
	m_strElaTime = m_tTimeStatus.GetElapsedString();
	m_strRemTime = m_tTimeStatus.GetRemainingString();

	UpdateData(FALSE);
	m_mLockControls.Unlock();
}

void EncodingStatusDlg::OnCancel() 
{
		
	m_bAbortEnc = TRUE;
	
	//wait for finished Thread Event
	WaitForSingleObject(m_eThreadFinished, INFINITE);
	
	CR_LockCD(FALSE);
	KillTimer(TIMERID);
	MessageBeep(MB_ICONEXCLAMATION);

	CTrayDialog::OnCancel();
}

void EncodingStatusDlg::OnStRestore() 
{
	TRACE("OnStRestore()\n");
	if(TrayIsVisible()){

		if(TrayHide()){

			this->ShowWindow(SW_SHOW);
		}
	}
}

void EncodingStatusDlg::OnStExit() 
{
	
	TRACE("OnStExit()\n");
	OnStRestore();
	OnCancel();
}

//Init funcs
void EncodingStatusDlg::parseFiles(CPtrArray* files, CString wdir, int preset)
{

	m_files = files;
	wd      = wdir;
	m_preset = preset;
}


//encoding Functions
BOOL EncodingStatusDlg::writeID3Tag(MP3File *file)
{	

	m_fileMsg.LoadString(IDS_ENC_WRITEID3);

	PostMessage(WM_TIMER,0,0);

	if(!TW_InitTagRenderer(file->getSaveAs())){

		file->setErrorString(IDS_ENC_ID3FAILED);
		return false;
	}

	char track;
	char year;
	
	
	itoa(file->getID3Info()->getTrack(), &track, 10);
	itoa(file->getID3Info()->getYear(), &year, 10);
	
	if(!TW_RenderTags(file->getID3Info()->getSong(),
			file->getID3Info()->getArtist(), 
			file->getID3Info()->getAlbum(), 
			file->getID3Info()->getComment(), 
			file->getID3Info()->getGenre(), 
			&year, 
			&track)){

		file->setErrorString(IDS_ENC_ID3INTERR);
		return false;
	}
	
	PostMessage(WM_TIMER,0,0);

	return false;
}

void EncodingStatusDlg::viewLog()
{

	CStdioFile log;
	CFileException e;
	TRY{
		
		CString tmp;
		log.Open(wd + "\\lameFE.log", CFile::modeCreate | CFile::modeWrite | CFile::typeText, &e);
		
#ifdef _DEBUG
		tmp.Format(
			"lameFE 2.0 Debug Release\nOutputdir=%s\nMode=%d\nError(s):%d\n\n",
			wd, job, errors
			);
#endif

		for(int i = 0; i < m_files->GetSize(); i++){
			
			tmp.Format(IDS_ENC_LOGENTRY,
				((MP3File *)m_files->GetAt(i))->getFileName(),
				((MP3File *)m_files->GetAt(i))->getSaveAs(),
				((MP3File *)m_files->GetAt(i))->getWfx()->nSamplesPerSec,
				((MP3File *)m_files->GetAt(i))->getWfx()->wBitsPerSample,
				((MP3File *)m_files->GetAt(i))->getWfx()->nChannels,
				((MP3File *)m_files->GetAt(i))->getError());



			log.WriteString(tmp);
		}
		log.Flush();
		log.Close();
	}
	CATCH(CFileException, e){

		//CString str
		AfxMessageBox(IDS_ENC_LOGFAILED ,MB_OK+MB_ICONSTOP,0);
	}END_CATCH

	WinExec("notepad.exe " + wd + "\\lameFE.log", SW_SHOW);
}

void EncodingStatusDlg::setJob(int ajob)
{
	
	job = ajob;
}

UINT EncodingStatusDlg::encoderFunc(PVOID pParams)
{
	
	EncodingStatusDlg *esdlg = (EncodingStatusDlg *)pParams;
	
	switch(esdlg->GetJob()){

	case RIP_TO_MP3:
		esdlg->RipToMP3();
		break;
	case RIP_TO_WAVE:
		esdlg->RipToWave();
		break;
	case WAVE_TO_MP3:
		esdlg->WaveToMP3();
		break;
	}

	// Indicate thread status
	esdlg->m_eThreadFinished.SetEvent();
	esdlg->m_pThread = NULL;
	
	esdlg->finishedJobs();

	return 0;
}

BOOL EncodingStatusDlg::WaveToMP3()
{


	cfgFile cfg(wd);

	//setup controls
	//m_mLockControls.Lock();
	m_listMsg.LoadString(IDS_ENC_PREPARING);
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	PostMessage(WM_TIMER,0,0);
	m_jitterPos.EnableWindow(FALSE);
	//m_mLockControls.Unlock();

	MP3File *file;

	FILE*	pFileIn	   = NULL;
	PSHORT	pWAVBuffer = NULL;
	DWORD   dwRead     = 0;
	DWORD   dwFileSize;
	DWORD   dwDone;
	
	CString a;

	for(int i = 0; i < m_files->GetSize(); i++){
	
		file = (MP3File*)m_files->GetAt(i);

		if(FileExists(file->getSaveAs()) && !cfg.GetValue(SILENT)){

			CString out;
			out.Format(IDS_ENC_FILEEXISTS, file->getSaveAs());
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				file->setErrorString(IDS_ENC_DENIEDOVWR);
				continue;
			}
		}
		if(!createDirs(file->getSaveAs())){
				
			CString msg;
			msg.LoadString(IDS_ENC_ERRDIRCREATE);
			AfxMessageBox(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum(), 
				MB_ICONSTOP+MB_OK);
			errors++;
			file->setErrorString(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum());
			continue;

		}
//		}

		if(!file->isValidateWave()){

			errors++;
			PostMessage(WM_TIMER,0,0);
			m_mLockControls.Unlock();
			continue;
		}
		dwRead = 0;
		dwDone = 0;

		pFileIn = fopen(file->getFileName().GetBuffer(10), "rb");
		file->getFileName().ReleaseBuffer();
		if(!pFileIn){

			TRACE("\nUUPS\n");
			continue;
		}
		dwFileSize = file->getFileSize();

		float est_mp3_size = 0;

		
		m_mLockControls.Lock();
		m_in = file->getFileName();
		m_out = file->getSaveAs();
		m_inputSize.Format("%.0f", file->getFileSize());
		m_fileMsg.LoadString(IDS_ENC_WAVE_ENC);
		m_listMsg.LoadString(IDS_ENC_WAVE_FILES);

		m_fileXofY.Format(IDS_ENC_FILE_XOFY, i + 1, m_files->GetSize());
		filePerc = 0;
		m_bResetTimer = TRUE;
		m_mLockControls.Unlock();

		CString tip;
		tip.LoadString(IDS_ENC_TOOLTIP);
		TraySetToolTip(tip + file->getFileShort());
		
		Encoder e(wd);
		e.setOutputFormat(OUTPUT_MP3);
		e.init();
		if(!e.prepareEncoding(file)){
			
			file->setErrorString(IDS_ENC_INITERR);
			errors++;
			continue;
		}
	
		m_mLockControls.Lock();
		est_mp3_size = e.getEstimatedSize(file->getWfx()->nSamplesPerSec, file->getWfx()->nChannels, file->getWfx()->wBitsPerSample, file->getFileSize());
		m_estSize.Format("%.0f", est_mp3_size/1024); 
		PostMessage(WM_TIMER,0,0);
		m_mLockControls.Unlock();

		pWAVBuffer = new SHORT[e.getSamplesToRead()];
		
		while ((dwRead = fread(pWAVBuffer, sizeof(SHORT), e.getSamplesToRead(), pFileIn)) > 0)
		{

			e.passBuffer((LPVOID)pWAVBuffer, dwRead);

			dwDone += dwRead * sizeof(SHORT);
			
			filePerc = (float)dwDone / (float)dwFileSize * 100;
			bufferPerc = 100;
			
		} //File is done:-)

		e.deInit();
		
		delete pWAVBuffer;

		fclose(pFileIn);
		
		cfgFile cfg(wd);

		if(cfg.GetValue(ID3V2, FALSE)){
	
			writeID3Tag(file);
		}

		if(file->getError().IsEmpty()){

			file->setErrorString(IDS_ENC_SUCCESS);
		}
	}
	m_jitterPos.EnableWindow(TRUE);
	return true;
}

BOOL EncodingStatusDlg::RipToWave()
{
	
	cfgFile cfg(wd);

	if(cfg.GetValue(LOCK, FALSE)){

		CR_LockCD(TRUE);
	}
	//setup controls
	m_mLockControls.Lock();
	m_listMsg.LoadString(IDS_ENC_PREPARING);
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	//UpdateData(FALSE);
	PostMessage(WM_TIMER,0,0);
	m_mLockControls.Unlock();
	
	int     currentTrack = 0;
	LONG    nBufferSize;
	BOOL    bAbort       = FALSE;

	MP3File *file;


	while(currentTrack < m_files->GetSize()){
	
		file = (MP3File *)m_files->GetAt(currentTrack++);

		if(FileExists(file->getSaveAs()) && !cfg.GetValue(SILENT)){

			CString out;
			out.Format(IDS_ENC_FILEEXISTS, file->getSaveAs());
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				file->setErrorString(IDS_ENC_DENIEDOVWR);
				continue;
			}
		}
		if(!createDirs(file->getSaveAs())){
				
			CString msg;
			msg.LoadString(IDS_ENC_ERRDIRCREATE);
			AfxMessageBox(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum(), 
				MB_ICONSTOP+MB_OK);
			errors++;
			file->setErrorString(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum());
			continue;

		}

		m_mLockControls.Lock();
		CString tip;
		tip.LoadString(IDS_ENC_TOOLTIP);
		TraySetToolTip(tip + file->getFileShort());
		m_listMsg.LoadString(IDS_ENC_RIPPING);
		m_fileXofY.Format(IDS_ENC_TRACK_XOFY, currentTrack, m_files->GetSize());
		m_fileMsg.Format(IDS_ENC_EXTRACT_TRACK, file->getCDTrackNum());
		m_inputSize.Format("%l", file->getFileSize());
		m_estSize = m_inputSize;
		m_in.Format(file->getFileName());
		m_out.Format(file->getSaveAs());
		filePerc = 0;
		m_bResetTimer = TRUE;
		m_mLockControls.Unlock();
		
	
		if (CR_OpenRipper(&nBufferSize, file->getStartSect(), file->getEndSect()) == CDEX_OK){
			
			LONG nNumBytesRead;
			
			BYTE* pbtStream= new BYTE[nBufferSize];

			CDEX_ERR ripErr;

			Encoder e(wd); //= new Encoder(wd);
			e.setOutputFormat(OUTPUT_WAVE);
			e.init();
			e.prepareEncoding(file);

			EncoderFeeder ef(&e, nBufferSize, 256, m_bAbortEnc);

			// Read all chunks
			while((ripErr=CR_RipChunk(pbtStream,&nNumBytesRead, bAbort)) != CDEX_RIPPING_DONE){
				
				//Check for jitter errors
				if ( CDEX_JITTER_ERROR == ripErr )
				{
					DWORD dwStartSector,dwEndSector;

					// Get info where jitter error did occur
					CR_GetLastJitterErrorPosition(dwStartSector,dwEndSector);
					CString jErr;
					jErr.Format(IDS_ENC_JITTER_ERR, dwStartSector, dwEndSector);
					file->setErrorString(jErr);
					m_fileMsg = jErr;
					PostMessage(WM_TIMER,0,0);
				}

				if ( CDEX_ERROR == ripErr ){
					
					errors++;
					file->setErrorString(IDS_ENC_RIPPERROR);
					m_list_errors.Format(IDS_ENC_NUMERRORS, errors);
					PostMessage(WM_TIMER,0,0);
					break;
				}
				
				ef.addData((SHORT*)pbtStream, nNumBytesRead);
				filePerc = CR_GetPercentCompleted();
				jitterPos = CR_GetJitterPosition();
				bufferPerc = ef.GetBufferStatus();
			}

			ef.waitForFinished();
			e.deInit();

			if(file->getError().IsEmpty()){

				file->setErrorString(IDS_ENC_SUCCESS);
			}

			delete pbtStream;
			
			CR_CloseRipper();
		}
		else{

			return false;
		}

	}
	
	if(cfg.GetValue(LOCK, FALSE)){

		CR_LockCD(FALSE);
	}

	if(cfg.GetValue(EJECT, FALSE)){

		CR_EjectCD(TRUE);
	}

	return true;
}

BOOL EncodingStatusDlg::RipToMP3()
{
	
	cfgFile cfg(wd);
	
	if(cfg.GetValue(LOCK, FALSE)){

		CR_LockCD(TRUE);
	}

	//setup controls
	m_mLockControls.Lock();
	m_listMsg.LoadString(IDS_ENC_PREPARING);
	m_fileStatus.SetRange(0, 100);
	m_fileStatus.SetPos(0);
	m_listStatus.SetRange(0,100);
	m_listStatus.SetPos(0);
	PostMessage(WM_TIMER,0,0);
	m_mLockControls.Unlock();
	
	int     currentTrack = 0;
	LONG    nBufferSize;
	BOOL    bAbort       = FALSE;

	MP3File *file;

	FILE*	pFileIn	   = NULL;
	PSHORT	pWAVBuffer = NULL;
	DWORD   dwRead     = 0;

	while(currentTrack < m_files->GetSize() && !m_bAbortEnc){
	
		file = (MP3File *)m_files->GetAt(currentTrack++);

		if(FileExists(file->getSaveAs()) && !cfg.GetValue(SILENT)){

			CString out;
			out.Format(IDS_ENC_FILEEXISTS, file->getSaveAs());
			if(IDNO == AfxMessageBox(out, MB_YESNO+MB_ICONQUESTION)){

				file->setErrorString(IDS_ENC_DENIEDOVWR);
				continue;
			}
		}
		
		if(!createDirs(file->getSaveAs())){
				
			CString msg;
			msg.LoadString(IDS_ENC_ERRDIRCREATE);
			AfxMessageBox(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum(), 
				MB_ICONSTOP+MB_OK);
			errors++;
			file->setErrorString(msg + (cfg.GetStringValue(OUTPUT) == "" ? wd : cfg.GetStringValue(OUTPUT)) + "\\" + file->getID3Info()->getArtist() + "\\" + file->getID3Info()->getAlbum());
			continue;

		}

		Encoder e(wd);
		e.setOutputFormat(OUTPUT_MP3);
		e.init();
		e.prepareEncoding(file);
		unsigned long dwSampleBufferSize = e.getSamplesToRead();
		
		m_mLockControls.Lock();
		TraySetToolTip("lameFE 2.0 | Working on file " + file->getFileShort());
		m_listMsg.LoadString(IDS_ENC_RIPPING);
		m_fileXofY.Format(IDS_ENC_TRACK_XOFY, currentTrack, m_files->GetSize());
		m_fileMsg.Format(IDS_ENC_EXTRACT_TRACK, file->getCDTrackNum());
		m_inputSize.Format("%.0f", file->getFileSize() / 1024);
		
		float mp3_est_size = e.getEstimatedSize(file->getWfx()->nSamplesPerSec, file->getWfx()->nChannels, file->getWfx()->wBitsPerSample, file->getFileSize());
		m_estSize.Format("%.0f", mp3_est_size / 1024);

		m_in.Format(file->getFileName());
		m_out.Format(file->getSaveAs());

		filePerc = 0;
		m_bResetTimer = TRUE;
		m_mLockControls.Unlock();
		PostMessage(WM_TIMER,0,0);
		
		if (CR_OpenRipper(&nBufferSize, file->getStartSect(), file->getEndSect()) == CDEX_OK){
			
			LONG nNumBytesRead;
			LONG nOffset=0;
			
			BYTE* pbtStream= new BYTE[nBufferSize + dwSampleBufferSize * 2];

			CDEX_ERR ripErr;
			EncoderFeeder ef(&e, dwSampleBufferSize, 256, m_bAbortEnc);
			
			// Read all chunks
			while((ripErr=CR_RipChunk(pbtStream + nOffset, &nNumBytesRead, m_bAbortEnc)) != CDEX_RIPPING_DONE  && !m_bAbortEnc){
				
				SHORT*	psEncodeStream = (SHORT*)pbtStream;
				DWORD	dwSamplesToConvert = (nNumBytesRead + nOffset) / 2;

				//Check for jitter errors
				if (CDEX_JITTER_ERROR == ripErr)
				{
					DWORD dwStartSector, dwEndSector;

					// Get info where jitter error did occur
					CR_GetLastJitterErrorPosition(dwStartSector,dwEndSector);
					CString jErr;
					jErr.Format(IDS_ENC_JITTER_ERR, dwStartSector, dwEndSector);
					file->setErrorString(jErr);
					m_fileMsg = jErr;
					PostMessage(WM_TIMER,0,0);
				}

				if (CDEX_ERROR == ripErr){
					
					errors++;
					file->setErrorString("Error ripping audiotrack");
					
					//m_mLockControls.Lock();
					m_list_errors.Format(IDS_ENC_NUMERRORS, errors);
					PostMessage(WM_TIMER,0,0);
					//m_mLockControls.Unlock();
					break;
				}
				
				
				while (dwSamplesToConvert >= dwSampleBufferSize)
				{

					if(!ef.addData(psEncodeStream, dwSampleBufferSize)){
					
						return false; //this is fatal! Buffer error
					}

					dwSamplesToConvert -= dwSampleBufferSize;

					psEncodeStream += dwSampleBufferSize;
				}

				// Copy the remaing bytes up front, if necessary
				if (dwSamplesToConvert > 0){

					// Calculate the offset in bytes
					nOffset = dwSamplesToConvert * 2;

					// Copy up front
					memcpy(pbtStream, psEncodeStream, nOffset);
				}
				else{
					nOffset=0;
				}


				filePerc   = CR_GetPercentCompleted();
				jitterPos = CR_GetJitterPosition();
				bufferPerc = ef.GetBufferStatus();
			}

			//some bytes left in the buffer?
			if (nOffset && !m_bAbortEnc)
			{

				ef.addData((SHORT*)pbtStream, nOffset / 2);
			}

			bufferPerc = ef.GetBufferStatus();
			ef.waitForFinished();
			ef.deInit();
			delete pbtStream;
			CR_CloseRipper();
			e.deInit();
			
			if(cfg.GetValue(ID3V2, FALSE)){
		
				writeID3Tag(file);
			}
			if(file->getError().IsEmpty()){

				file->setErrorString("Encoded successfully");
			}
		}
		else{

			return false;
		}

	}

	if(cfg.GetValue(LOCK, FALSE)){
		
		CR_LockCD(FALSE);
	}
	
	if(cfg.GetValue(EJECT, FALSE)){

		CR_EjectCD(TRUE);
	}

	return true;
}

int EncodingStatusDlg::GetJob()
{

	return job;
}


void EncodingStatusDlg::finishedJobs()
{

	TRACE("finishedJobs()\n");

	if(m_bAbortEnc){

		return;
	}
	//Whe are done, reset Controls

	if(TrayIsVisible()){
		
		OnStRestore();
	}
	
	KillTimer(TIMERID);

	cfgFile lcfg(wd);
	CString tmp;

	if(lcfg.GetValue(QUIT, FALSE) || lcfg.GetValue(SHUTDOWN, FALSE)){
		
		CTrayDialog::EndDialog(IDOK);
		return;
	}

	CString text;
	text.LoadString(IDS_ENC_CLOSE_BTN);
	m_closeBtn.SetWindowText(text);
	m_fileMsg.Empty();
	m_listMsg.LoadString(IDS_ENC_FINISHED);
	m_estSize.Empty();
	m_fileXofY.Empty();
	m_inputSize.Empty();
	m_in.Empty();
	m_out.Empty();
	filePerc = 0;
	bufferPerc = 0;
	PostMessage(WM_TIMER, 0, 0);
	//m_mLockControls.Unlock();
	

	TRACE("Starting finished Jobs\n");
	if(errors != 0){

		int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG_ERR, MB_YESNO + MB_ICONQUESTION);
		
		if(nResult == IDYES){
			
			viewLog();
		}
	}
	else{

		//check what to be done after encoding


		if ((lcfg.GetValue(BEEP,  FALSE)) == TRUE){  //beep

			
			Beep(500,500);
			//MessageBeep(0xFFFFFFFF);
			//MessageBeep(MB_OK);
		}

		if((lcfg.GetValue(DIALOG, FALSE)) == TRUE){ //show finished dialog
			

			int nResult = AfxMessageBox(IDS_ENC_VIEW_LOG, MB_YESNO + MB_ICONQUESTION);
			
			if(nResult == IDYES){

				viewLog();
			}
		}				
		
		if ((lcfg.GetValue(M3U, FALSE)) == TRUE){  //create m3u-playlist with encoded files

			m_listMsg.LoadString(IDS_ENC_WRITE_M3U);
			//UpdateData(FALSE);
			PostMessage(WM_TIMER,0,0);
			CStdioFile m3u;
			CFileException f;
			tmp = ((MP3File *)m_files->GetAt(0))->getSaveAs();
			tmp = tmp.Left(tmp.ReverseFind('\\') + 1);
			tmp += ((MP3File *)m_files->GetAt(0))->getID3Info()->getArtist() + " - " + ((MP3File *)m_files->GetAt(0))->getID3Info()->getAlbum() + ".m3u";
			TRY{
				m3u.Open(tmp, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &f );
				for(int i = 0; i < m_files->GetSize(); i++){
					
					m3u.WriteString(((MP3File *)m_files->GetAt(i))->getSaveAs() + "\n");
				}
				m3u.Flush();
				m3u.Close();
				m_listMsg.LoadString(IDS_ENC_SAVED_M3U);
				m_listMsg +=  tmp;
			}
			CATCH(CFileException, f){

				m_listMsg.Format(IDS_ENC_FAILED_M3U);
				m_listMsg +=  tmp;
			}
			END_CATCH;
			//UpdateData(FALSE);
			PostMessage(WM_TIMER,0,0);
		}		

		if ((lcfg.GetValue(PLAY, FALSE)) == TRUE){ //play encoded files


			CStdioFile m3u;
			CFileException f;
			TRY{
				
				m3u.Open(wd + "\\lameFE.m3u", CFile::modeCreate | CFile::modeWrite | CFile::typeText, &f );
				for(int i = 0; i < m_files->GetSize(); i++){
					
					m3u.WriteString(((MP3File *)m_files->GetAt(i))->getSaveAs() + "\n");
				}
				m3u.Flush();
				m3u.Close();
				m_listMsg.LoadString(IDS_ENC_SAVED_M3U);
				m_listMsg += wd + "\\lameFE.m3u";
			}
			CATCH(CFileException, f){

				m_listMsg.Format(IDS_ENC_FAILED_M3U);
				m_listMsg += wd + "\\lameFE.m3u";
			}
			END_CATCH;

			PostMessage(WM_TIMER,0,0);
			tmp = lcfg.GetStringValue(PLAYER);
			WinExec(tmp + " " + wd + "\\lameFE.m3u", SW_SHOW);
		}
	}

}


BOOL EncodingStatusDlg::FileExists(CString filename)
{

	TRY{
		
		CFileStatus rStatus;
		if(CFile::GetStatus( filename, rStatus )){

			return TRUE;
		}
		else{

			return FALSE;
		}
	}
	CATCH(CFileException, e){

		return FALSE;
	}
	END_CATCH;

	return FALSE;
}

BOOL EncodingStatusDlg::createSubDirs(CString basePath, CString artist, CString album)
{

	if(_chdir(basePath) != 0){

		return FALSE;
	}

	if(_chdir(artist) != 0){

		
		if(mkdir(artist) != 0){

			return FALSE;
		}
	}

	if(_chdir(basePath + "\\" + artist) != 0){

		return FALSE;
	}

	if(_chdir(album) != 0){

		
		if(mkdir(album) != 0){

			return FALSE;
		}
	}

	return TRUE;
}

BOOL EncodingStatusDlg::createDirs(CString filename)
{

	int nStart = 3;
	int nEnd = 0;
	
	CString validated = filename.Left(2);
	_chdir(validated);
	
	while(TRUE){

		nEnd = filename.Find('\\', nStart);
		if(nEnd == -1){

			return TRUE;
		}

		if(_chdir(validated + "\\" + filename.Mid(nStart, nEnd - nStart)) != 0){

			_chdir(validated);
			if(_mkdir(filename.Mid(nStart, nEnd - nStart)) != 0){

				return FALSE;
			}
			
		}
		validated += "\\" + filename.Mid(nStart, nEnd - nStart);
		nStart = nEnd + 1;		
	}

	return FALSE;
}

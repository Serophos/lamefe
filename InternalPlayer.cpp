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
#include "InternalPlayer.h"
#include "MP3File.h"
#include <string>
#include <fstream>
#include <io.h>
#include "CDRip/CDRip.h" 
//#include "dsiPlayer/dsiPlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////



InternalPlayer::InternalPlayer(int playerMode, HWND hWnd)

{
	
	mode		 = playerMode;
	isPlaying	 = false;
	currentTrack = 0;
	files        = NULL;
	m_bPaused	 = FALSE;
	m_bStopped   = FALSE;

/*	if(playerMode == PLAYER_MODE_WAVE){
		
		DSIP_Init(hWnd);
	}*/
}

InternalPlayer::~InternalPlayer()
{
	
}


UINT InternalPlayer::playback(LPVOID lParam){
	
	InternalPlayer *ip = (InternalPlayer*)lParam;

	ip->playInternal();
	
	AfxEndThread(0, TRUE);
	return 0;
}

void InternalPlayer::play()
{
    
	m_bStopped = FALSE;
	thread = AfxBeginThread(playback, (void*)this);
}



void InternalPlayer::stop()
{
	
	if(!files){
		return;
	}
	if(!files->GetSize()){
		return;
	}

	m_bStopped = TRUE;
	if(mode == PLAYER_MODE_CD){
		
		CR_StopPlayTrack();
	}

	/*if(mode == PLAYER_MODE_WAVE){

		DSIP_Stop();
	}*/
	currentTrack = 0;
}



void InternalPlayer::setPlaylist(CPtrArray *pFiles)
{

	files = pFiles;
}

void InternalPlayer::next()
{

	if(!files){
		return;
	}
	if(!files->GetSize()){
		return;
	}

/*	if(mode == PLAYER_MODE_WAVE){

		DSIP_Stop();
	}
*/
	if(currentTrack + 1 >= files->GetSize()){

		currentTrack = 0;
	}
	else{

		currentTrack++;
	}
	play();
}

void InternalPlayer::prev()
{
	if(!files){
		return;
	}
	if(!files->GetSize()){
		return;
	}

/*	if(mode == PLAYER_MODE_WAVE){

		DSIP_Stop();
	}*/
	if(currentTrack - 1< 0){

		currentTrack = files->GetSize() - 1;
	}
	else{

		currentTrack--;
	}
	play();
}

void InternalPlayer::playInternal()
{

	if(!files){
		return;
	}
	if(!files->GetSize()){
		return;
	}


	if(mode == PLAYER_MODE_CD){
		
		if(m_bPaused){

			pause();
			return;
		}

		for(int i = currentTrack; i < files->GetSize() && !m_bStopped; i++){
			
			CR_PlayTrack(((MP3File*)files->GetAt(currentTrack))->getCDTrackNum());
			while(CR_IsAudioPlaying()){

				Sleep(500);
			}
		}
	}
/*	if(mode == PLAYER_MODE_WAVE){

		for(int i = currentTrack; i < files->GetSize() && !m_bStopped; i++){
			
			TRACE("%d\n", i);
			CString filename = ((MP3File*)files->GetAt(i))->getFileName();

			if(DSIP_Open(filename.GetBuffer(10))){

				DSIP_Play();
				Sleep(200);
			}
			filename.ReleaseBuffer();
			while(DSIP_IsAudioPlaying() || m_bPaused){

				Sleep(500);
			}
		}
	}*/
}


void InternalPlayer::deInit()
{

	stop();

	if(mode == PLAYER_MODE_CD){

		if(files){

			MP3File *tmp;
			
			for(int i = files->GetSize() - 1; i >= 0; i--){

				tmp = (MP3File*)files->GetAt(i);
				files->RemoveAt(i);
				delete tmp;
			}

			files->FreeExtra();
		}
		delete files;
	}
}

void InternalPlayer::pause()
{


	if(mode == PLAYER_MODE_CD){

		m_bPaused = !m_bPaused;
		CR_PauseCD(m_bPaused);
		return;
	}

/*	if(mode == PLAYER_MODE_WAVE){

		m_bPaused = !m_bPaused;
		DSIP_Play();

	}*/
}

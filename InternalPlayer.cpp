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
//#include "MP3File.h"
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



InternalPlayer::InternalPlayer(int playerMode, HWND hWnd /*= NULL */)

{
	
	mode		 = playerMode;
	isPlaying	 = FALSE;
	currentTrack = 0;
	m_cd        = NULL;
	m_bPaused	 = FALSE;
	m_bStopped   = FALSE;
}

InternalPlayer::~InternalPlayer()
{
	
}


UINT InternalPlayer::playback(LPVOID lParam){  // This is the internal Thread func
	
	TRACE("Starting Internal Player Thread\n");
	InternalPlayer *ip = (InternalPlayer*)lParam;

	ip->playInternal();
	
	//AfxEndThread(0, TRUE);
	TRACE("------- Terminating Internal Player Thread ---------\n");
	return 0;
}

void InternalPlayer::play()
{
    
	m_bStopped = FALSE;
	thread = AfxBeginThread(playback, (void*)this);
}

void InternalPlayer::stop()
{
	
	if(!m_cd){
		return;
	}
	if(!m_cd->GetNumTracks()){
		return;
	}

	m_bStopped = TRUE;
	if(mode == PLAYER_MODE_CD){
		
		CR_StopPlayTrack();
	}
	currentTrack = 0;

}



void InternalPlayer::setPlaylist(CCompactDisc *cd)
{

	m_cd = cd;
}

void InternalPlayer::next()
{

	if(!m_cd){
		return;
	}
	if(!m_cd->GetNumTracks()){
		return;
	}
	
	TRACE("Next Track\n");
	if(currentTrack + 1 >= m_cd->GetNumTracks()){

		currentTrack = 0;
	}
	else{

		currentTrack++;
	}
	TRACE("Playback Stop");
	CR_StopPlayTrack();
}

void InternalPlayer::prev()
{
	if(!m_cd){
		return;
	}
	if(!m_cd->GetNumTracks()){
		return;
	}

	if(currentTrack - 1< 0){

		currentTrack = m_cd->GetNumTracks() - 1;
	}
	else{

		currentTrack--;
	}
	play();
}

void InternalPlayer::playInternal()
{

	if(!m_cd){
		return;
	}
	if(!m_cd->GetNumTracks()){
		return;
	}

	if(mode == PLAYER_MODE_CD){
		
		if(m_bPaused){

			pause();
			return;
		}

		for(int i = currentTrack; i < m_cd->GetNumTracks() && !m_bStopped; i++){
			
			TRACE("Starting playback of Audio Track %d\n", currentTrack);
			CR_PlayTrack(m_cd->GetCDTrack(currentTrack)->m_btTrack);
			while(isCDPlaying()){

				Sleep(500);
				//TRACE("Sleep %d\n", i);
			}
		}
	}
}


void InternalPlayer::deInit()
{

	stop();
}

void InternalPlayer::pause()
{


	if(mode == PLAYER_MODE_CD){

		m_bPaused = !m_bPaused;
		CR_PauseCD(m_bPaused);
		return;
	}

}

BOOL InternalPlayer::isCDPlaying()
{

	BOOL bReturn = TRUE;

	BYTE btAudioPlaying = CR_IsAudioPlaying();

	// Are we still playing
	if (( btAudioPlaying==0x13 || btAudioPlaying==0x14 || btAudioPlaying==0x15 ))
	{
		bReturn = FALSE;
	}

	return bReturn;

}

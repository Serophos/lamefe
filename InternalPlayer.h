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


#if !defined(AFX_INTERNALPLAYER_H__540FB980_98FD_4BB3_8CB7_5BBEDE41720B__INCLUDED_)
#define AFX_INTERNALPLAYER_H__540FB980_98FD_4BB3_8CB7_5BBEDE41720B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CompactDisk.h"

#define PLAYER_MODE_CD    0
#define PLAYER_MODE_WAVE  1

class InternalPlayer  
{
public:
	BOOL isCDPlaying();
	void pause();
	void deInit();
	void prev();
	void next();
	void setPlaylist(CCompactDisc *cd);
	void stop();
	static UINT playback(LPVOID lParam);
	void play();
	InternalPlayer(int playerMode, HWND hWnd);
	void playInternal();
	virtual ~InternalPlayer();

protected:
	BOOL isPlaying;
	int mode;
private:
	BOOL m_bStopped;
	BOOL m_bPaused;
	CWinThread *thread;
	CCompactDisc *m_cd;
	int currentTrack;
};

#endif // !defined(AFX_INTERNALPLAYER_H__540FB980_98FD_4BB3_8CB7_5BBEDE41720B__INCLUDED_)

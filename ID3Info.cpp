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
#include "ID3Info.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CID3Info::CID3Info()
{
	
	m_album.LoadString(IDS_DEFAULT_ALBUM);
	m_artist.LoadString(IDS_DEFAULT_ARTIST);
	m_song.Empty();
	m_genre = "Other";
	m_comment.Empty();
	m_track = 0;
	m_year = 0;
}


CID3Info::~CID3Info()
{

}

CString CID3Info::GetSong()
{

	return m_song;
}

CString CID3Info::GetArtist()
{

	return m_artist;
}

CString CID3Info::GetAlbum()
{

	return m_album;
}

CString CID3Info::GetComment()
{

	return m_comment;
}

CString CID3Info::GetGenre()
{

	return m_genre;
}

int CID3Info::GetTrack()
{

	return m_track;
}

int CID3Info::GetYear()
{

	return m_year;
}

void CID3Info::SetSong(CString songname)
{

	m_song = songname;
}

void CID3Info::SetArtist(CString artist)
{

	m_artist = artist;
}

void CID3Info::SetAlbum(CString album)
{

	m_album = album;
}


void CID3Info::SetComment(CString comment)
{

	m_comment = comment;
}


void CID3Info::SetGenre(CString genre)
{

	m_genre = genre;
}


void CID3Info::SetTrack(int track)
{

	m_track = track;
}


void CID3Info::SetYear(int year)
{

	m_year = year;
}



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
#include "ID3Info.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

ID3Info::ID3Info()
{
	
	m_album = "Unspecified Album";
	m_artist = "Unspecified Artist";
	m_song.Empty();
	m_genre.Empty();
	m_comment.Empty();
	m_track = 0;
	m_year = 0;
}


/*ID3Info::ID3Info(CString song, CString artist, CString album, CString comment, CString genre, int track, int year)
{

	m_album	   = album;
	m_artist   = artist;
	m_song     = song;
	m_genre    = genre;
	m_comment  = comment;
	m_track    = track;
	m_year     = year;
}*/


ID3Info::~ID3Info()
{

}

CString ID3Info::getSong()
{

	return m_song;
}

CString ID3Info::getArtist()
{

	return m_artist;
}

CString ID3Info::getAlbum()
{

	return m_album;
}

CString ID3Info::getComment()
{

	return m_comment;
}

CString ID3Info::getGenre()
{

	return m_genre;
}

int ID3Info::getTrack()
{

	return m_track;
}

int ID3Info::getYear()
{

	return m_year;
}

void ID3Info::setSong(CString songname)
{

	m_song = songname;
}

void ID3Info::setArtist(CString artist)
{

	m_artist = artist;
}

void ID3Info::setAlbum(CString album)
{

	m_album = album;
}


void ID3Info::setComment(CString comment)
{

	m_comment = comment;
}


void ID3Info::setGenre(CString genre)
{

	m_genre = genre;
}


void ID3Info::setTrack(int track)
{

	m_track = track;
}


void ID3Info::setYear(int year)
{

	m_year = year;
}



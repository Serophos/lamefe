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

#if !defined(AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_)
#define AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ID3Info  
{
public:

	ID3Info();
	//ID3Info(CString song, CString artist, CString album, CString comment, CString genre, int track, int year);
	virtual ~ID3Info();

	CString getSong();
	CString getArtist();
	CString getAlbum();
	CString getComment();
	CString getGenre();
	int     getTrack();
	int     getYear();

	void	setSong(CString songname);
	void	setArtist(CString artist);
	void	setAlbum(CString album);
	void	setComment(CString comment);
	void	setGenre(CString genre);
	void    setTrack(int track);
	void    setYear(int year);

protected:
	CString m_song;
	CString m_artist;
	CString m_album;
	CString m_comment;
	CString m_genre;
	int     m_track;
	int     m_year;

};

#endif // !defined(AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_)

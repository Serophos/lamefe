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


#ifndef MMFILE_H___INCLUDED
#define MMFILE_H___INCLUDED

typedef struct
{

	DWORD dwSampleRate;
	int	  nChannels;
	int   nBitsPerSample;
} MMFILE_FORMAT, *PMMFILE_FORMAT;

typedef struct
{

	const char * artist;
	const char * album;
	const char * song;
	const char * comment;
	const char * genre;
	int track;
	int year;
} MMFILE_ALBUMINFO, *PMMFILE_ALBUMINFO;

#endif
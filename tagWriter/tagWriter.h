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

#undef DLLEXPORT
#define DLLEXPORT
#define CCONV _stdcall

typedef struct{
	
	BYTE MajorVersion;		//Majorversion number of tagWriter.dll
	BYTE MinorVersion;		//Minorversion number of tagWriter.dll

	BYTE id3libMajorVersion;  //Majorversion number of libmp3lame.lib
	BYTE id3libMinorVersion;  //Minorversion number of libmp3lame.lib
	
	BYTE rDay;				//release Day of tagWriter.dll
	BYTE rMonth;			//release Month of tagWriter.dll
	WORD rYear;				//release Year of tagWrite.dll

	CHAR Homepage[128];		//Home of tagWiter.dll

} MP3_VERSION, *PMP3_VERSION;


DLLEXPORT BOOL CCONV TW_InitTagRenderer(LPCTSTR filename);

DLLEXPORT BOOL CCONV TW_RenderTags(LPCTSTR song,
							 LPCTSTR artist,
							 LPCTSTR album,
							 LPCTSTR comment,
							 LPCTSTR genre,
							 LPCTSTR year,
							 LPCTSTR track);



DLLEXPORT void CCONV TW_GetVersion(PMP3_VERSION pmp3Version);


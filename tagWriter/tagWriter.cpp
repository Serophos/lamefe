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
#include "TagRenderer.h"
#include "tagWriter.h"

#define MAJORVERSION		0;
#define MINORVERSION		90;
#define ID3MAJORVER			3;
#define ID3MINORVER			8;


TagRenderer *tRenderer = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DLLEXPORT BOOL CCONV TW_InitTagRenderer(LPCTSTR filename){

	tRenderer = new TagRenderer(filename);

	if (!tRenderer){

		return false;
	}

	return true;
}

DLLEXPORT BOOL CCONV TW_RenderTags(LPCTSTR song,
							 LPCTSTR artist,
							 LPCTSTR album,
							 LPCTSTR comment,
							 LPCTSTR genre,
							 LPCTSTR year,
							 LPCTSTR track){

	if(!tRenderer){

		return false;
	}

	tRenderer->RenderTag(song, artist, album, comment, genre, year, track);
	
	return true;
}

DLLEXPORT void CCONV TW_GetVersion(PMP3_VERSION pmp3Version){

	char Date[20]	= {'0',};
	char tmp[5]		= {'0',};

	pmp3Version->MajorVersion = MAJORVERSION;
	pmp3Version->MinorVersion = MINORVERSION;
	
	pmp3Version->id3libMajorVersion = ID3MAJORVER;
	pmp3Version->id3libMinorVersion = ID3MINORVER;

	strcpy(Date, __DATE__);
	strncpy(tmp, Date, 3);
	tmp[3] = '\0';

	pmp3Version->rMonth = 1;

	if(strcmp(tmp, "Jan") == 0) pmp3Version->rMonth = 1;
	if(strcmp(tmp, "Feb") == 0) pmp3Version->rMonth = 2;
	if(strcmp(tmp, "Mar") == 0) pmp3Version->rMonth = 3;
	if(strcmp(tmp, "Apr") == 0) pmp3Version->rMonth = 4;
	if(strcmp(tmp, "May") == 0) pmp3Version->rMonth = 5;
	if(strcmp(tmp, "Jun") == 0) pmp3Version->rMonth = 6;
	if(strcmp(tmp, "Jul") == 0) pmp3Version->rMonth = 7;
	if(strcmp(tmp, "Aug") == 0) pmp3Version->rMonth = 8;
	if(strcmp(tmp, "Sep") == 0) pmp3Version->rMonth = 9;
	if(strcmp(tmp, "Oct") == 0) pmp3Version->rMonth = 10;
	if(strcmp(tmp, "Nov") == 0) pmp3Version->rMonth = 11;
	if(strcmp(tmp, "Dec") == 0) pmp3Version->rMonth = 12;

	pmp3Version->rDay = atoi(Date + 4);
	pmp3Version->rYear = atoi(Date + 7);
	
	memset(pmp3Version->Homepage, 0x00, 128);
	strcpy(pmp3Version->Homepage, "www.nauta-clarus-media.de");
}


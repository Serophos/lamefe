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
#include "VirtualOutPlugin.h"
#include "Encoder.h"
#include "EncoderFeeder.h"
#include "Resource.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//This is all subject to change

//#define RAWOUTPUT

/*#ifdef RAWOUTPUT

HANDLE	outFile;
int		w_offset;
int		last_pause;
 //RAWOUTPUT
#else

int		written_time;
CString         wd;
Encoder		    *e					= NULL;
EncoderFeeder   *ef					= NULL;
CMultimediaFile *mFile				= NULL;
//CMutex			g_mutex;
unsigned long   dwSampleBufferSize	= 0;
int				nOffset				= 0;
int				samplesUsed			= 0;
char*			pcStream			= 0;
#endif

int		s_nOutSampleRate    = 0;
int		s_nNumChannels		= 0;	
int		s_nBufferLenghtMs	= 0;
int		s_nPreBufferMs		= 0;
int		s_bOutOpened		= FALSE;
int		s_nBitsPerSample	= 0;
int		&s_nBufferStatus	= samplesUsed;
CString m_list_errors		= "";
BOOL	bAbort				= FALSE;


CEvent	g_eFileIsOpened;
*/


// input file properties

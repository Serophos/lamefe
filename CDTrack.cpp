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
#include "CDTrack.h"
#include "Resource.h"
#include "cfgFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CDROMDATAFLAG 0x04
#define AUDIOTRKFLAG 0x10

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCDTrack::CCDTrack()
{

	m_btFlags		= 0;
	m_btTrack		= 0;
	m_dwStartSector = 0;
	m_strTrackName	= "";
	m_bExtract		= FALSE;
	bAlerted		= FALSE;
	m_bRename		= TRUE;
	
}

CCDTrack::~CCDTrack()
{

}

BOOL CCDTrack::IsAudioTrack()
{

	return !(m_btFlags & CDROMDATAFLAG); // hehe, finally fixed it, yeah!! :-)
}

CString CCDTrack::GetTrackname()
{

	
	if(IsAudioTrack()){
		
		m_strTrackName.Format(IDS_MAIN_AUDIOTRACK, m_btTrack);
	}
	else{

		m_strTrackName.LoadString(IDS_MAIN_DATATRACK);
	}
	return m_strTrackName;
}


//DEL void CCDTrack::SetErrorString(CString err)
//DEL {
//DEL 
//DEL 	m_strError = err;
//DEL }

//DEL void CCDTrack::SetErrorString(UINT errID)
//DEL {
//DEL 
//DEL 	m_strError.LoadString(errID);
//DEL }

//DEL CString CCDTrack::GetError()
//DEL {
//DEL 	
//DEL 	return m_strError;
//DEL }



BOOL CCDTrack::IsSelected()
{

	return m_bExtract;
}

void CCDTrack::SetExtract(BOOL bExtract)
{

	m_bExtract = bExtract;
}

void CCDTrack::SetRename(BOOL bFlag)
{

	m_bRename = bFlag;
}

BOOL CCDTrack::GetRename()
{

	return m_bRename;
}


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
#include "I18n.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CDROMDATAFLAG 0x04
#define AUDIOTRKFLAG 0x10

extern CI18n g_iLang;

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
	m_bAlerted		= FALSE;
	
}

CCDTrack::CCDTrack(CCDTrack &cdTrack)
{

	m_btFlags		= cdTrack.m_btFlags;
	m_btTrack		= cdTrack.m_btTrack;
	m_dwStartSector = cdTrack.m_dwStartSector;
	m_strTrackName	= cdTrack.m_strTrackName;
	m_bExtract		= cdTrack.m_bExtract;
	m_bAlerted		= cdTrack.m_bAlerted;
	m_id3Info.Copy(&cdTrack.m_id3Info);
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
		
		m_strTrackName.Format(g_iLang.GetString(IDS_MAIN_AUDIOTRACK), m_btTrack);
	}
	else{

		m_strTrackName = g_iLang.GetString(IDS_MAIN_DATATRACK);
	}
	return m_strTrackName;
}



BOOL CCDTrack::IsSelected()
{

	return m_bExtract;
}

void CCDTrack::SetExtract(BOOL bExtract)
{

	m_bExtract = bExtract;
}

CCDTrack& CCDTrack::operator=(const CCDTrack &cdTrack )
{

	if(this != &cdTrack){

		m_btFlags		= cdTrack.m_btFlags;
		m_btTrack		= cdTrack.m_btTrack;
		m_dwStartSector = cdTrack.m_dwStartSector;
		m_strTrackName	= cdTrack.m_strTrackName;
		m_bExtract		= cdTrack.m_bExtract;
		m_bAlerted		= cdTrack.m_bAlerted;
		m_id3Info.Copy(&(CID3Info)cdTrack.m_id3Info);
	}

	return *this;
}



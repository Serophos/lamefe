/*
** Copyright (C) 2002-2003 Thees Winkler
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

#if !defined(AFX_CDTRACK_H__BD7B46E6_D8BF_46E2_A5F0_BAD330A886E4__INCLUDED_)
#define AFX_CDTRACK_H__BD7B46E6_D8BF_46E2_A5F0_BAD330A886E4__INCLUDED_

#include "ID3Info.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultimediaFile.h"

class CCDTrack //: public CMultimediaFile  
{
public:
	BOOL GetRename();
	void SetRename(BOOL bFlag);
	void SetExtract(BOOL bExtract);
	BOOL IsSelected();
	CString GetTrackname();
	BOOL IsAudioTrack();
	CCDTrack();
	virtual ~CCDTrack();
	BOOL  GetAlerted(){ return bAlerted; }
	void  SetAlerted(){ bAlerted = TRUE; }

	DWORD	m_dwStartSector;
	BYTE	m_btFlags;
	BYTE	m_btTrack;
	CString	m_strTrackName;
	CID3Info m_id3Info;
private:
	BOOL bAlerted;
	BOOL m_bExtract;
	BOOL m_bRename;
//	CString m_strError;
};

#endif // !defined(AFX_CDTRACK_H__BD7B46E6_D8BF_46E2_A5F0_BAD330A886E4__INCLUDED_)

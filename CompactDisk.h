/*
** Copyright (C) 2002-2003 Thees Winkler
**  
** Parts of this codes are based on code from CDEx (c) 1999-2002 by Albert L. Faber 
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

#if !defined(AFX_COMPACTDISK_H__1BDF1F5B_BD29_4D42_B60D_E7C9DA880C05__INCLUDED_)
#define AFX_COMPACTDISK_H__1BDF1F5B_BD29_4D42_B60D_E7C9DA880C05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	
#define MAXTRACKS		100
#define TRACKSPERSEC	75
#define CB_CDDASECTOR	2352
#define LEAD_OUT_NBR	0xAA


#include "CDTrack.h"

typedef CArray<CCDTrack, CCDTrack>	CCDTrackArray;

class CCompactDisc  
{
public:
	DWORD GetEndSector(int nTrack);
	int GetLastAudioTrack();
	CString GetTrackDurationMS(int track);
	CString GetStartTime(int nTrack);
	BOOL WriteCueSheet(CString strWd, CString fileExt);
	CString GetSaveAs(int nTrack, CString wd, CString ext, BOOL bAppendDiscID = FALSE);
	CString GetAlbumString(CString wdir, CString ext, BOOL bAppendDiscID = FALSE);
	BOOL ReadCDText();

	CCompactDisc();
	virtual ~CCompactDisc();

	CCDTrack* GetCDTrack(int n);

	void Eject();
	void Init();

	void	GetSelItems(int iCnt, LPINT pIndex);
	void	SetSelItems(int iCnt, LPINT pIndex);
	int		GetNumSelTracks();
	int		GetNumAudioTracks();
	int		GetNumTracks();
	int		GetDiscID();
	int		GetVolID();
	CString GetStrVolID();

	static int cddb_sum(int n);

private:
	BOOL	m_bAlerted;
	int*    m_pSel;
	int     m_nTocEntries;
	CString m_strMCDI;
	int		CalculateDiscID();
	int		CalculateVolID();
	int		m_nAudioTracks;
	int		m_nNumTracks;
	int		m_nDiscID;
	CString m_strVolID;
	int		m_nVolID;

	CCDTrackArray	m_cdTracks;

};

#endif // !defined(AFX_COMPACTDISK_H__1BDF1F5B_BD29_4D42_B60D_E7C9DA880C05__INCLUDED_)

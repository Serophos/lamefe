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

#if !defined(AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_)
#define AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ID3T_Tag
{
  ID3T_NONE    =      0,   /**< Represents an empty or non-existant tag */
  ID3T_ID3V1   = 1 << 0,   /**< Represents an id3v1 or id3v1.1 tag */
  ID3T_ID3V2   = 1 << 1,   /**< Represents an id3v2 tag */
  /** Represents both id3 tags: id3v1 and id3v2 */
  ID3T_ID3     = ID3T_ID3V1 | ID3T_ID3V2,
};

class CID3Info  
{
public:
	void FixArtistName(ID3T_Tag iType = ID3T_ID3);
	void FixVariousArtistCD(ID3T_Tag iType = ID3T_ID3);
	CID3Info();
	//CID3Info(CID3Info &id3Info);
	virtual ~CID3Info();

	// Methods
	void SwapAlbumInterpret(ID3T_Tag iType = ID3T_ID3);
	BOOL Write(CString strFile);
	BOOL Read(CString strFile);
	void Copy(CID3Info *id3Tag, BOOL bCopySong = TRUE);
	void SwapTitleInterpret(ID3T_Tag iType = ID3T_ID3);
	void Sync(ID3T_Tag iType = ID3T_ID3V2);
	void Empty(ID3T_Tag iType = ID3T_ID3);
	void Case(ID3T_Tag iType = ID3T_ID3);

	// Operators
	CID3Info& operator=(const CID3Info &);
	// Data Access
	void SetGenre(int nValue, ID3T_Tag iType = ID3T_ID3);
	void SetGenre(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetEncodedBy(CString strValue);
	void SetYearNum(int nValue, ID3T_Tag iType = ID3T_ID3);
	void SetYear(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetTrackNum(int nValue, ID3T_Tag iType = ID3T_ID3);
	void SetTrack(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetSong(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetComment(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetArtist(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetAlbum(CString strValue, ID3T_Tag iType = ID3T_ID3);
	void SetTLEN(CString strValue){ m_strTLEN = strValue;}

	int GetGenreNum(ID3T_Tag iType = ID3T_ID3V2);
	CString GetGenre(ID3T_Tag iType = ID3T_ID3V2);
	int GetTrackNum(ID3T_Tag iType = ID3T_ID3V2);
	CString GetTrack(ID3T_Tag iType = ID3T_ID3V2);
	int GetYearNum(ID3T_Tag iType = ID3T_ID3V2);
	CString GetYear(ID3T_Tag iType = ID3T_ID3V2);
	CString GetSong(ID3T_Tag iType = ID3T_ID3V2);
	CString GetComment(ID3T_Tag iType = ID3T_ID3V2);
	CString GetArtist(ID3T_Tag iType = ID3T_ID3V2);
	CString GetAlbum(ID3T_Tag iType = ID3T_ID3V2);
	CString GetEncodedBy();
	CString GetTLEN(){ return m_strTLEN;};





private:
	// Data Version 1 Tag
	CString m_strSongV1;
	CString m_strArtistV1;
	CString m_strAlbumV1;
	CString m_strCommentV1;
	int     m_nGenreV1;
	int     m_nTrackV1;
	int     m_nYearV1;

	// Data Version 2 Tag
	CString m_strSongV2;
	CString m_strArtistV2;
	CString m_strAlbumV2;
	CString m_strCommentV2;
	CString m_strEncodedBy;
	CString m_strGenreV2;
	CString m_strTrackV2;
	CString m_strYearV2;
	CString m_strTLEN;
	
protected:
	CString CorrectCase(CString strToCorrect);
};

#endif // !defined(AFX_ID3INFO_H__1FC4214F_1716_47A4_95EA_C78B6A9A3F34__INCLUDED_)

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
#include "ID3Info.h"
#include "Resource.h"

#include "ID3Lib/include/id3.h"
#include "ID3Lib/include/id3/misc_support.h"

#include "Settings.h"
#include "I18n.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

extern CI18n g_iLang;
extern CSettings g_sSettings;

static const char *const genre_names[] =
{
	"Acapella", "Acid", "Acid Jazz", "Acid Punk", "Acoustic", "Alternative", "Alternative Rock", "Ambient", 
	"Anime", "Avantgarde", "Ballad", "Bass", "Beat", "Bebob", "Big Band", "Black Metal", "Bluegrass", "Blues", 
	"Booty Bass", "BritPop", "Cabaret", "Celtic", "Chamber Music", "Chanson", "Chorus", "Christian Gangsta", 
	"Christian Rap", "Christian Rock", "Classic Rock", "Classical", "Club", "Club-House", "Comedy", 
	"Contemporary C", "Country", "Crossover", "Cult", "Dance", "Dance Hall", "Darkwave", "Death Metal", 
	"Disco", "Dream", "Drum & Bass", "Drum Solo", "Duet", "Easy Listening", "Electronic", "Ethnic", "Eurodance", 
	"Euro-House", "Euro-Techno", "Fast Fusion", "Folk", "Folklore", "Folk-Rock", "Freestyle", "Funk", "Fusion", 
	"Game", "Gangsta", "Goa", "Gospel", "Gothic", "Gothic Rock", "Grunge", "Hard Rock", "Hardcore", "Heavy Metal", 
	"Hip-Hop", "House", "Humour", "Indie", "Industrial", "Instrumental", "Instrumental Pop", "Instrumental Rock", 
	"Jazz", "Jazz+Funk", "JPop", "Jungle", "Latin", "Lo-Fi", "Meditative", "Merengue", "Metal", "Musical", "National Folk", 
	"Native US", "Negerpunk", "New Age", "New Wave", "Noise", "Oldies", "Opera", "Other", "Polka", "Polsk Punk", "Pop", 
	"Pop/Funk", "Pop-Folk", "Porn Groove", "Power Ballad", "Pranks", "Primus", "Progressive Rock", "Psychadelic", 
	"Psychedelic Rock", "Punk", "Punk Rock", "R&B", "Rap", "Rave", "Reggae", "Retro", "Revival", "Rhytmic Soul", "Rock", 
	"Rock & Roll", "Salsa", "Samba", "Satire", "Showtunes", "Ska", "Slow Jam", "Slow Rock", "Sonata", "Sound Clip", 
	"Soundtrack", "Southern Rock", "Space", "Speech", "Swing", "Symphonic Rock", "Symphony", "SynthPop", "Tango", 
	"Techno", "Techno-Industrial", "Terror", "Thrash Metal", "Top ", "Trailer", "Trance", "Tribal", "Trip-Hop", 
	"Vocal"
};

#define GENRE_NAME_COUNT   ((int)(sizeof genre_names / sizeof (const char *const)))


CID3Info::CID3Info()
{

	m_strAlbumV1  = g_iLang.GetString(IDS_DEFAULT_ALBUM);
	m_strAlbumV2  = m_strAlbumV1;
	m_strArtistV1 = g_iLang.GetString(IDS_DEFAULT_ARTIST);
	m_strArtistV2 = m_strArtistV1;
	m_strGenreV2  = "Ohter";
	m_nGenreV1    = 127;
	m_nTrackV1    = 0;
	m_strTrackV2  = "0";
	m_strYearV2   = "";
	m_nYearV1 	  = 0;

}

/*
CID3Info::CID3Info(CID3Info &id3Info)
{

	//Copy(&id3Info, TRUE);
}

*/
CID3Info::~CID3Info()
{

}

void CID3Info::SwapTitleInterpret(ID3T_Tag iType)
{

	CString strTmp;
	if(iType & ID3T_ID3V2){

		strTmp        = m_strArtistV2;
		m_strArtistV2 = m_strSongV2;
		m_strSongV2   = strTmp;
	}
	else{

		strTmp        = m_strArtistV1;
		m_strArtistV1 = m_strSongV1;
		m_strSongV1  = strTmp;
	}
}

void CID3Info::SwapAlbumInterpret(ID3T_Tag iType)
{

	CString strTmp;
	if(iType & ID3T_ID3V2){

		strTmp        = m_strArtistV2;
		m_strArtistV2 = m_strAlbumV2;
		m_strAlbumV2   = strTmp;
	}
	else{

		strTmp        = m_strArtistV1;
		m_strArtistV1 = m_strAlbumV1;
		m_strAlbumV1  = strTmp;
	}
}

void CID3Info::Sync(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V2){

		m_strAlbumV2   = m_strAlbumV1;
		m_strArtistV2  = m_strArtistV1;
		m_strCommentV2 = m_strCommentV1;
		m_strSongV2    = m_strSongV1;
		itoa(m_nTrackV1, m_strTrackV2.GetBuffer(10), 10);
		itoa(m_nYearV1, m_strYearV2.GetBuffer(10), 10);
	}
	else{

		m_strAlbumV1   = m_strAlbumV2;
		m_strArtistV1  = m_strArtistV2;
		m_strCommentV1 = m_strCommentV2;
		m_strSongV2    = m_strSongV2;
		m_nYearV1	   = atoi(m_strYearV2);
		m_nTrackV1     = atoi(m_strTrackV2);
	}
}

void CID3Info::Empty(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V2){
		
		m_strAlbumV2.Empty();
		m_strArtistV2.Empty();
		m_strCommentV2 .Empty();
		m_strSongV2.Empty();
		m_strTrackV2.Empty();
		m_strYearV2.Empty();
	}
	if(iType & ID3T_ID3V1){

		m_strAlbumV1.Empty();
		m_strArtistV1.Empty();
		m_strCommentV1 .Empty();
		m_strSongV1.Empty();
		m_nTrackV1 = 0;
		m_nYearV1  = 0;;
	}
	
}

void CID3Info::Case(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V2){

		m_strAlbumV2	= CorrectCase(m_strAlbumV2);
		m_strArtistV2	= CorrectCase(m_strArtistV2);
		m_strCommentV2	= CorrectCase(m_strCommentV2);
		m_strSongV2		= CorrectCase(m_strSongV2);
	}
	if(iType & ID3T_ID3V1){

		m_strAlbumV1	= CorrectCase(m_strAlbumV1);
		m_strArtistV1	= CorrectCase(m_strArtistV1);
		m_strCommentV1	= CorrectCase(m_strCommentV1);
		m_strSongV1		= CorrectCase(m_strSongV1);
	}
}

void CID3Info::FixVariousArtistCD(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V2){

		m_strArtistV2 = m_strSongV2.Left(m_strSongV2.Find(" -", 0));
		m_strSongV2 = m_strSongV2.Right(m_strSongV2.GetLength() - m_strSongV2.Find("- ", 0) - 2);
	}
	if(iType & ID3T_ID3V1){

		m_strArtistV1 = m_strSongV1.Left(m_strSongV1.Find(" -", 0));
		m_strSongV1 = m_strSongV1.Right(m_strSongV1.GetLength() - m_strSongV1.Find(" -", 0) - 2);
	}
}

void CID3Info::FixArtistName(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V2){

		CString strTmp = m_strArtistV2.Right(m_strArtistV2.GetLength() - m_strArtistV2.Find(",", 0)-1);
		m_strArtistV2 = m_strArtistV2.Left(m_strArtistV2.Find(",", 0));
		m_strArtistV2 = strTmp + " " + m_strArtistV2;
		m_strArtistV2.TrimLeft();
	}
	if(iType & ID3T_ID3V1){

		CString strTmp = m_strArtistV1.Right(m_strArtistV1.GetLength() - m_strArtistV1.Find(",", 0)-1);
		m_strArtistV1 = m_strArtistV1.Left(m_strArtistV1.Find(",", 0));
		m_strArtistV1 = strTmp + " " + m_strArtistV1;
		m_strArtistV1.TrimLeft();
	}
}

CString CID3Info::CorrectCase(CString strToCorrect)
{

	CString strRet, strTmp;

	BOOL bCorrectNext = FALSE;
	for(int i = 0; i < strToCorrect.GetLength(); i++){

		strTmp = strToCorrect.GetAt(i);

		if(bCorrectNext){

			strTmp.MakeUpper();
			strRet += strTmp;
			bCorrectNext = FALSE;
			continue;
		}

		if(strTmp == " "){

			bCorrectNext = TRUE;
		}
		strRet += strTmp;
	}

	return strRet;
}

CString CID3Info::GetArtist(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_strArtistV1.Left(33);
	}
	if(iType & ID3T_ID3V2){

		return m_strArtistV2;
	}

	return "";
}

CString CID3Info::GetComment(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_strCommentV1.Left(33);
	}
	if(iType & ID3T_ID3V2){

		return m_strCommentV2;
	}
	return "";
}

CString CID3Info::GetSong(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_strSongV1.Left(33);
	}
	if(iType & ID3T_ID3V2){

		return m_strSongV2;
	}
	return "";
}

CString CID3Info::GetYear(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){
		
		char strTmp[4];
		itoa(m_nYearV1, strTmp, 10);
		return strTmp;
	}
	if(iType & ID3T_ID3V2){

		return m_strYearV2;
	}
	return "";
}

int CID3Info::GetYearNum(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_nYearV1;
	}
	if(iType & ID3T_ID3V2){

		return atoi(m_strYearV2);
	}
	return 0;
}

CString CID3Info::GetTrack(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		char strTmp[4];
		itoa(m_nTrackV1, strTmp, 10);
		return strTmp;
	}
	if(iType & ID3T_ID3V2){

		return m_strArtistV2;
	}
	return "";
}

int CID3Info::GetTrackNum(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_nTrackV1;
	}
	if(iType & ID3T_ID3V2){

		return atoi(m_strTrackV2);
	}
	return 0;
}

CString CID3Info::GetAlbum(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_strAlbumV1.Left(33);
	}
	if(iType & ID3T_ID3V2){

		return m_strAlbumV2;
	}
	return "";
}

CString CID3Info::GetGenre(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return genre_names[(m_nGenreV1 < GENRE_NAME_COUNT ? m_nGenreV1 : 0)];
	}
	if(iType & ID3T_ID3V2){

		return m_strGenreV2;
	}
	return "";
}

int CID3Info::GetGenreNum(ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		return m_nGenreV1;
	}
	if(iType & ID3T_ID3V2){

		for(int i = 0; i < GENRE_NAME_COUNT; i++){

			if(m_strGenreV2 == CString(genre_names[i])){

				return i;
			}
		}
	}
	return 0;
}

CString CID3Info::GetEncodedBy()
{

	return m_strEncodedBy;
}


void CID3Info::SetAlbum(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_strAlbumV1 = strValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strAlbumV2 = strValue;
	}
}

void CID3Info::SetArtist(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_strArtistV1 = strValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strArtistV2 = strValue;
	}
}

void CID3Info::SetComment(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_strCommentV1 = strValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strCommentV2 = strValue;
	}
}

void CID3Info::SetSong(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_strSongV1 = strValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strSongV2 = strValue;
	}
}

void CID3Info::SetTrack(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_nTrackV1 = atoi(strValue);
	}
	
	if(iType & ID3T_ID3V2){

		m_strTrackV2 = strValue;
	}
}

void CID3Info::SetTrackNum(int nValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_nTrackV1 = nValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strTrackV2.Format("%02d", nValue);
	}
}

void CID3Info::SetYear(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_nYearV1 = atoi(strValue);
	}
	
	if(iType & ID3T_ID3V2){

		m_strYearV2 = strValue;
	}
}

void CID3Info::SetYearNum(int nValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_nYearV1 = nValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strYearV2.Format("%02d",nValue);
	}
}

void CID3Info::SetEncodedBy(CString strValue)
{

	m_strEncodedBy = strValue;
}


void CID3Info::SetGenre(CString strValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		for(int i = 0; i < GENRE_NAME_COUNT; i++){

			if(strValue == CString(genre_names[i])){

				m_nGenreV1 =  i;
			}
		}
	}
	if(iType & ID3T_ID3V2){

		m_strGenreV2 = strValue;
	}
}

void CID3Info::SetGenre(int nValue, ID3T_Tag iType)
{

	if(iType & ID3T_ID3V1){

		m_nGenreV1 = nValue;
	}
	
	if(iType & ID3T_ID3V2){

		m_strGenreV2 = genre_names[(nValue < GENRE_NAME_COUNT ? nValue : 0)];
	}

}


void CID3Info::Copy(CID3Info *id3Tag, BOOL bCopySong)
{

	if(bCopySong){
		
		m_strSongV1		= id3Tag->m_strSongV1;
		m_strSongV2		= id3Tag->m_strSongV2;
	}
	m_strArtistV1	= id3Tag->m_strArtistV1;
	m_strAlbumV1	= id3Tag->m_strAlbumV1;
	m_strCommentV1	= id3Tag->m_strCommentV1;
	m_nGenreV1		= id3Tag->m_nGenreV1;
	m_nTrackV1		= id3Tag->m_nTrackV1;
	m_nYearV1		= id3Tag->m_nYearV1;

	// Data Version 2 Tag

	m_strArtistV2	= id3Tag->m_strArtistV2;
	m_strAlbumV2	= id3Tag->m_strAlbumV2;
	m_strCommentV2	= id3Tag->m_strCommentV2;
	m_strEncodedBy	= id3Tag->m_strEncodedBy;
	m_strGenreV2	= id3Tag->m_strGenreV2;
	m_strTrackV2	= id3Tag->m_strTrackV2;
	m_strYearV2		= id3Tag->m_strYearV2;
	m_strTLEN		= id3Tag->m_strTLEN;

}

BOOL CID3Info::Read(CString strFile)
{

	ID3_Tag	id3Tag;

	id3Tag.Link(strFile);
	
	BOOL	bResult = id3Tag.HasV1Tag() || id3Tag.HasV2Tag();

	id3Tag.Link(strFile, ID3TT_ID3V1);

	if(id3Tag.HasV1Tag()){

		m_strSongV1		= ID3_GetTitle(&id3Tag);
		m_strArtistV1	= ID3_GetArtist(&id3Tag);
		m_strAlbumV1	= ID3_GetAlbum(&id3Tag);
		m_strCommentV1	= ID3_GetComment(&id3Tag);
		m_nGenreV1		= ID3_GetGenreNum(&id3Tag);
		m_nTrackV1		= atoi(ID3_GetTrack(&id3Tag));
		m_nYearV1		= atoi(ID3_GetYear(&id3Tag));
	}

	id3Tag.Link(strFile, ID3TT_ID3V2);
	// Data Version 2 Tag
	if(id3Tag.HasV2Tag()){
		
		m_strSongV2		= ID3_GetTitle(&id3Tag);
		m_strArtistV2	= ID3_GetArtist(&id3Tag);
		m_strAlbumV2	= ID3_GetAlbum(&id3Tag);
		m_strCommentV2	= ID3_GetComment(&id3Tag);
		m_strEncodedBy	= ID3_GetEncodedBy(&id3Tag);
		m_strGenreV2	= ID3_GetGenre(&id3Tag);
		m_strTrackV2	= ID3_GetTrack(&id3Tag);
		m_strYearV2		= ID3_GetYear(&id3Tag);
		m_strTLEN		= ID3_GetSongLen(&id3Tag);
	}

	return bResult;
}

BOOL CID3Info::Write(CString strFile)
{
/*
	ID3_Tag			id3Tag;
	unsigned short	sFlags  = 0;
	BOOL			bV1     = FALSE;
	BOOL			bV2		= FALSE;

	bV1	= g_sSettings.GetId3v1();
	bV2 = g_sSettings.GetId3v2();
	

	if(bV1){

		id3Tag.Link(strFile, ID3TT_ID3V1);
		if(!m_strSongV1.IsEmpty()){

			ID3_AddTitle(&id3Tag, m_strSongV1 , true);
		}

		//artist
		if(!m_strArtistV1.IsEmpty()){

			ID3_AddArtist(&id3Tag, m_strArtistV1, true);
		}

		//album
		if(!m_strAlbumV1.IsEmpty()){

			ID3_AddAlbum(&id3Tag, m_strAlbumV1, true);
		}

		//comment
		if(!m_strCommentV1.IsEmpty()){

			ID3_AddComment(&id3Tag, m_strCommentV1, "", true);
		}

		//year
	 	char *_year = new char[4];
		//unsigned char *_track = new unsigned char[4];

		if(m_nYearV1 != 0){
	
			itoa(m_nYearV1, _year, 10);
			ID3_AddYear(&id3Tag, _year, true);
		}

		//track
		if(m_nTrackV1 != 0){

 			//itoa(tmpAI.track, (char*)_track, 10);
			ID3_AddTrack(&id3Tag, m_nTrackV1, 1, true);
		}

		if(m_nGenreV1 != -1){

			ID3_AddGenre(&id3Tag, m_nGenreV1, true);
		}

		id3Tag.Update(ID3TT_ID3V1);
		delete _year;
	}

	if(bV2){

		id3Tag.Link(strFile, ID3TT_ID3V2);
		if(!m_strSongV2.IsEmpty()){

			ID3_AddTitle(&id3Tag, m_strSongV2 , true);
		}

		//artist
		if(!m_strArtistV2.IsEmpty()){

			ID3_AddArtist(&id3Tag, m_strArtistV2, true);
		}

		//album
		if(!m_strAlbumV2.IsEmpty()){

			ID3_AddAlbum(&id3Tag, m_strAlbumV2, true);
		}

		//comment
		if(!m_strCommentV2.IsEmpty()){

			ID3_AddComment(&id3Tag, m_strCommentV2, "", true);
		}

		if(m_strYearV2.IsEmpty()){
	
			ID3_AddYear(&id3Tag, m_strYearV2, true);
		}

		//track
		if(m_strTrackV2.IsEmpty()){

			ID3_AddTrack(&id3Tag, atoi(m_strTrackV2), 1, true);
		}

		if(m_strEncodedBy.IsEmpty()){

			ID3_AddEncodedBy(&id3Tag, m_strEncodedBy, true);
		}

		if(m_strTLEN.IsEmpty()){

			ID3_AddSongLen(&id3Tag, m_strTLEN, true);
		}

		if(m_strGenreV2.IsEmpty()){

			ID3_AddGenre(&id3Tag, m_strGenreV2.GetBuffer(10), true);
			m_strGenreV2.ReleaseBuffer();
		}
		id3Tag.Update(ID3TT_ID3V2);
	}
*/
	return TRUE;
}

CID3Info & CID3Info::operator=(const CID3Info &id3Info)
{

	if(this != &id3Info){

		Copy(&(CID3Info)id3Info, TRUE);
	}

	return *this;
}

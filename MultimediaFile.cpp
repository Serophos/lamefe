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
#include "MultimediaFile.h"
#include "cfgFile.h"
//#include "tagWriter/tagWriter.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMultimediaFile::CMultimediaFile(CString path, CString pluginName)
{

	m_bIsEncoded = FALSE;
	m_bRename    = TRUE;
	m_strInputFile = path;
	m_strPluginname = pluginName;
	bAlerted = FALSE;
	InitID3Tag();

}

CMultimediaFile::~CMultimediaFile()
{

}

CString CMultimediaFile::GetSaveAs(CString wdir, CString strExt)
{

	cfgFile cfg(wdir);

	CString strPath, strSaveAs, strFormat;
	strPath = cfg.GetStringValue("output");

	if(strPath.IsEmpty()){

		strPath = wdir;
	}

	if(!(m_bRename && cfg.GetValue("rename"))){
		
		CString out = m_strInputFile.Left(m_strInputFile.GetLength() - 4);
		out = out.Right(out.GetLength() - out.ReverseFind('\\'));
		strSaveAs = strPath + "\\" + out + strExt;
		return strSaveAs;
	}

	strFormat = (LPCSTR)cfg.GetStringValue("formatstr");

	CString tmp;
	//int lastMatch = 0;
	//int actualMatch = 0;
/*	while(TRUE){

		actualMatch = strFormat.Find("%", lastMatch);
		if(actualMatch != -1){

			tmp += strFormat.Mid(lastMatch, actualMatch - lastMatch) += GetValue(strFormat.Mid(actualMatch, 2));
			lastMatch = actualMatch + 2;
		}
		else{

			tmp += strFormat.Mid(lastMatch, strFormat.GetLength());
			break;
		}
	}
*/
	//tmp = strFormat;
	strFormat.Replace("%1", m_id3Info.GetArtist());
	strFormat.Replace("%2", m_id3Info.GetSong());
	strFormat.Replace("%3", m_id3Info.GetAlbum());
	strFormat.Replace("%5", m_id3Info.GetGenre());
	strFormat.Replace("%c", "0x00000000");
	strFormat.Replace("%d", "0x00000000");
	
	tmp.Format("%d", m_id3Info.GetYear());
	strFormat.Replace("%4", tmp);
	tmp.Empty();
	tmp.Format("%02d", m_id3Info.GetTrack());
	strFormat.Replace("%a", tmp);
	strFormat.Replace("%b", tmp);

	strFormat.Replace('/', '-');
	strFormat.Remove('*');
	strFormat.Remove('?');
	strFormat.Replace('\"', '\'');
	strFormat.Replace('<', '(');
	strFormat.Replace('>', ')');
	strFormat.Replace('|', '-');
	strFormat.Remove(':');
	strFormat.Remove('\\');
	strFormat.Remove('.');

	//Validate filename length
	if(strFormat.GetLength() > _MAX_FNAME){

		TRACE("Filename is too long. Saving as inputfilename.\n");

		CString msg;
		CString out = m_strInputFile.Left(m_strInputFile.GetLength() - 4);
		out = out.Right(out.GetLength() - out.ReverseFind('\\'));

		if(!bAlerted){
		
			msg.Format(IDS_ERR_LONGFILENAME, strPath + tmp, strPath + out + "mp3");
			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);
			bAlerted = TRUE;
		}
		strSaveAs = strPath + out + strExt;

		if(strSaveAs.GetLength() > _MAX_FNAME){

			AfxMessageBox("The filename is still too long.\n");
			ASSERT(FALSE);
		}

		return strSaveAs;  // return saveAs;
	}
	if(!strFormat.IsEmpty()){
		
		strSaveAs.Empty();
		strSaveAs = strPath + "\\" + strFormat + strExt;
	}	

	return strSaveAs;
}

WAVEFORMATEX* CMultimediaFile::GetWfx()
{

	return &m_wfx;
}

CID3Info* CMultimediaFile::GetID3Info()
{

	return &m_id3Info;
}

CString CMultimediaFile::GetFileName()
{

	return m_strInputFile;
}


//DEL CString CMultimediaFile::GetErrorString()
//DEL {
//DEL 
//DEL 	return m_strError;
//DEL }

//DEL void CMultimediaFile::SetErrorString(CString strError)
//DEL {
//DEL 
//DEL 	m_strError = strError;
//DEL }

//DEL void CMultimediaFile::SetErrorString(UINT nErrorID)
//DEL {
//DEL 
//DEL 	m_strError.LoadString(nErrorID);
//DEL }

//DEL void CMultimediaFile::SetEncoded(BOOL bFileIsEncoded)
//DEL {
//DEL 
//DEL 	m_bIsEncoded = bFileIsEncoded;
//DEL }

//DEL BOOL CMultimediaFile::IsEncoded()
//DEL {
//DEL 
//DEL 	return m_bIsEncoded;
//DEL }

//DEL CString CMultimediaFile::GetValue(CString val)
//DEL {
//DEL 
//DEL 	CString tmp = "";
//DEL 	if(val == "%1"){
//DEL 
//DEL 		tmp = m_id3Info.GetArtist();
//DEL 	}
//DEL 	if(val == "%2"){
//DEL 
//DEL 		tmp = m_id3Info.GetSong();
//DEL 	}
//DEL 	if(val == "%3"){
//DEL 
//DEL 		tmp = m_id3Info.GetAlbum();
//DEL 	}
//DEL 	if(val == "%4"){
//DEL 
//DEL 		tmp.Format("%d", m_id3Info.GetYear());
//DEL 	}
//DEL 	if(val == "%5"){
//DEL 
//DEL 		tmp.Format("%s", m_id3Info.GetGenre());
//DEL 	}
//DEL 	if(val == "%a"){
//DEL 
//DEL 		tmp.Format("%02d", m_id3Info.GetTrack());
//DEL 	}
//DEL 	if(val == "%b"){
//DEL 		
//DEL 		tmp.Format("%02d", m_id3Info.GetTrack());
//DEL 	}
//DEL 	if(val == "%c"){
//DEL 
//DEL 		tmp.Format("0x%X", 0);
//DEL 	}
//DEL 	if(val == "%d"){
//DEL 
//DEL 		tmp.Format("0x%X", 0);
//DEL 	}
//DEL 
//DEL 
//DEL 	tmp.Replace('/', '-');
//DEL 	tmp.Remove('*');
//DEL 	tmp.Remove('?');
//DEL 	tmp.Replace('\"', '\'');
//DEL 	tmp.Replace('<', '(');
//DEL 	tmp.Replace('>', ')');
//DEL 	tmp.Replace('|', '-');
//DEL 	tmp.Remove(':');
//DEL 	tmp.Remove('\\');
//DEL 	tmp.Remove('.');
//DEL 	return tmp;
//DEL }


//DEL void CMultimediaFile::SetRename(BOOL bRename)
//DEL {
//DEL 
//DEL 	m_bRename = bRename;
//DEL }

BOOL CMultimediaFile::GetRename()
{

	return m_bRename;
}

void CMultimediaFile::InitID3Tag()
{
	
	CString tmp = m_strInputFile.Right(m_strInputFile.GetLength() - m_strInputFile.ReverseFind('\\') - 1);
	tmp = tmp.Left(tmp.GetLength() - 4);
	m_id3Info.SetSong(tmp);

/*	if(m_strInputFile.Right(3).CompareNoCase("mp3") == 0){

		if(TW_InitTagRenderer(m_strInputFile)){
			
			LPCSTR	album	= NULL;
			LPCSTR  artist	= NULL;
			LPCSTR	comment = NULL;
			LPCSTR	genre	= NULL;
			LPCSTR	title	= NULL;
			LPCSTR	track	= NULL;
			LPCSTR	year	= NULL;

			if(!TW_ReadTags(title, artist, album, comment, genre, year, track)){

				TRACE("No Id3 Info present");
				return;
			}
			
			if(album){

				m_id3Info.SetAlbum(album);
			}

			if(artist){

				m_id3Info.SetArtist(artist);
			}

			if(comment){

				m_id3Info.SetComment(comment);
			}

			if(genre){

				m_id3Info.SetGenre(genre);
			}

			if(title){

				m_id3Info.SetSong(title);
			}

			if(track){

				m_id3Info.SetTrack(atoi(track));
			}

			if(year){

				m_id3Info.SetYear(atoi(year));
			}
		}
	}*/
}

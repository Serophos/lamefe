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
#include "Settings.h"
#include "Resource.h"
#include "Utils.h"

#include "ID3Lib/include/id3.h"
#include "ID3Lib/include/id3/misc_support.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSettings g_sSettings;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMultimediaFile::CMultimediaFile(CString path, CString pluginName)
{

	TRACE("CMultimediaFile::CMultimediaFile() processing file %s\n", path);

	m_bIsEncoded = FALSE;
	m_bRename    = TRUE;
	m_strInputFile = path;
	m_strPluginname = pluginName;
	bAlerted = FALSE;
	
	BOOL bID3read = FALSE;

	CString strTmp = path.Right(3);
	strTmp.MakeLower();

	//
	CException e;
	TRY{
	
		if((strTmp == "mp3") || (strTmp == "ape")){
		//Look for ID3 Tag in file
			ID3_Tag		id3Tag;
			id3Tag.Link(path);

			if(id3Tag.HasV1Tag() || id3Tag.HasV2Tag()){

				char *tmp = NULL;

				tmp = ID3_GetTitle(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetSong(tmp);
				}

				delete tmp;
				tmp = NULL;
				tmp = ID3_GetAlbum(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetAlbum(tmp);
				}
				
				delete tmp;
				tmp = NULL;
				tmp = ID3_GetArtist(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetArtist(tmp);
				}
				
				delete tmp;
				tmp = NULL;
				tmp = ID3_GetComment(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetComment(tmp);
				}
				
				delete tmp;
				tmp = NULL;
				tmp = ID3_GetGenre(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetGenre(tmp);
				}
				
				delete tmp;
				tmp = NULL;
				tmp = ID3_GetTrack(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetTrack(atoi(tmp));
				}
					
				delete tmp;
				tmp = NULL;
				tmp = ID3_GetYear(&id3Tag);
				if(tmp > NULL){

					m_id3Info.SetYear(atoi(tmp));
				}
				
				delete tmp;
				tmp = NULL;

				bID3read = TRUE;
			}
			else{

				bID3read = FALSE;
			}
		}
	}
	CATCH_ALL(e){

		TRACE("CMultimediaFile::CMultimediaFile() --> Exception while rading ID3 Tag\n");
		bID3read = FALSE;

	}END_CATCH_ALL
	
	if(!bID3read){

		CString strTmp;
		strTmp = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
		strTmp = strTmp.Left(strTmp.GetLength() - 4);
		m_id3Info.SetSong(strTmp);
	}


}

CMultimediaFile::~CMultimediaFile()
{

}

CString CMultimediaFile::GetSaveAs(CString wdir, CString strExt)
{

	CString strPath, strSaveAs, strFormat;
	strPath = g_sSettings.GetBasePath();

	if(strPath.IsEmpty()){

		strPath = wdir;
	}

	if(!(m_bRename && g_sSettings.GetRenameFiles())){
		
		CString out = m_strInputFile.Left(m_strInputFile.GetLength() - 4);
		out = out.Right(out.GetLength() - out.ReverseFind('\\'));
		strSaveAs = strPath + "\\" + out + strExt;
		return strSaveAs;
	}

	strFormat = g_sSettings.GetFilename();

	strFormat = Utils::CreateFilename(&m_id3Info, strFormat, strExt);
	//Validate filename length
	if(strFormat.GetLength() > _MAX_FNAME){

		TRACE("Filename is too long. Saving as inputfilename.\n");

		CString tmp, msg;
		CString out = m_strInputFile.Left(m_strInputFile.GetLength() - 4);
		out = out.Right(out.GetLength() - out.ReverseFind('\\'));

		if(!bAlerted){
		
			msg.Format(IDS_ERR_LONGFILENAME, strPath + tmp, strPath + out + strExt);
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



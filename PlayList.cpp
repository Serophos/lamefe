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
#include "PlayList.h"
#include "Settings.h"
#include "resource.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSettings g_sSettings;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CPlayList::CPlayList(CCompactDisc *cd)
{
	
	m_cd  = cd;
	m_mmf = NULL;
}

CPlayList::CPlayList(CMMFArray *mmf)
{

	m_mmf = mmf;
	m_cd  = NULL;
}

CPlayList::~CPlayList()
{

	if(m_mmf)
		delete m_mmf;
	if(m_cd)
		delete m_cd;
}

BOOL CPlayList::WriteToDisc(CString wd, CString strExtension, BOOL bInternal, BOOL bAlbumMode )
{

 	CString strFilename;
 	if(bInternal)
 		strFilename = wd + "\\LameFE.m3u";
 	else
 		strFilename = GetFilename(wd);
 
 	CStdioFile m3u;
 	CFileException f;
 	
 	int *pIndex = 0;
 	int iCnt = 0;
 
 	if(m_cd){
 
 		iCnt	= m_cd->GetNumSelTracks();
 		pIndex	= new int[iCnt];
 		m_cd->GetSelItems(iCnt, pIndex);
 	}
 	else if(m_mmf){
 
 		iCnt = m_mmf->GetSize();
 	}
 	TRY{
 
 		m3u.Open(strFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &f);
 		
 		if(m_cd){
 
 			if(bAlbumMode){
 
 				m3u.WriteString(m_cd->GetAlbumString(wd, strExtension) + "\n");
 			}
 			else{
 
 				for(int i = 0; i < iCnt; i++){
 			
 					m3u.WriteString(m_cd->GetSaveAs(m_cd->GetCDTrack(pIndex[i])->m_btTrack, wd, strExtension) + "\n");
 				}
 			}
 		}
 		else if(m_mmf){
 
 			for(int i = 0; i < iCnt; i++){
 
 				m3u.WriteString(m_mmf->GetAt(i)->GetSaveAs(wd, strExtension) + "\n");
			}
 		}
 
 		m3u.Flush();
 		m3u.Close();
 	}
 	CATCH(CFileException, f){
 
		TRACE("CPlaylist::WriteToDisc(): Error during fileoperation\n");
 		return FALSE;
 	}
 	END_CATCH;
 
 	if(m_cd){
 
 		delete pIndex;
 	}
 
 
 	return TRUE;
}

//DEL CString CPlayList::GetValue(CString val)
//DEL  {
//DEL  
//DEL  	CString  tmp = "";
//DEL  	CID3Info *id3 = NULL;
//DEL  
//DEL  	if(m_cd)
//DEL  		id3 = &m_cd->GetCDTrack(0)->m_id3Info;
//DEL  	else
//DEL  		id3 = &m_mmf->GetAt(0)->m_id3Info;
//DEL  
//DEL  	if(val == "%1"){
//DEL  
//DEL  		tmp = id3->GetArtist();
//DEL  	}
//DEL  	if(val == "%2"){
//DEL  
//DEL  		tmp = id3->GetSong();
//DEL  	}
//DEL  	if(val == "%3"){
//DEL  
//DEL  		tmp = id3->GetAlbum();
//DEL  	}
//DEL  	if(val == "%4"){
//DEL  
//DEL  		tmp.Format("%d", id3->GetYear());
//DEL  	}
//DEL  	if(val == "%5"){
//DEL  
//DEL  		tmp = id3->GetGenre();
//DEL  	}
//DEL  	if(val == "%a"){
//DEL  
//DEL  		tmp.Format("%02d", 1);
//DEL  	}
//DEL  	if(val == "%b"){
//DEL  
//DEL  		tmp.Format("%02d", 1);
//DEL  	}
//DEL  	if(val == "%c"){
//DEL  
//DEL  		if(m_cd)
//DEL  			tmp.Format("0x%X", m_cd->GetDiscID());
//DEL  		else
//DEL  			tmp.Format("0x%X", 0);
//DEL  
//DEL  	}
//DEL  	if(val == "%d"){
//DEL  
//DEL  		if(m_cd)
//DEL  			tmp.Format("0x%X", m_cd->GetVolID());
//DEL  		else
//DEL  			tmp.Format("0x%X", 0);
//DEL  	}
//DEL  
//DEL  	tmp.Replace('/', '-');
//DEL  	tmp.Remove('*');
//DEL  	tmp.Remove('?');
//DEL  	tmp.Replace('\"', '\'');
//DEL  	tmp.Replace('<', '(');
//DEL  	tmp.Replace('>', ')');
//DEL  	tmp.Replace('|', '-');
//DEL  	tmp.Remove(':');
//DEL  	tmp.Remove('\\');
//DEL  
//DEL  	return tmp;
//DEL  }

 CString CPlayList::GetFilename(CString wdir)
 {
 

 
 	CString strPath, strSaveAs, strFormat;
 	CString ext = ".m3u";
 
 	strPath = g_sSettings.GetBasePath();
 
 	if(strPath.IsEmpty()){
 
 		strPath = wdir;
 	}
 
 	strFormat = g_sSettings.GetPlaylistFilename();
 
 	CString tmp;
	
	CID3Info *id3Info = NULL;

	if(m_cd){

		id3Info = &m_cd->GetCDTrack(0)->m_id3Info;
	}
	else{

		id3Info = &m_mmf->GetAt(0)->m_id3Info;
	}

	tmp = Utils::CreateFilename(id3Info, strFormat, ext, (m_cd ? m_cd->GetVolID() : 0), 
		(m_cd ? m_cd->GetDiscID() : 0), 1, (m_cd ? m_cd->GetNumAudioTracks() : 1));

 /*	int lastMatch = 0;
 	int actualMatch = 0;
 	while(TRUE){
 
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
 	if(tmp.GetLength() > _MAX_FNAME){
 
 		TRACE("Filename is too long. Saving as CD-VolID.\n");
 
 
 		if(!m_bAlerted){
 
 			CString msg;
 			msg.Format(IDS_ERR_LONGFILENAME, tmp, "LameFE.m3u");
 			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);
 			m_bAlerted = TRUE;
 		}
 		strSaveAs = strPath + "\\" + "LameFE" + ext;
 
 		if(strSaveAs.GetLength() > _MAX_FNAME){
 
 			AfxMessageBox("The filename is still too long.\n");
 			ASSERT(FALSE);
 		}
 
 		return strSaveAs;	
 	}
 
 	if(!tmp.IsEmpty()){
 		
 		strSaveAs.Empty();
 		strSaveAs = strPath + "\\" + tmp + ext;
 	}	
 
 	return strSaveAs;
 }

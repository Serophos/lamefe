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
#include "MP3File.h"
#include "cfgFile.h"
#include "Resource.h"
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define TRACKSPERSEC 75
//#define CB_CDDASECTOR 2352

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

MP3File::MP3File(CString filename, CString wdr)
{

	id3Tag = new ID3Info();

	wfx.wFormatTag		 = NULL; 
	wfx.nChannels		 = NULL; 
	wfx.nSamplesPerSec	 = NULL; 
	wfx.nAvgBytesPerSec  = NULL; 
	wfx.nBlockAlign		 = NULL; 
	wfx.wBitsPerSample   = NULL; 
	wfx.cbSize			 = NULL; 

	valid				 = false;
	encoded				 = false;
	mp3Output            = true;
	rename				 = true;
	error_string		 = "";
	m_filename           = filename;
	m_fileshort          = filename.Right(filename.GetLength() - filename.ReverseFind('\\') - 1);
	cdTrack				 = -1;


	valid = validateWaveFormat();
	//cfgFile lcfg(wd);
	wd = wdr;
	//savePath = lcfg.GetStringValue(OUTPUT);;
	//m_bSubDirs = lcfg.GetValue(SUBDIRS);

	//if(savePath.IsEmpty()){
		
	//	savePath = wd;
	//}

	//m_strFormat = lcfg.GetStringValue(FORMATSTR);

	//m_saveas = savePath + "\\" + m_fileshort.Left(m_fileshort.GetLength() - 3) + "mp3";

	id3Tag->setSong(m_fileshort.Left(m_fileshort.GetLength() - 4));

	CFile f;
	CFileException e;
	TRY{
		f.Open(m_filename, CFile::modeRead, &e);
		m_filesize = (float)f.GetLength();
		f.Close();
	}

	CATCH(CFileException, e){
		CString err;
		err.Format(IDS_FILEOPENERR, savePath);
		AfxMessageBox(err, MB_OK, 0);
		m_filesize = 0;
	} 
	END_CATCH

	//rename = (BOOL)lcfg.GetValue(RENAME, FALSE);
}

MP3File::MP3File(TOCENTRY tocEntry, CString wdr,  BOOL mp3OrWave){ // mp3OrWave = TRUE = mp3Output yes :)


	id3Tag    = new ID3Info();
	cdTrack   = tocEntry.btTrackNumber;
	startSect = tocEntry.dwStartSector;
	endSect	  = CR_GetTocEntry(cdTrack).dwStartSector;
	m_filesize = (endSect - startSect) *2352;

	mp3Output = mp3OrWave;

	wfx.wFormatTag		 = 0; 
	wfx.nChannels		 = 2; 
	wfx.nSamplesPerSec	 = 44100; 
	wfx.nAvgBytesPerSec  = 0; 
	wfx.nBlockAlign	     = 0; 
	wfx.wBitsPerSample   = 16; 
	wfx.cbSize			 = 0; 
	

	valid				 = true;
	encoded				 = false;
	rename				 = true;
	
	error_string		 = "";

	m_filename.Format("Track_%d", cdTrack);
	m_fileshort.Format("Track_%d", cdTrack);
	
	//cfgFile lcfg(wd);
	wd = wdr;
	//savePath = lcfg.GetStringValue(OUTPUT);
	//m_bSubDirs = lcfg.GetValue(SUBDIRS);

	//if(savePath.IsEmpty()){
		
	//	savePath = wd;
	//}

	//m_strFormat = lcfg.GetStringValue(FORMATSTR);

	//m_saveas = savePath + "\\" + m_fileshort + (mp3Output ? ".mp3": ".wav");
	
	id3Tag->setSong(m_filename);

	//rename = (BOOL)lcfg.GetValue(RENAME, FALSE);
}



MP3File::~MP3File()
{
	delete id3Tag;
}

ID3Info* MP3File::getID3Info()
{
	
	return id3Tag;
}

void MP3File::setID3Info(ID3Info *id3Info)
{
	
	id3Tag = id3Info;
}


BOOL MP3File::validateWaveFormat()
{
	
	std::string filename = m_filename.GetBuffer(10);
	m_filename.ReleaseBuffer();
	std::ifstream file;
	char csID[16];
	long wfxsize;

	file.open (filename.c_str (), std::ios_base::binary); 
	
    if (file == NULL){
		
		error_string.Format("File not found");
		return false; 
    }
	
    file.read (csID, 16); //skip first 16bytes containing RIFF-Wave stuff, not interesting
	
	//Get PCM-Wave-Header
    file.read ((char*)&wfxsize, 4); 
    file.read ((char*)&wfx, wfxsize);

	file.close();
	
#ifdef _DEBUG
	TRACE("\n#####################################################################");
	TRACE("\nWave file is # %d kHz %d bit %d channels", wfx.nSamplesPerSec, wfx.wBitsPerSample, wfx.nChannels);
	TRACE("\n#####################################################################\n\n");
#endif
	
	if (wfx.wBitsPerSample != 16){
			
		error_string.Format(IDS_INVBITRATE, wfx.wBitsPerSample);
		return false;
	}

	return true;
}


BOOL MP3File::isEncoded()
{
	
	return encoded;
}

BOOL MP3File::isValidateWave()
{

	return valid;
}

CString MP3File::getError()
{
 	
	return error_string;
}



CString MP3File::getFileShort()
{
	
	return m_fileshort;
}

void MP3File::setRename(BOOL aflag)
{
	
	rename = aflag;
}

BOOL MP3File::getRename()
{

	return rename;
}

CString MP3File::getSaveAs()
{

	cfgFile cfg(wd);

/*	if(rename){
*/		
	savePath = (LPCSTR)cfg.GetStringValue(OUTPUT);
	if(savePath.IsEmpty()){

		savePath = wd;
	}

	if(!(rename && cfg.GetValue(RENAME))){
		
		m_saveas = savePath + "\\" + m_fileshort.Left(m_fileshort.GetLength() - 3) + (mp3Output ? ".mp3" : ".wav");
		return m_saveas;
	}

	m_strFormat = (LPCSTR)cfg.GetStringValue(FORMATSTR);

	CString tmp;
	int lastMatch = 0;
	int actualMatch = 0;
	while(TRUE){

		actualMatch = m_strFormat.Find("%", lastMatch);
		if(actualMatch != -1){

			tmp += m_strFormat.Mid(lastMatch, actualMatch - lastMatch) += getValue(m_strFormat.Mid(actualMatch, 2));
			lastMatch = actualMatch + 2;
		}
		else{

			break;
		}
	}
	if(!tmp.IsEmpty()){
		
		m_saveas.Empty();
		m_saveas = savePath + "\\" + tmp + (mp3Output ? ".mp3" : ".wav");
		TRACE(m_saveas + "\n");
	}	
//	}

/*	if(m_bSubDirs){
		
		CString filename = m_saveas.Right(m_saveas.GetLength() - m_saveas.ReverseFind('\\'));
		m_saveas = savePath + "\\" + id3Tag->getArtist() + "\\" + id3Tag->getAlbum() + filename;
	}*/
	
	verifyFilename(m_saveas);
	return m_saveas;

}

CString MP3File::getFileName()
{
	
	return m_filename;
}

float MP3File::getFileSize()
{
	
	
	return  m_filesize;
}

WAVEFORMATEX* MP3File::getWfx()
{

	return &wfx;
}


void MP3File::setErrorString(UINT stringID){

	error_string.LoadString(stringID);
}

void MP3File::setErrorString(CString error)
{
	
	error_string = error;
}

void MP3File::setEncoded(BOOL aflag)
{
	
	encoded = aflag;
}

int MP3File::getCDTrackNum()
{

	return cdTrack;
}

long MP3File::getStartSect()
{
	
	return startSect;
}

long MP3File::getEndSect()
{

	return endSect;
}

void MP3File::UpdateTrackInfo(CString sTrackInfo)
{

}

long MP3File::GetStartSector()
{

	return startSect;
}

long MP3File::GetEndSector()
{

	return endSect;
}

CCDDBTrackPosition MP3File::getTrackDuration()
{


	CCDDBTrackPosition trackPos;

	DWORD dwSector = GetStartSector()+150;

	trackPos.m_nMinute	= (dwSector/(75*60));
	trackPos.m_nSecond	= (dwSector/75)%60;
	trackPos.m_nFrame	= (dwSector%75);

	return trackPos;
}

CString MP3File::getValue(CString val)
{

	if(val == "%a"){

		return id3Tag->getArtist();
	}
	if(val == "%t"){

		return id3Tag->getSong();
	}
	if(val == "%m"){

		return  id3Tag->getAlbum();
	}
	if(val == "%n"){

		CString result;
		result.Format("%d", id3Tag->getTrack());
		return result;
	}
	if(val == "%y"){

		CString result;
		result.Format("%d", id3Tag->getYear());
		return result;
	}

	return "";
}

void MP3File::verifyFilename(CString &strFilename)
{

	strFilename.Replace('/', '-');
	strFilename.Remove('*');
	strFilename.Remove('?');
	strFilename.Replace('\"', '\'');
	strFilename.Replace('<', '(');
	strFilename.Replace('>', ')');
	strFilename.Replace('|', '-');
	if(strFilename.Find(':', 3) != -1){

		strFilename.Remove(':');
		strFilename.Insert(1, ":");
	}
}

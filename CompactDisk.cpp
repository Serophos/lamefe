/*
** Copyright (C) 2002 Thees Winkler
**  
** Parts of this codes are based on code from CDEx (c) 1999-2002 by Albert L. Faber 
** These code segments are marked with an enclosing // (c) A.L. Faber //
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
#include "Resource.h"
#include "CompactDisk.h"
#include "CDRip/CDRip.h"
#include "cfgFile.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma pack(push,1)

enum CDTEXT_PACK_TYPE
{
	CDT_TRACK_TITLE	=0x80, // Title of Album name(ID2=00h) or Track Titles (ID2=01h...63h)
	CDT_PERFORMER	=0x81, // Name(s) of the performer(s) (in ASCII)
	CDT_SONGWRITER	=0x82, // Name(s) of the songwriter(s) (in ASCII)
	CDT_COMPOSER	=0x83, // Name(s) of the composer(s) (in ASCII)
	CDT_ARRANGER	=0x84, // Name(s) of the arranger(s) (in ASCII)
	CDT_MESSAGE		=0x85, // Message(s) from content provider and/or artist (in ASCII)
	CDT_DISC_ID		=0x86, // Disc Identification information
	CDT_GENRE		=0x87, // Genre Identification and Genre information
	CDT_TOC_INFO	=0x88, // Table of Content information
	CDT_SEC_TOC_INFO=0x89, // Second Table of Content information
	CDT_RESERVED_1	=0x8A, // Reserved
	CDT_RESERVED_2	=0x8B, // Reserved
	CDT_RESERVED_3	=0x8C, // Reserved
	CDT_RESERVED_4	=0x8D, // Reserved for content provider only
	CDT_UPC_EAN		=0x8E, // UPC/EAN code of the album and ISRC code of each track
	CDT_SIZE_BLOCK	=0x8F  // Size information of the Block
};

typedef struct CDTEXTPACK_TAG
{
  BYTE packType;
  BYTE trackNumber;
  BYTE sequenceNumber;

  BYTE characterPosition:4;		// character position
  BYTE block			:3;		// block number 0..7
  BYTE bDBC				:1;		// double byte character

  BYTE data[12];
  BYTE crc0;
  BYTE crc1;
} CDTEXTPACK;

#pragma pack(pop)

//static const int LEAD_OUT_NBR = 0xAA;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCompactDisc::CCompactDisc()
{

	m_nAudioTracks = 0;
	m_nDiscID      = 0;
	m_nNumTracks   = 0;
	m_nVolID       = 0;
	m_pSel		   = 0;
	m_bAlerted	   = FALSE;
	
}

CCompactDisc::~CCompactDisc()
{
	
	delete m_pSel;
}

void CCompactDisc::Init()
{

	TRACE( "Entering CCompactDisc::Init()\n");
	CDEX_ERR	bErr = FALSE;
	m_nAudioTracks = 0;
	m_nDiscID      = 0;
	m_nTocEntries  = 0;
	m_nVolID       = 0;
	m_cdTracks.RemoveAll();

	int			nAudioTracks = 0;
	
	// Get information from CD-ROM
	bErr = CR_ReadToc();

	// Get the number of 
	m_nTocEntries = CR_GetNumTocEntries();

	m_strMCDI = "";

	for (int i=0;i <= m_nTocEntries;i++)
	{
		TOCENTRY myTocEntry;
		CCDTrack track;
		myTocEntry = CR_GetTocEntry(i);
		
		track.m_btFlags			=	myTocEntry.btFlag;
		track.m_btTrack			=	myTocEntry.btTrackNumber;
		track.m_dwStartSector	=	myTocEntry.dwStartSector;

		if ( ( 0 == i ) && myTocEntry.dwStartSector >= 80 * 60 * TRACKSPERSEC )
		{
			track.m_dwStartSector = 0;
		}

		CString tmp;
		tmp.Format((track.IsAudioTrack() ? IDS_MAIN_AUDIOTRACK : IDS_MAIN_DATATRACK), track.m_btTrack);
		track.m_id3Info.SetSong(tmp);
		m_cdTracks.Add(track);

	}
	m_nVolID = CalculateVolID();
	m_strVolID.Format("%X", m_nVolID);
	
	m_nDiscID = CalculateDiscID();
}

int CCompactDisc::GetVolID()
{

	return m_nVolID;
}

int CCompactDisc::GetDiscID()
{

	return m_nDiscID;
}

CString CCompactDisc::GetStrVolID()
{

	return m_strVolID;
}

int CCompactDisc::GetNumTracks()
{

	return m_nTocEntries;
}

int CCompactDisc::GetNumAudioTracks()
{

	return m_nAudioTracks;
}

int CCompactDisc::GetNumSelTracks(){

	return m_nNumTracks;
}

int CCompactDisc::CalculateVolID()
{

	// Get proper volume ID, and fix TOC using MCI
	// Compat = 1 for use with CDPLAYER.EXE 
	DWORD	dwCompat=1;
	DWORD	dwVolID=0;
	DWORD dwMagic=0;

	int		nAudioTracks = 0;
	CString strTmp;

/////////////////
// (c) A.L. Faber
	for (int i=0; i < m_nTocEntries; i++)
	{

		DWORD dwSector= m_cdTracks[i].m_dwStartSector + 150;

		// Transform to MSF
		DWORD dwMin = (dwSector / (TRACKSPERSEC * 60));
		DWORD dwSec = (dwSector / TRACKSPERSEC) %60;
		DWORD dwFrm = (dwSector % TRACKSPERSEC);

		// check if we cross a section border, if so, were done
		if ((i > 1) && ((m_cdTracks[i].m_btFlags & CDROMDATAFLAG) != 
			            (m_cdTracks[i-1].m_btFlags & CDROMDATAFLAG))){

			break;
		}

		// Add to total
		dwVolID += (dwMin << 16) + (dwSec << 8) + dwFrm;

		if (i == 0){

			dwMagic = dwFrm;
		}
	}

	switch (i){

		case 1:
		case 2:
			dwVolID += -dwMagic + m_cdTracks[i].m_dwStartSector;
		break;
		default: // No correction required
		break;
	}

	for (i = 0; i <= m_nTocEntries; i++ )
	{
		// check if this is an audio track
	    if( m_cdTracks[i].m_btFlags & CDROMDATAFLAG)
		{
			// Assume we have crossed a session boundary
			strTmp.Format("+%X", m_cdTracks[i].m_dwStartSector - 11100);
			m_strMCDI += strTmp;
			break;
		}

		strTmp.Format("+%X", m_cdTracks[i].m_dwStartSector + 150);
		m_strMCDI += strTmp;

		// check for lead-out
		if(m_cdTracks[i].m_btTrack == LEAD_OUT_NBR)
		{
			break;
		}

		// increase number of audio tracks
	    m_nAudioTracks++;
	}

	// Format (in hex) the total number of audio tracks 
	strTmp.Format("%X", m_nAudioTracks );

	// Add number of audio tracks to MCDI string
	m_strMCDI = strTmp + m_strMCDI;

	// If this contains only one track, add track length, IN SECTORS!
	/*if (m_nTocEntries < 2)
	{
		// Add to total
		dwVolID += m_cdTracks[i].m_dwStartSector;
	}*/

// end (c) A.L. Faber
/////////////////

	return dwVolID;
}

int CCompactDisc::CalculateDiscID()
{

	DWORD dwRet;
	DWORD t = 0;
	DWORD n = 0;
	DWORD	dwTwoSecOffset=0;

	// For backward compatibility this algorithm must not change
	dwTwoSecOffset = 2 * TRACKSPERSEC;

	for (int i = 0; i < m_nTocEntries; i++) 
	{
		
		//TRACE("i=%d, size=%d\n", i, m_pFiles->GetSize());
		//Keep in mind the two seconds offset
		DWORD dwSectors = m_cdTracks[i].m_dwStartSector + dwTwoSecOffset;

		n += CCompactDisc::cddb_sum(dwSectors/TRACKSPERSEC);

		// Keep in mind the two seconds offset
		DWORD dwSectorsNext = m_cdTracks[i + 1].m_dwStartSector + dwTwoSecOffset;
		t += (dwSectorsNext / TRACKSPERSEC - dwSectors / TRACKSPERSEC);
	}

	dwRet=( (n % 0xff) << 24 | t << 8 | (DWORD)(m_nTocEntries));

	//Get total playing time
	//m_dwTotalSecs=(GetEndSector(m_nNumTracks)+1+150)/75;
	return dwRet;
}

int CCompactDisc::cddb_sum(int n)
{

	char	buf[12],*p;
	int	ret = 0;

	// For backward compatibility this algorithm must not change
	sprintf(buf, "%lu", n);
	for (p = buf; *p != '\0'; p++)
		ret += (*p - '0');

	return ret;
}

CCDTrack* CCompactDisc::GetCDTrack(int n)
{

	return &m_cdTracks[n];
}

void CCompactDisc::Eject()
{

	m_nAudioTracks = 0;
	m_nDiscID      = 0;
	m_nTocEntries  = 0;
	m_nVolID       = 0;
	m_cdTracks.RemoveAll();
}


void CCompactDisc::SetSelItems(int iCnt, LPINT pIndex)
{
	m_nNumTracks = iCnt;

	m_pSel = new int[iCnt];
	
	for(int i = 0; i < iCnt; i++){

		m_pSel[i]	 = pIndex[i];
	}
}

void CCompactDisc::GetSelItems(int iCnt, LPINT pIndex)
{

	for(int i = 0; i < iCnt; i++){

		pIndex[i] = m_pSel[i];
	}
}

BOOL CCompactDisc::ReadCDText()
{

	CDEX_ERR	bErr=FALSE;
	const int	nBufferSize=4 + 8 * sizeof(CDTEXTPACK) * 256;
	BYTE*		pbtBuffer=NULL;
	INT			nCDTextSize=0;
	INT			nTrack=0;
	CHAR*		lpZero = NULL;

	TRACE("Entering CCompactDisc::ReadCDText()\n");

/////////////////
// (c) A.L. Faber
	// Allocate CD-Text buffer
	pbtBuffer=new BYTE[ nBufferSize ];

	// Try to Read CD-Text section
	bErr=CR_ReadCDText( pbtBuffer, nBufferSize, &nCDTextSize );

	// Check size CD-Text buffer
	if ( nCDTextSize < 4 )
		bErr = CDEX_ERROR;

	// Did it succeed ?
	if ( CDEX_OK == bErr )
	{
		int			i;
		int			nNumPacks=(nCDTextSize-4)/sizeof(CDTEXTPACK);
		CDTEXTPACK* pCDtextPacks=NULL;
		char		lpszBuffer[1024]={'\0',};
		int			nInsertPos=0;

		TRACE("CDInfo::ReadCDText nCDTextSize = %d, nNumPacks = %d\n", nCDTextSize, nNumPacks );

		/* loop through all the packets */
		for ( i=0; i < nNumPacks; i++ )
		{
			/* get CD-Text packet overlay */
			pCDtextPacks=(CDTEXTPACK*)&pbtBuffer[ i * sizeof(CDTEXTPACK) + 4 ];

			TRACE("CDInfo::ReadCDText pack %02X packType %02X trackNumber %02X sequenceNumber %02X characterPosition %02X block %02X bDBC %02X\n",
				i,
				pCDtextPacks->packType,
				pCDtextPacks->trackNumber,
				pCDtextPacks->sequenceNumber,
				pCDtextPacks->characterPosition,
				pCDtextPacks->block,
				pCDtextPacks->bDBC );

			TRACE("data %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n" ,
				pCDtextPacks->data[0],
				pCDtextPacks->data[1],
				pCDtextPacks->data[2],
				pCDtextPacks->data[3],
				pCDtextPacks->data[4],
				pCDtextPacks->data[5],
				pCDtextPacks->data[6],
				pCDtextPacks->data[7],
				pCDtextPacks->data[8],
				pCDtextPacks->data[9],
				pCDtextPacks->data[10],
				pCDtextPacks->data[11] );


			/* only get block number zero */
			if ( 0 == pCDtextPacks->block )
			{
				int j=0;

				for ( j = 0; j < 12; j++ )
				{
					lpszBuffer[nInsertPos++] = pCDtextPacks->data[j];
				}

				// process all strings
				while ( nInsertPos > 0 &&
						NULL != ( lpZero = (CHAR*)memchr( lpszBuffer, '\0', nInsertPos ) ) )
				{
					int nOut = ( lpZero - lpszBuffer ) + 1 ;

					TRACE( "CDTextEntry: type = %d, trackNumber = %d value = \"%s\" \n" ,
								pCDtextPacks->packType,
								pCDtextPacks->trackNumber,
								lpszBuffer );
					
					if ( CDT_TRACK_TITLE == pCDtextPacks->packType )
					{
						if ( 0 == pCDtextPacks->trackNumber ){

							for(int j = 0; j < m_cdTracks.GetSize(); j++){
								
								m_cdTracks[j].m_id3Info.SetAlbum( CString(lpszBuffer) );
							}
						}
						else{

							m_cdTracks[pCDtextPacks->trackNumber-1].m_id3Info.SetSong(CString(lpszBuffer));
						}
					}
					if ( CDT_PERFORMER == pCDtextPacks->packType )
					{
						if (pCDtextPacks->trackNumber==0){

							for(int k = 0; k < m_cdTracks.GetSize(); k++){
								
								m_cdTracks[k].m_id3Info.SetArtist(CString(lpszBuffer));
							}
						}
					}

					nInsertPos -= nOut;
					memmove( lpszBuffer, lpZero + 1, 1024 - nOut -1   );
					pCDtextPacks->trackNumber++;

					while ( nInsertPos > 0 && lpszBuffer[ 0 ] == '\0' )
					{
						memmove( lpszBuffer, lpszBuffer + 1, 1024 -1  );
						nInsertPos--;
					}

				}
			}
		}
	}


	// Delete CD-Text buffer
	delete[] pbtBuffer;

// end (c) A.L. Faber
/////////////////
	TRACE( "Leaving CCompactDisc::ReadCDText()\n" );

	return ( CDEX_OK == bErr );
}

CString CCompactDisc::GetAlbumString(CString wdir, CString ext)
{

	cfgFile cfg(wdir);

	CString strPath, strSaveAs, strFormat;

	strPath = cfg.GetStringValue("output");

	if(strPath.IsEmpty()){

		strPath = wdir;
	}

	strFormat = (LPCSTR)cfg.GetStringValue("albumstr");

	CString tmp;
/*	int lastMatch = 0;
	int actualMatch = 0;
	while(TRUE){

		actualMatch = strFormat.Find("%", lastMatch);
		if(actualMatch != -1){

			tmp += strFormat.Mid(lastMatch, actualMatch - lastMatch) += GetValue(0, strFormat.Mid(actualMatch, 2));
			lastMatch = actualMatch + 2;
		}
		else{
			
			tmp += strFormat.Mid(lastMatch, strFormat.GetLength());
			break;
		}
	}
*/	
	strFormat.Replace("%1", GetCDTrack(0)->m_id3Info.GetArtist());
	strFormat.Replace("%2", GetCDTrack(0)->m_id3Info.GetSong());
	strFormat.Replace("%3", GetCDTrack(0)->m_id3Info.GetAlbum());
	strFormat.Replace("%5", GetCDTrack(0)->m_id3Info.GetGenre());

	tmp.Format("0x%X", GetDiscID());
	strFormat.Replace("%c", tmp);
	tmp.Format("0x%X", GetVolID());
	strFormat.Replace("%d", tmp);
	
	tmp.Format("%d", GetCDTrack(0)->m_id3Info.GetYear());
	strFormat.Replace("%4", tmp);
	//tmp.Format("%02d", 1);
	strFormat.Replace("%a", "01");
	tmp.Format("%02d", GetNumAudioTracks());
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

	if(strFormat.GetLength() > _MAX_FNAME){

		TRACE("Filename is too long. Saving as CD-VolID.\n");

		CString tmp2;
		tmp2.Format("Disc 0x%X", GetVolID());

		if(!m_bAlerted){

			CString msg;
			msg.Format(IDS_ERR_LONGFILENAME, tmp,tmp2);
			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);
			m_bAlerted = TRUE;
		}
		strSaveAs = strPath + "\\" + tmp2 + ext;

		if(strSaveAs.GetLength() > _MAX_FNAME){

			AfxMessageBox("The filename is still too long.\n");
			ASSERT(FALSE);
		}

		return strSaveAs;	
	}

	if(!strFormat.IsEmpty()){
		
		strSaveAs.Empty();
		strSaveAs = strPath + "\\" + strFormat + ext;
	}	

	return strSaveAs;
}

//DEL CString CCompactDisc::GetValue(int nTrack, CString val)
//DEL {
//DEL 
//DEL 	CString tmp = "";
//DEL 	if(val == "%1"){
//DEL 
//DEL 		tmp = GetCDTrack(nTrack)->m_id3Info.GetArtist();
//DEL 	}
//DEL 	if(val == "%2"){
//DEL 
//DEL 		tmp = GetCDTrack(nTrack)->m_id3Info.GetSong();
//DEL 	}
//DEL 	if(val == "%3"){
//DEL 
//DEL 		tmp = GetCDTrack(nTrack)->m_id3Info.GetAlbum();
//DEL 	}
//DEL 	if(val == "%4"){
//DEL 
//DEL 		tmp.Format("%d", GetCDTrack(nTrack)->m_id3Info.GetYear());
//DEL 	}
//DEL 	if(val == "%5"){
//DEL 
//DEL 		tmp.Format("%s", GetCDTrack(nTrack)->m_id3Info.GetGenre());
//DEL 	}
//DEL 	if(val == "%a"){
//DEL 
//DEL 		tmp.Format("%02d", nTrack + 1);
//DEL 	}
//DEL 	if(val == "%b"){
//DEL 
//DEL 		tmp.Format("%02d", GetNumAudioTracks());
//DEL 	}
//DEL 	if(val == "%c"){
//DEL 
//DEL 		tmp.Format("0x%X", GetDiscID());
//DEL 	}
//DEL 	if(val == "%d"){
//DEL 
//DEL 		tmp.Format("0x%X", GetVolID());
//DEL 	}
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
//DEL 	return tmp;
//DEL }

CString CCompactDisc::GetSaveAs(int nTrack, CString wd, CString ext)
{

	cfgFile cfg(wd);
	nTrack--;	// nTrack is not zero-index-based so decrease nTrack
	CString strPath, strSaveAs, strFormat;
	strPath = cfg.GetStringValue("output");

	if(strPath.IsEmpty()){

		strPath = wd;
	}

	if(!(GetCDTrack(nTrack)->GetRename() && cfg.GetValue("rename"))){
		
		strSaveAs = strPath + "\\" + GetCDTrack(nTrack)->GetTrackname() + ext;
		return strSaveAs;
	}

	strFormat = (LPCSTR)cfg.GetStringValue("formatstr");

	CString tmp;
/*	int lastMatch = 0;
	int actualMatch = 0;
	while(TRUE){

		actualMatch = strFormat.Find("%", lastMatch);
		if(actualMatch != -1){

			tmp += strFormat.Mid(lastMatch, actualMatch - lastMatch) += GetValue(nTrack, strFormat.Mid(actualMatch, 2));
			lastMatch = actualMatch + 2;
		}
		else{
			
			tmp += strFormat.Mid(lastMatch, strFormat.GetLength());
			break;
		}
	}
*/
	strFormat.Replace("%1", GetCDTrack(nTrack)->m_id3Info.GetArtist());
	strFormat.Replace("%2", GetCDTrack(nTrack)->m_id3Info.GetSong());
	strFormat.Replace("%3", GetCDTrack(nTrack)->m_id3Info.GetAlbum());
	strFormat.Replace("%5", GetCDTrack(nTrack)->m_id3Info.GetGenre());

	tmp.Format("0x%X", GetDiscID());
	strFormat.Replace("%c", tmp);
	tmp.Format("0x%X", GetVolID());
	strFormat.Replace("%d", tmp);
	
	tmp.Format("%d", GetCDTrack(nTrack)->m_id3Info.GetYear());
	strFormat.Replace("%4", tmp);
	tmp.Format("%02d", nTrack + 1);
	strFormat.Replace("%a", tmp);
	tmp.Format("%02d", GetNumAudioTracks());
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
	
	if(strFormat.GetLength() > _MAX_FNAME){

		TRACE("Filename is too long. Saving as inputfilename.\n");
		
		if(!GetCDTrack(nTrack)->GetAlerted()){

			CString msg;
			msg.Format(IDS_ERR_LONGFILENAME, tmp, GetCDTrack(nTrack)->GetTrackname());
			AfxMessageBox(msg, MB_OK+MB_ICONEXCLAMATION);
			GetCDTrack(nTrack)->SetAlerted();
		}
		strSaveAs = strPath + "\\" + GetCDTrack(nTrack)->GetTrackname()+ ext;

		if(strSaveAs.GetLength() > _MAX_FNAME){

			AfxMessageBox("The filename is still too long.\n");
			ASSERT(FALSE);
		}

		return strSaveAs;	
	}
	if(!strFormat.IsEmpty()){
		
		strSaveAs.Empty();
		strSaveAs = strPath + "\\" + strFormat + ext;
	}	

	return strSaveAs;
}

BOOL CCompactDisc::WriteCueSheet(CString strWd, CString fileExt)
{

	TRACE("CCompactDisc::WriteCueSheet( ... )\n");
	CString strCueFilename = GetAlbumString(strWd, ".cue");
	CString strBinFilename = GetAlbumString(strWd, fileExt);
	strBinFilename = strBinFilename.Right(strBinFilename.GetLength() - strBinFilename.ReverseFind('\\') - 1);
	CString strTmp;
	
	fileExt.MakeUpper();
	fileExt.Remove('.');

	TRY{

		CStdioFile	csfCueFile(strCueFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		
		strTmp.Format("PERFORMER \"%s\"\nTITLE \"%s\"\nFILE \"%s\" %s\n",
				GetCDTrack(0)->m_id3Info.GetArtist(),
				GetCDTrack(0)->m_id3Info.GetAlbum(),
				strBinFilename,
				fileExt
			);
		csfCueFile.WriteString(strTmp);
		TRACE(strTmp);
		strTmp.Empty();

		for(int i = 0; i < GetLastAudioTrack() + 1; i++){

			strTmp.Format("\tTRACK %02d %s\n\t\tTITLE \"%s\"\n\t\tINDEX 01 %s\n",
					i + 1,								// Track Number
					"AUDIO",							// Track Type
					GetCDTrack(i)->m_id3Info.GetSong(), // TITLE
					GetStartTime(i)						// Time String
				);
			csfCueFile.WriteString(strTmp);
			TRACE(strTmp);
			strTmp.Empty();
		}
		csfCueFile.Flush();
		csfCueFile.Close();
	}
	CATCH(CFileException, e){

		TRACE("Error during file operation. CFileExcption!\n");
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

CString CCompactDisc::GetStartTime(int nTrack)
{

	int		nMins;
	int		nSecs;
	int		nFrame;
	CString strTime;

	strTime = "00:00.00";

	DWORD dwSectors = GetCDTrack(nTrack)->m_dwStartSector;

	nMins	= dwSectors / (TRACKSPERSEC*60L);
	nSecs	= (int)(fmod(dwSectors, 60*TRACKSPERSEC) / TRACKSPERSEC);
	nFrame	= dwSectors % TRACKSPERSEC;

	strTime.Format("%u:%02u.%02u", nMins, nSecs, nFrame);

	return strTime;
}

CString CCompactDisc::GetTrackDurationMS(int nTrack)
{

	int		nMins;
	int		nSecs;
	int		nMillisecs;
	CString strMillisecs;

	strMillisecs = "0";

	DWORD dwSectors = GetCDTrack(nTrack + 1)->m_dwStartSector - GetCDTrack(nTrack)->m_dwStartSector;

	nMins	= dwSectors / (TRACKSPERSEC*60L);
	nSecs	= (int)(fmod(dwSectors, 60*TRACKSPERSEC) / TRACKSPERSEC);

	nMillisecs = (nMins * 60 * 1000) + (nSecs * 1000); 

	strMillisecs.Format("%d", nMillisecs);

	return nMillisecs;	
}

int CCompactDisc::GetLastAudioTrack()
{

	int nResult = m_nTocEntries - 1;
	
	while(!m_cdTracks.GetAt(nResult).IsAudioTrack()){

		nResult--;
	}

	return nResult;
}

DWORD CCompactDisc::GetEndSector(int nTrack)
{

	int i;
	DWORD	dwEndSector = 0;

/////////////////
// (c) A.L. Faber
	// Loop through the available tracks
	for (i = 1; i <= m_nTocEntries; i++){

		if ((m_cdTracks[i-1].m_btTrack == nTrack) || (m_cdTracks[i-1].m_btTrack == LEAD_OUT_NBR)){

			// check if we cross a session border
			// and check if the next track is not the default audio lead-out
			// but not the first track, due to extra CD format
			// where first track could be a data track, the second an audio track
			if (((m_cdTracks[i-1].m_btFlags & CDROMDATAFLAG) != (m_cdTracks[i].m_btFlags & CDROMDATAFLAG)) &&
				 (m_cdTracks[i].m_btTrack != LEAD_OUT_NBR) ){

				// If we have crossed a session border, we get an additional leadout
				// Session lead-out = 1m30s => 6750 sectors
				// Session lead-in  = 1m00s => 4500 sectors
				TRACE("Correct end-sector of track %d for session lead-out time by 11250 sectors\n", i + 1 );
				dwEndSector = m_cdTracks[i].m_dwStartSector - 11250 - 1;
			}
			else{

				dwEndSector = m_cdTracks[i].m_dwStartSector - 1;
			}

			break;
		}
	}
// end (c) A.L. Faber
/////////////////
	return dwEndSector;
}

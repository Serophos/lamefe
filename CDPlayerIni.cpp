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
#include "CDPlayerIni.h"
#include "CDTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCDPlayerIni::CCDPlayerIni(CCompactDisc *pFiles)
{

	m_pFiles = pFiles;
}

CCDPlayerIni::~CCDPlayerIni()
{

}

BOOL CCDPlayerIni::Read()
{

	if(!ReadFile()){

		return FALSE;
	}
	if(!FindEntry()){
		
		return FALSE;
	}
	SetValues();
	return TRUE;
}

BOOL CCDPlayerIni::FindEntry()
{

	BOOL			bResult = FALSE;
	CString			strWindir;
	CString			strSection;
	strSection.Format("[%X]", m_pFiles->GetVolID());

	for(int i = 0; i < m_strAEntries.GetSize(); i++){

		if(m_strAEntries.GetAt(i).CompareNoCase(strSection) == 0){

			i++;
			while(m_strAEntries.GetAt(i).Left(1) != "[") {

				m_strACurrentCD.Add(m_strAEntries.GetAt(i++));
				
				if(i == m_strAEntries.GetSize()){

					return TRUE;
				}
			}
			return TRUE;
		}
	}

	return bResult;
}



BOOL CCDPlayerIni::GetItem(LPCSTR item, CString &str)
{

	int nEndItem;

	for(int i = 0; i < m_strACurrentCD.GetSize(); i++){

		nEndItem = m_strACurrentCD.GetAt(i).Find("=", 0);
		if(m_strACurrentCD.GetAt(i).Left(nEndItem) == item){

			str = m_strACurrentCD.GetAt(i).Right(m_strACurrentCD.GetAt(i).GetLength() - nEndItem - 1);
			return TRUE;
		}
	}

	return FALSE;
}

void CCDPlayerIni::SetValues()
{

	CString strTmp;
	CString strTrackNum;
	int currentAudioTrack = 0;

	for(int i = 0; i < m_pFiles->GetNumTracks(); i++){
		
		CCDTrack *tmp = m_pFiles->GetCDTrack(i);
		if(!tmp->IsAudioTrack()){
			
			continue;
		}

		if(GetItem("title", strTmp)){

			tmp->m_id3Info.SetAlbum(strTmp);
		}
		if(GetItem("artist", strTmp)){

			tmp->m_id3Info.SetArtist(strTmp);
		}
		
		strTrackNum.Format("%d", currentAudioTrack);
		if(GetItem(strTrackNum.GetBuffer(10), strTmp)){

			tmp->m_id3Info.SetSong(strTmp);
		}
		strTrackNum.ReleaseBuffer();
		tmp->m_id3Info.SetTrack(currentAudioTrack + 1);
		
		if(GetItem("year", strTmp)){

			tmp->m_id3Info.SetYear(atoi(strTmp));
		}
		if(GetItem("genre", strTmp)){

			tmp->m_id3Info.SetGenre(strTmp);
		}

//		delete tmp;
		currentAudioTrack++;
	}

}

/*
[1E45FBE]
entryType=1
artist=Yann Tiersen
title=Le Fabuleux Destin d'Amélie Poulain
numTracks=20
0=J'y suis jamais allé
1=Les jours tristes (instrumental)
2=La valse d'Amélie
3=Comptine d'un autre été : L'après-midi
4=La noyée
5=L'autre valse d'Amélie
6=Guilty
7=A quai
8=Le moulin
9=Pas si simple
10=La valse d'Amélie (version orchestre)
11=La valse des vieux os
12=La dispute
13=Si tu n'étais pas là (Fréhel)
14=Soir de fête
15=La redécouverte
16=Sur le fil
17=Le banquet
18=La valse d'Amélie (version piano)
19=La valse des monstres
numplay=20
year=2001
genre=Soundtrack
*/
int CCDPlayerIni::Write()
{

	if(!ReadFile()){

		return -1;
	}
	if(FindEntry()){

		return 2;
	}

	CStdioFile cdini;
	CFileException* e = NULL;
	CString strTmp;
	char*			windir = new char[256];

	GetWindowsDirectory(windir, 256);
	strcat(windir, "\\CDPlayer.ini");

	TRY{

		if(!cdini.Open(windir, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::typeText, e)){
			
			TRACE("Could not open cdplayer.ini\n");
			return -1;
		}
		cdini.Seek(0, CFile::end);
		strTmp.Format("[%X]\n", m_pFiles->GetVolID());
		cdini.WriteString(strTmp);
		cdini.WriteString("entryType=1\n");
		cdini.WriteString("artist=" + m_pFiles->GetCDTrack(0)->m_id3Info.GetArtist() + "\n");
		cdini.WriteString("title=" + m_pFiles->GetCDTrack(0)->m_id3Info.GetAlbum() + "\n");
		strTmp.Format("numTracks=%d\n", m_pFiles->GetNumTracks());
		cdini.WriteString(strTmp);
		for(int i = 0; i < m_pFiles->GetNumTracks(); i++){
			
			strTmp.Format("%d=%s\n", i, m_pFiles->GetCDTrack(i)->m_id3Info.GetSong());
			cdini.WriteString(strTmp);
		}
		strTmp.Format("numPlay=%d\n", m_pFiles->GetNumTracks());
		cdini.WriteString(strTmp);
		strTmp.Format("year=%d\n", m_pFiles->GetCDTrack(0)->m_id3Info.GetYear());
		cdini.WriteString(strTmp);
		cdini.WriteString("genre=" + m_pFiles->GetCDTrack(0)->m_id3Info.GetGenre() + "\n");
		cdini.WriteString("\n");
		cdini.Flush();
		cdini.Close();

	}
	CATCH(CFileException, e){

		return -1;
	}END_CATCH;

	delete windir;

	return 0;
}

BOOL CCDPlayerIni::ReadFile()
{

	CStdioFile		cdini;
	CFileException* e		= NULL;
	CString			tmp;
	char*			windir = new char[256];

	GetWindowsDirectory(windir, 256);
	strcat(windir, "\\CDPlayer.ini");

	TRY{


		if(!cdini.Open(windir, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeText, e)){

			TRACE("Could not open CDPlayer.ini\n");
			return FALSE;
		}

		while(cdini.GetPosition() != cdini.GetLength()){

			cdini.ReadString(tmp);
			m_strAEntries.Add(tmp);

		}
		
		cdini.Close();

	}
	CATCH(CFileException, e){

		TRACE("Exception during File operation\n");
		return FALSE;
	}
	END_CATCH;
	
	TRACE("Lines: %d\n", m_strAEntries.GetSize());
	delete windir;
	return TRUE;

}

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
#include "Utils.h"
#include <stdio.h>
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef BOOL (PASCAL *GFDPEX )(LPCSTR,PULARGE_INTEGER,PULARGE_INTEGER,PULARGE_INTEGER);

//enum COMCTL32VERSION {COMCTL32_UNKNOWN, COMCTL32_400, COMCTL32_470, COMCTL32_471};


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

Utils::Utils()
{

}

Utils::~Utils()
{

}

BOOL Utils::CheckCOMTL32Dll()
{

	BOOL bReturn = FALSE;
    LPBYTE  lpVersionData; 
    DWORD   dwLangCharset; 

 	TCHAR lpszModuleName[ MAX_PATH + 1 ] = { '\0',};
	
	// Get Comctl32.dll product version
	GetSystemDirectory( lpszModuleName,	MAX_PATH );
	
	strcat(lpszModuleName, "\\COMCTL32.DLL");

	DWORD dwHandle;     
    DWORD dwDataSize = ::GetFileVersionInfoSize(lpszModuleName, &dwHandle); 
    if ( dwDataSize == 0 ){

        return FALSE;
	}

    lpVersionData = new BYTE[dwDataSize]; 
    if(!::GetFileVersionInfo((LPTSTR)lpszModuleName, dwHandle, dwDataSize, (void**)lpVersionData)){

		delete[] lpVersionData; 
		lpVersionData = NULL;
		dwLangCharset = 0;

        return FALSE;
    }

    UINT nQuerySize;
    DWORD* pTransTable;
    if (!::VerQueryValue(lpVersionData, "\\VarFileInfo\\Translation",
                         (void **)&pTransTable, &nQuerySize)){

		delete[] lpVersionData; 
		lpVersionData = NULL;
		dwLangCharset = 0;

        return FALSE;
    }

    // Swap the words to have lang-charset in the correct format
    dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

    // Query version information value
    LPVOID lpData;
    CString strVersion, strBlockName;

    strBlockName.Format(_T("\\StringFileInfo\\%08lx\\%s"), dwLangCharset, "FileVersion");

    if(::VerQueryValue((void **)lpVersionData, strBlockName.GetBuffer(0), &lpData, &nQuerySize)){

        strVersion = (LPCTSTR)lpData;
	}

    strBlockName.ReleaseBuffer();

	float fVersion = 0.0f;
	_stscanf(strVersion, "%f", & fVersion);

	bReturn = (fVersion > 5.80f);

	delete[] lpVersionData; 
	lpVersionData = NULL;
	dwLangCharset = 0;

	return bReturn;
}

DOUBLE Utils::GetMyFreeDiskSpace(CString strPath)
{

	CString strRootDir;
	GFDPEX		GetDiskFreeSpaceOSR2=NULL;

	// Check input parameter
	if (strPath.IsEmpty())
	{
		ASSERT(FALSE);
		return 0;
	}

	// Copy driverletter, colon and back slash
	strRootDir=strPath.Left(3);

	DOUBLE dFreeDiskSpace=0.0;

	// Is this NT or OSR2, then call extented free disk space routine
	if (GetDiskFreeSpaceOSR2!=NULL)
	{
		ULARGE_INTEGER	FreeBytes; 
		ULARGE_INTEGER	TotalNumberOfBytes;
		ULARGE_INTEGER	TotalNumberOfFreeBytes;

		// Make call to kernel 32 dll
		BOOL bReturn=GetDiskFreeSpaceOSR2(strRootDir,&FreeBytes,&TotalNumberOfBytes,&TotalNumberOfFreeBytes);

		// If result is OK, then calculate result
		if (bReturn)
		{
			dFreeDiskSpace=(DOUBLE)FreeBytes.LowPart;
			dFreeDiskSpace+=((DOUBLE)FreeBytes.HighPart)*ULONG_MAX;
		}
	}
	else
	{
		// Standard Windows 95
 		DWORD dwSectorsPerCluster; 
		DWORD dwBytesPerSector; 
		DWORD dwNumberOfFreeClusters; 
		DWORD dwTotalNumberOfClusters;

		// Try the good old GetDiskFreeSpace
		GetDiskFreeSpace(strRootDir,&dwSectorsPerCluster,&dwBytesPerSector,&dwNumberOfFreeClusters,&dwTotalNumberOfClusters);
 
		// Calculate free disk space
		dFreeDiskSpace=(DOUBLE)dwNumberOfFreeClusters*(DOUBLE)dwBytesPerSector*(DOUBLE)dwSectorsPerCluster;

	// Get the free disc space as you already do.
	}
	return dFreeDiskSpace;
}

BOOL Utils::FileExists(CString strFilename)
{

	TRY{
		
		CFileStatus rStatus;
		if(CFile::GetStatus(strFilename, rStatus)){

			return TRUE;
		}
		else{

			return FALSE;
		}
	}
	CATCH(CFileException, e){

		TRACE("File access exception/violation in Utils::FileExist\n");
		return TRUE;
	}
	END_CATCH;

	return FALSE;
}

BOOL Utils::CreateDirs(CString strFilename)
{

	int nStart = 3;
	int nEnd = 0;
	
	CString validated = strFilename.Left(2);
	_chdir(validated);
	
	while(TRUE){

		nEnd = strFilename.Find('\\', nStart);
		if(nEnd == -1){

			return TRUE;
		}

		if(_chdir(validated + "\\" + strFilename.Mid(nStart, nEnd - nStart)) != 0){

			_chdir(validated);
			if(_mkdir(strFilename.Mid(nStart, nEnd - nStart)) != 0){

				return FALSE;
			}
			
		}
		validated += "\\" + strFilename.Mid(nStart, nEnd - nStart);
		nStart = nEnd + 1;		
	}

	return FALSE;
}

CString Utils::CreateFilename(CID3Info *id3Info, CString strFormat, CString strExt, int nVolID, int nDiscID, int nTrack, int nNumAudioTracks)
{

	CString tmp;

	// %1 = Artist
	tmp = id3Info->GetArtist();
	tmp.Remove('\\');
	strFormat.Replace("%1", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%U1", "tmp");
	tmp.MakeLower();
	strFormat.Replace("%L1", "tmp");

	// %2 = Songtitle
	tmp =  id3Info->GetSong();
	tmp.Remove('\\');
	strFormat.Replace("%2", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%U2", "tmp");
	tmp.MakeUpper();
	
	// %3 = Album
	tmp =  id3Info->GetAlbum();
	tmp.Remove('\\');
	strFormat.Replace("%3", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%U3", "tmp");
	tmp.MakeUpper();
	strFormat.Replace("%L3", "tmp");
	
	// %5 = Genre
	tmp = id3Info->GetGenre();
	tmp.Remove('\\');
	strFormat.Replace("%5", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%U5", "tmp");
	tmp.MakeUpper();
	strFormat.Replace("%L5", "tmp");


	// %c = DiscID
	tmp.Format("0x%X", nDiscID);
	strFormat.Replace("%c", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%Uc", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%Lc", tmp);
	// %d = VolID
	tmp.Format("0x%X", nVolID);
	strFormat.Replace("%d", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%Ud", tmp);
	tmp.MakeUpper();
	strFormat.Replace("%Ld", tmp);
	
	// %4 = ID3-Year
	tmp.Format("%d", id3Info->GetYear());
	strFormat.Replace("%4", tmp);
	strFormat.Replace("%U4", tmp);
	strFormat.Replace("%L4", tmp);

	// %a = track
	tmp.Format("%02d", nTrack);
	strFormat.Replace("%Ua", tmp);
	strFormat.Replace("%La", tmp);
	strFormat.Replace("%a", tmp);

	// %a = numtracks
	tmp.Format("%02d", nNumAudioTracks);
	strFormat.Replace("%Ub", tmp);
	strFormat.Replace("%Lb", tmp);
	strFormat.Replace("%b", tmp);

	strFormat.Replace('/', '-');
	strFormat.Remove('*');
	strFormat.Remove('?');
	strFormat.Replace('\"', '\'');
	strFormat.Replace('<', '(');
	strFormat.Replace('>', ')');
	strFormat.Replace('|', '-');
	strFormat.Remove(':');
	//strFormat.Remove('\\');
	//strFormat.Remove('.');

	CString strSaveAs;

	strSaveAs = strFormat; // + strExt;

	return strSaveAs;
}

CString Utils::EncryptString(CString strPlain)
{

	int		nNumCharacters	= 0;
	int*	strArray		= NULL;
	int		nTmp			= 0;
	CString	strTmp			= "";
	CString strEncrypted	= "";

	nNumCharacters  = strPlain.GetLength();
	strArray		= new int[nNumCharacters];

	for(int i = 0; i < nNumCharacters; i++){

		strArray[i] = strPlain.GetAt(i);
	}
	for(i = 0; i < nNumCharacters; i++){

		nTmp = strArray[i];
		nTmp += 20;
		strTmp.Format("%03d", nTmp);
		strEncrypted += strTmp;
	}
	delete strArray;

	return strEncrypted;
}

CString Utils::DecryptString(CString strEncrypted)
{

	int		nNumCharacters	= 0;
	int*	strArray		= NULL;
	int		nTmp			= 0;
	CString	strTmp			= "";
	CString strPlain		= "";

	nNumCharacters	= strEncrypted.GetLength() / 3;
	strArray		= new int[nNumCharacters];

	for(int i = 0; i < nNumCharacters; i++){

		strTmp = strEncrypted.Mid(i*3, 3);
		nTmp = atoi(strTmp);
		strArray[i] = nTmp - 20;
	}
	for(i = 0; i < nNumCharacters; i++){

		nTmp = strArray[i];
		strPlain += (CString)nTmp;
	}

	delete strArray;

	return strPlain;
}

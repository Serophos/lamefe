/*
** Copyright (C) 1999 - 2002 Albert L. Faber
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

#include "StdAfx.h"
#include "Aspi.h"
#include "AspiDebug.h"
#include "NtScsi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE hAspiLib=NULL;
GETASPI32SUPPORTINFO	GetASPI32SupportInfo = NULL;
SENDASPI32COMMAND		SendASPI32Command = NULL;
/*
GETASPI32BUFFER			GetASPI32Buffer=NULL;
FREEASPI32BUFFER		FreeASPI32Buffer=NULL;
GETASPI32DLLVERSION		GetASPI32DLLVersion=NULL;
TRANSLATEASPI32ADDRESS	TranslateASPI32Address;
*/

BOOL bUseNtScsi = FALSE;


VOID *Swap (VOID *p, int size)
{
	char *pc = (char*)p;
	char tmp;

	if (size == 4) 
	{
		tmp = pc [0];
		pc [0] = pc [3];
		pc [3] = tmp;
		tmp = pc [1];
		pc [1] = pc [2];
		pc [2] = tmp;
	} else 
	{
		tmp = pc [0];
		pc [0] = pc [1];
		pc [1] = tmp;
	}
	return p;
}


BYTE IsScsiError( LPSRB lpSrb )
{
	int		nErrorCode=lpSrb->SRB_Status;

	DWORD dwStart=GetTickCount();

	// Check pending state
	while ( (lpSrb->SRB_Status==SS_PENDING) && (( GetTickCount()-dwStart)<TIMEOUT) )
	{
		::Sleep(0);
		nErrorCode=lpSrb->SRB_Status;
	}
	DWORD dwDelta=GetTickCount()-dwStart;
	return nErrorCode;
}


CDEX_ERR InitAspiDll( bool& bUseNtScsi )
{
	CDEX_ERR bReturn = CDEX_OK;
	const int MYMAXPATHLENGTH = 255;
	char lpszPathName[ MYMAXPATHLENGTH ] = {'\0',};

	DebugPrintf( _T( "Entering InitAspiDLL" ) );

	if ( FALSE == bUseNtScsi ) 
	{
		// try to load DLL ( no path )
		strcpy( lpszPathName, _T( "wnaspi32.dll" ) );

		hAspiLib = LoadLibrary( lpszPathName );

		// check result
		if ( NULL == hAspiLib )
		{
			// try to load DLL from the system directory
			GetSystemDirectory( lpszPathName, MYMAXPATHLENGTH );

			strcat( lpszPathName, _T( "\\wnaspi32.dll" ) );
			
			hAspiLib = LoadLibrary( lpszPathName );

			// check result
			if ( NULL == hAspiLib )
			{
				// try to load DLL from the system directory
				GetWindowsDirectory( lpszPathName, MYMAXPATHLENGTH );

				strcat( lpszPathName, _T( "\\wnaspi32.dll" ) );

				hAspiLib = LoadLibrary( lpszPathName );
			}
		}

		// check ASPI results
		if ( NULL == hAspiLib )
		{
			OSVERSIONINFO	osVersion;

			memset( &osVersion, 0x00, sizeof( osVersion ) );

			osVersion.dwOSVersionInfoSize = sizeof( osVersion );

			GetVersionEx( &osVersion );

			if ( VER_PLATFORM_WIN32_NT == osVersion.dwPlatformId )
			{
				bReturn = CDEX_NATIVEEASPISUPPORTEDNOTSELECTED;
			}
			else
			{
				bReturn = CDEX_FAILEDTOLOADASPIDRIVERS;
			}

		}
	}
	else
	{
		// Use native NT SCSI library
		if ( NtScsiInit() > 0 )
		{
			DebugPrintf( _T( "Suggest to use native NT scsi libraries" ) );
			bReturn = CDEX_NATIVEEASPISUPPORTEDNOTSELECTED;
		}
		else
		{
			if ( TRUE == bUseNtScsi )
			{
				bReturn = CDEX_NATIVEEASPINOTSUPPORTED;
			}
			else
			{
				bReturn = CDEX_FAILEDTOLOADASPIDRIVERS;
			}
		}
	}

	if ( CDEX_OK == bReturn )
	{
		if ( FALSE == bUseNtScsi )
		{
			GetASPI32SupportInfo	=(GETASPI32SUPPORTINFO)GetProcAddress(hAspiLib,TEXT_GETASPI32SUPPORTINFO);
			SendASPI32Command		=(SENDASPI32COMMAND)GetProcAddress(hAspiLib,TEXT_SENDASPI32COMMAND);
			//	GetASPI32Buffer			=(GETASPI32BUFFER)GetProcAddress(hAspiLib,TEXT_GETASPI32BUFFER);
			//	FreeASPI32Buffer		=(FREEASPI32BUFFER)GetProcAddress(hAspiLib,TEXT_FREEASPI32BUFFER);
			//	GetASPI32DLLVersion		=(GETASPI32DLLVERSION)GetProcAddress(hAspiLib,TEXT_GETASPI32DLLVERSION);
			//	TranslateASPI32Address	=(TRANSLATEASPI32ADDRESS)GetProcAddress(hAspiLib,TEXT_TRANSLATEASPI32ADDRESS);
		}
		else
		{
			GetASPI32SupportInfo	= NtScsiGetASPI32SupportInfo;
			SendASPI32Command		= NtScsiSendASPI32Command;
		}

		if (	GetASPI32SupportInfo	==NULL	|| 
				SendASPI32Command		==NULL
	//			GetASPI32Buffer			==NULL	||
	//			FreeASPI32Buffer		==NULL	
	//			|| TRUE
	//			GetASPI32DLLVersion		==NULL	|| 
	//			TranslateASPI32Address	==NULL
				)
		{
			DebugPrintf( _T( "InitAspiDLL::Invalid Pointer" ) );

			FreeLibrary( hAspiLib );
			hAspiLib = NULL;
			GetASPI32SupportInfo = NULL;
			SendASPI32Command = NULL;

			bReturn = CDEX_FAILEDTOLOADASPIDRIVERS;
		}
	}

	DebugPrintf( _T( "Leaving InitAspiDLL, return value %d" ), bReturn );
	
	return bReturn;
}

void GetAspiError(int nErrorCode,LPSTR lpszError)
{
	switch (nErrorCode)
	{
		case SS_PENDING:	strcpy(lpszError,"SRB being processed");			break;
		case SS_COMP:		strcpy(lpszError,"SRB completed without error");	break;
		case SS_ABORTED:	strcpy(lpszError,"SRB aborted");					break;
		case SS_ABORT_FAIL:	strcpy(lpszError,"Unable to abort SRB");			break;
		case SS_ERR:		strcpy(lpszError,"SRB completed with error");		break;
		case SS_INVALID_CMD:strcpy(lpszError,"Invalid ASPI command");			break;
		case SS_INVALID_HA:	strcpy(lpszError,"Invalid host adapter number");	break;
		case SS_NO_DEVICE:	strcpy(lpszError,"SCSI device not installed");		break;
		case SS_INVALID_SRB:strcpy(lpszError,"Invalid parameter set in SRB");	break;
		case SS_OLD_MANAGER:strcpy(lpszError,"ASPI manager doesn't support");	break;
//		case SS_BUFFER_ALIGN:strcpy(lpszError,"Buffer not aligned (replaces SS_OLD_MANAGER in Win32"); break;
		case SS_ILLEGAL_MODE:	strcpy(lpszError,"Unsupported Windows mode"); break;
		case SS_NO_ASPI:	strcpy(lpszError,"No ASPI managers"); break;
		case SS_FAILED_INIT:	strcpy(lpszError,"ASPI for windows failed init"); break;
		case SS_ASPI_IS_BUSY:	strcpy(lpszError,"No resources available to execute command"); break;
//		case SS_BUFFER_TO_BIG:
		case SS_BUFFER_TOO_BIG:	strcpy(lpszError,"Buffer size too big to handle"); break;
		case SS_MISMATCHED_COMPONENTS:	strcpy(lpszError,"The DLLs/EXEs of ASPI don't version check"); break;
		case SS_NO_ADAPTERS:	strcpy(lpszError,"No host adapters to manager"); break;
		case SS_INSUFFICIENT_RESOURCES:	strcpy(lpszError,"Couldn't allocate resources needed to init"); break;
		case SS_ASPI_IS_SHUTDOWN:	strcpy(lpszError,"Call came to ASPI after PROCESS_DETACH"); break;
		case SS_BAD_INSTALL:	strcpy(lpszError,"The DLL or other components are installed wrong"); break;
		default: 
			strcpy(lpszError,"Unknow ASPI error"); 
		break;
	}
}

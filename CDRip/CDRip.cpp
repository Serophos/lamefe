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
#include <math.h>

#include "CDRip.h"
#include "CDExtract.h"
#include "AspiDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCDExtract* pExtract=NULL;


// Get the DLL version number
LONG CCONV CR_GetCDRipVersion()
{
	// Get CDex version
//	CFileVersion myVersion;
//	char lpszModuleFileName[MAX_PATH];
//	GetModuleFileName(NULL,lpszModuleFileName,sizeof(lpszModuleFileName));
//	myVersion.Open(lpszModuleFileName);
//	CString strVersion=myVersion.GetProductVersion();
//	return atoi(strVersion);
	return 115;
}

DLLEXPORT CDEX_ERR CCONV CR_DeInit( )
{
	if ( pExtract )
	{
		pExtract->Clear();
		delete pExtract;
		pExtract = NULL;
	}

	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_Init( LPCSTR strIniFname )
{
	CDEX_ERR bReturn = CDEX_OK;

	ASSERT( strIniFname );

	CDRomSettings::SetIniFileName( strIniFname );


	SetDebugLevel( ::GetPrivateProfileInt( "Debug", "DebugCDRip", FALSE, strIniFname ) );

	DebugPrintf("Entering CR_Init, ini file name = %s", strIniFname);



	CR_DeInit();

	DebugPrintf("Create new CCDExtract");
	pExtract=new CCDExtract;

	if ( pExtract == NULL )
	{
		DebugPrintf( _T( "pExtract creation failed!" ) );
		bReturn = CDEX_OUTOFMEMORY;
	}
	else
	{
		// get the INI settings
		pExtract->LoadSettings( FALSE );

		bReturn = pExtract->Init();
	}


	// Check if low level CD-ROM drivers are intialized properly
	if ( pExtract->IsAvailable() )
	{
		// Obtain the specs of the SCSI devices and select the proper CD Device
		bReturn = pExtract->GetCDRomDevices();
	}
	else
	{
		CR_DeInit();
	}

	if ( CDEX_OK == bReturn )
	{
		if ( CR_GetNumCDROM() < 1 )
		{
			CR_DeInit();
			bReturn = CDEX_NOCDROMDEVICES;
		}
		else
		{
			// Set drive zero as default
			CR_SetActiveCDROM( 0 );

			// Get the settings once again to set the active drive
			CR_LoadSettings();
		}
	}


	DebugPrintf( _T( "Leaving CR_Init, return value %d" ), bReturn );

	return bReturn;
}


DLLEXPORT LONG CCONV CR_GetNumCDROM()
{
	if (!pExtract)
		return CDEX_ERROR;

	return pExtract->GetNumDrives();
}

//DLLFUNCTION void CR_SetActiveCDROM(LONG nActiveDrive)
DLLEXPORT void CCONV CR_SetActiveCDROM(LONG nActiveDrive)
{
	DebugPrintf("CR_SetActiveDrive");
	if (pExtract)
		pExtract->SetActiveCDROM(nActiveDrive);
}


DLLEXPORT LONG CCONV CR_GetActiveCDROM()
{
	DebugPrintf("CR_GetActiveCDRom");
	if (!pExtract)
		return CDEX_ERROR;
	return pExtract->GetActiveCDROM();
}

DLLEXPORT CDEX_ERR CCONV CR_SelectCDROMType( DRIVETYPE cdType )
{
	DebugPrintf("CR_SelectCDROMType");
	if (!pExtract)
		return CDEX_ERROR;

	pExtract->SetDriveType(cdType);
	pExtract->UpdateDriveSettings();
	return CDEX_OK;
}

//DLLFUNCTION CDEX_ERR CR_GetCDROMParameters( CDROMPARAMS* pParam)
DLLEXPORT CDEX_ERR CCONV CR_GetCDROMParameters( CDROMPARAMS* pParam)
{
	DebugPrintf("CR_GetCDROMParameters");
	// Clear structure
	memset(pParam,0x00,sizeof(CDROMPARAMS));

	// Bail out if necessary
	if (!pExtract)
		return CDEX_ERROR;

	// Set CDROM ID
	strcpy(pParam->lpszCDROMID,pExtract->GetCDROMID()); 

	pParam->nOffsetStart		= pExtract->GetOffsetStart();
	pParam->nOffsetEnd			= pExtract->GetOffsetEnd();
	pParam->nSpeed				= pExtract->GetSpeed();
	pParam->nSpinUpTime			= pExtract->GetSpinUpTime();
	pParam->bJitterCorrection	= pExtract->GetJitterCorrection();
	pParam->bSwapLefRightChannel= pExtract->GetSwapLefRightChannel();
	pParam->nNumOverlapSectors	= pExtract->GetNumOverlapSectors();
	pParam->DriveTable			= pExtract->GetDriveTable();
	pParam->nNumReadSectors		= pExtract->GetNumReadSectors();
	pParam->nNumCompareSectors	= pExtract->GetNumCompareSectors();
		
	pParam->btTargetID			= pExtract->GetTargetID();
	pParam->btAdapterID			= pExtract->GetAdapterID();
	pParam->btLunID				= pExtract->GetLunID();

	pParam->bAspiPosting		= pExtract->GetAspiPosting();
	pParam->nAspiTimeOut		= pExtract->GetAspiTimeOut();
	pParam->nAspiRetries		= pExtract->GetAspiRetries();

	pParam->bEnableMultiRead    = pExtract->GetMultiReadEnable();
	pParam->nMultiReadCount     = pExtract->GetMultiRead();
	pParam->bMultiReadFirstOnly = pExtract->GetMultiReadFirstOnly();

	pParam->bLockDuringRead		= pExtract->GetLockDuringRead();

	pParam->nRippingMode		= pExtract->GetRippingMode();
	pParam->nParanoiaMode		= pExtract->GetParanoiaMode();

	pParam->bUseCDText			= pExtract->GetUseCDText();

	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_SetCDROMParameters( CDROMPARAMS* pParam)
{
	DebugPrintf("CR_SetCDROMParameters");
	if (!pExtract)
		return CDEX_ERROR;

	CDEX_ERR nErr=CDEX_OK;

	pExtract->SetOffsetStart(pParam->nOffsetStart);
	pExtract->SetOffsetEnd(pParam->nOffsetEnd);
	pExtract->SetSpeed(pParam->nSpeed);
	pExtract->SetSpinUpTime(pParam->nSpinUpTime);
	pExtract->SetJitterCorrection(pParam->bJitterCorrection);
	pExtract->SetSwapLefRightChannel(pParam->bSwapLefRightChannel);
	pExtract->SetNumOverlapSectors(pParam->nNumOverlapSectors);
	pExtract->SetDriveTable(pParam->DriveTable);
	pExtract->SetNumReadSectors(pParam->nNumReadSectors	);
	pExtract->SetNumCompareSectors(pParam->nNumCompareSectors);

	pExtract->SetTargetID(pParam->btTargetID);
	pExtract->SetAdapterID(pParam->btAdapterID);
	pExtract->SetLunID(pParam->btLunID);

	pExtract->SetAspiPosting( pParam->bAspiPosting );

	pExtract->SetAspiRetries(pParam->nAspiRetries);
	pExtract->SetAspiTimeOut(pParam->nAspiTimeOut);

	pExtract->SetMultiReadEnable(pParam->bEnableMultiRead);
	pExtract->SetMultiRead(pParam->nMultiReadCount);
	pExtract->SetMultiReadFirstOnly(pParam->bMultiReadFirstOnly);

	pExtract->SetLockDuringRead( pParam->bLockDuringRead );

	if (pParam->DriveTable.DriveType!=CUSTOMDRIVE)
	{
		nErr=CR_SelectCDROMType( pParam->DriveTable.DriveType );
	}

	pExtract->SetRippingMode( pParam->nRippingMode );

	pExtract->SetParanoiaMode( pParam->nParanoiaMode );

	pExtract->SetUseCDText( pParam->bUseCDText );

	return nErr;
}




DLLEXPORT CDEX_ERR CCONV CR_OpenRipper(	LONG* plBufferSize,
										LONG dwStartSector,
										LONG dwEndSector
										)
{
	DebugPrintf("Entering CR_OpenRipper");
	if (!pExtract)
		return CDEX_ERROR;

	// Set Extract paramters, dwEndSector is inclusive !
	// thus if startsector=0 and endsector 1649, 1650 sectors are extracted
	switch ( pExtract->GetRippingMode() )
	{
		case CR_RIPPING_MODE_NORMAL:
			pExtract->SetupTrackExtract( dwStartSector, dwEndSector + 1 );
		break;
		case CR_RIPPING_MODE_PARANOIA:
			pExtract->SetupTrackExtractParanoia( dwStartSector, dwEndSector + 1 );
		break;
		default:
			ASSERT( FALSE );
			return CDEX_ERROR;
	}


	// Start Thread
	//pExtract->StartThread(pExtract->ThreadFunc,pExtract);
	*plBufferSize= pExtract->GetNumReadSectors() * CB_CDDASECTORSIZE;

	DebugPrintf("Leaving CR_OpenRipper");
	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_CloseRipper()
{
	DebugPrintf("Entering CR_CloseRipper");
	if (pExtract)
	{
		// Set Extract paramters
		pExtract->EndTrackExtract();

	}

	DebugPrintf("Leaving CR_CloseRipper");

	// And return 
	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_RipChunk(BYTE* pbtStream,LONG* pNumBytes, BOOL& bAbort)
{
	CDEX_ERR ret = CDEX_OK;

	DebugPrintf("Entering CR_RipChunk");

	ASSERT( pNumBytes );
	ASSERT( pbtStream );

	*pNumBytes=0;

	if (pExtract)
	{
		switch ( pExtract->GetRippingMode() )
		{
			case CR_RIPPING_MODE_NORMAL:
				ret = pExtract->RipChunk( pbtStream, pNumBytes, bAbort );
			break;
			case CR_RIPPING_MODE_PARANOIA:
				ret = pExtract->RipChunkParanoia( pbtStream, pNumBytes, bAbort );
			break;
			default:
				ASSERT( FALSE );
				ret = CDEX_ERROR;
		}
	}
	else
	{
		ASSERT( FALSE );
		ret = CDEX_ERROR;
	}

	DebugPrintf("Leaving CR_RipChunk with return %d", ret );

	return ret;

}


DLLEXPORT LONG CCONV	CR_GetPeakValue()
{
	if (pExtract)
	{
		return pExtract->GetPeakValue();
	}
	return 0;
}




DLLEXPORT LONG CCONV CR_GetPercentCompleted()
{
	if (pExtract)
	{
		return min(pExtract->GetPercentCompleted(),99);
	}
	return 0;
}


DLLEXPORT LONG CCONV	CR_GetNumberOfJitterErrors()
{
	if (pExtract)
	{
		return pExtract->GetJitterErrors();
	}
	return 0;
}


DLLEXPORT CDEX_ERR CCONV CR_SaveSettings()
{
	DebugPrintf( "CR_SaveSettings" );

	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	pExtract->SaveSettings();

	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_LoadSettings()
{
	DebugPrintf( "CR_LoadSettings" );

	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	pExtract->LoadSettings( TRUE );
	return CDEX_OK;
}


DLLEXPORT CDEX_ERR CCONV CR_ReadToc()
{
	DebugPrintf( "CR_ReadToc" );

	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	return pExtract->ReadToc();
}

DLLEXPORT CDEX_ERR CCONV CR_ReadCDText(BYTE* pbtBuffer,int nBufferSize,LPINT pnCDTextSize)
{
	DebugPrintf( "CR_ReadCDText" );

	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	if ( FALSE == pExtract->GetUseCDText() )
	{
		return CDEX_ERROR;
	}

	return pExtract->ReadCDText(pbtBuffer,nBufferSize,pnCDTextSize);
}


DLLEXPORT LONG CCONV CR_GetNumTocEntries()
{
	DebugPrintf( "CR_GetNumTocEntries" );

	if ( !pExtract )
		return 0;

	return pExtract->GetToc().GetNumTracks();
}

DLLEXPORT TOCENTRY CCONV CR_GetTocEntry(LONG nTocEntry)
{
	TOCENTRY TocEntry;

	DebugPrintf( "CR_GetTocEntry" );

	memset( &TocEntry, 0x00, sizeof( TocEntry ) );

	if ( pExtract )
	{
		TocEntry.dwStartSector=pExtract->GetToc().GetStartSector( nTocEntry );
		TocEntry.btFlag=pExtract->GetToc().GetFlags( nTocEntry );
		TocEntry.btTrackNumber=pExtract->GetToc().GetTrackNumber( nTocEntry );
	}

	return TocEntry;
}

DLLEXPORT void CCONV CR_NormalizeChunk(SHORT* pbsStream,LONG nNumSamples,DOUBLE dScaleFactor)
{
	int i;
	DebugPrintf( "CR_NormalizeChunk" );

	for ( i = 0; i < nNumSamples; i++)
	{
		pbsStream[ i ] = (short)( (double)pbsStream[ i ] * dScaleFactor );
	}
}




DLLEXPORT BOOL CCONV CR_IsUnitReady()
{
	if (!pExtract)
		return CDEX_ERROR;

	return pExtract->IsUnitReady();
}

DLLEXPORT CDEX_ERR CCONV CR_IsMediaLoaded( CDMEDIASTATUS& IsMediaLoaded )
{
	IsMediaLoaded = CDMEDIA_NOT_PRESENT;

	if (!pExtract)
	{
		return CDEX_ERROR;
	}

	IsMediaLoaded = pExtract->IsMediaLoaded();

	return CDEX_OK;
}

DLLEXPORT BOOL CCONV CR_EjectCD(BOOL bEject)
{
	DebugPrintf("CR_EjectCD");
	if (!pExtract)
		return CDEX_ERROR;

	pExtract->PreventMediaRemoval( FALSE );


	return pExtract->EjectCD(bEject);
}

DLLEXPORT void CCONV CR_LockCD( BOOL bLock )
{
	DebugPrintf("CR_LockCD");
	if (!pExtract)
		return ;

	pExtract->PreventMediaRemoval( bLock );
}


DLLEXPORT BOOL CCONV CR_IsAudioPlaying()
{
	if (!pExtract)
		return CDEX_ERROR;
	return pExtract->IsAudioPlaying();
}

DLLEXPORT CDEX_ERR CCONV CR_PlayTrack( int nTrack )
{
	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	int nNumTocEntries=CR_GetNumTocEntries();
	
	for ( int i = 0; i < nNumTocEntries; i++ )
	{
		TOCENTRY myTocEntry = CR_GetTocEntry( i );
		TOCENTRY myTocEntry1 = CR_GetTocEntry( i + 1 );

		if ( pExtract && myTocEntry.btTrackNumber == nTrack )
		{
			pExtract->PlayTrack(	myTocEntry.dwStartSector, 
									myTocEntry1.dwStartSector - 1 );
			return CDEX_OK;
		}
	}
	return CDEX_ERROR;
}


DLLEXPORT CDEX_ERR CCONV CR_PlaySection(LONG lStartSector,LONG lEndSector)
{
	DebugPrintf( "CR_PlaySection" );
	if ( !pExtract )
		return CDEX_ERROR;

	pExtract->PlayTrack(lStartSector,lEndSector);
	return CDEX_OK;
}



DLLEXPORT CDEX_ERR CCONV CR_StopPlayTrack()
{
	DebugPrintf( "CR_StopPlayTrack" );
	if ( !pExtract )
	{
		return CDEX_ERROR;
	}

	pExtract->StopPlayTrack();

	return CDEX_OK;
}

DLLEXPORT CDEX_ERR CCONV CR_PauseCD(BOOL bPause)
{
	DebugPrintf("CR_Pause");
	if (!pExtract)
		return CDEX_ERROR;

	pExtract->PauseCD(bPause);
	return CDEX_OK;
}


DLLEXPORT CDSTATUSINFO CCONV CR_GetCDStatusInfo()
{
	return g_CDStatusInfo;
}


DLLEXPORT CDEX_ERR CCONV CR_GetPlayPosition(DWORD& dwRelPos,DWORD& dwAbsPos)
{
	if (!pExtract)
		return CDEX_ERROR;

	pExtract->CurrentPosition(dwRelPos,dwAbsPos);
	return CDEX_OK;
}

DLLEXPORT CDEX_ERR CCONV CR_SetPlayPosition(DWORD dwAbsPos)
{
	if (!pExtract)
		return CDEX_ERROR;

	pExtract->Seek(dwAbsPos);

	return CDEX_OK;
}


DLLEXPORT DRIVETYPE CCONV CR_GetCDROMType()
{
	if (!pExtract)
		return GENERIC;
	return pExtract->GetDriveType();
}


DLLEXPORT LONG CCONV	CR_GetJitterPosition()
{
	if (pExtract)
		return pExtract->GetJitterPosition();
	return 50;
}


DLLEXPORT void CCONV CR_GetLastJitterErrorPosition(DWORD& dwStartSector,DWORD& dwEndSector)
{
	dwStartSector=0;
	dwEndSector=0;
	if (pExtract)
		pExtract->GetLastJitterErrorPosition(dwStartSector,dwEndSector);
}



DLLEXPORT void CCONV  CR_GetSubChannelTrackInfo(	
										int&	nReadIndex,
										int&	nReadTrack,
										DWORD&	dwReadPos )
{
	if (pExtract)
		pExtract->GetSubChannelTrackInfo( nReadIndex, nReadTrack, dwReadPos );
}



BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			TRACE0("CDRIP.DLL Initializing!\n");
		break;
		case DLL_THREAD_ATTACH:
		break;
		case DLL_THREAD_DETACH:
		break;
		case DLL_PROCESS_DETACH:
			TRACE0("CDRIP.DLL Terminating!\n");
			DebugPrintf("DllMain DLL_PROCESS_DETACH");
			CR_DeInit();
		break;
    }
    return TRUE;
}



DLLEXPORT VOID CCONV CR_SetTransportLayer( int nTransportLayer )
{
	CDRomSettings::SetTransportLayer( nTransportLayer );
}

DLLEXPORT INT CCONV CR_GetTransportLayer(  )
{
	return CDRomSettings::GetTransportLayer();
}

DLLEXPORT CDEX_ERR CCONV CR_ScanForC2Errors(	
	DWORD	dwStartSector,
	DWORD	dwNumSectors,
	DWORD&	dwErrors,
	DWORD*	pdwErrorSectors )
{
	CDEX_ERR bReturn = CDEX_ERROR;

	if ( pExtract )
	{
		bReturn = pExtract->ScanForC2Errors(	dwStartSector, 
												dwNumSectors,
												dwErrors,
												pdwErrorSectors );
	}

	return bReturn;
}

DLLEXPORT DWORD CCONV CR_GetCurrentRipSector()
{
	return 0;
}

DLLEXPORT CDEX_ERR CCONV CR_GetDetailedDriveInfo( 
	LPSTR lpszInfo,
	DWORD dwInfoSize )
{
	CDEX_ERR bReturn = CDEX_ERROR;

	if ( pExtract )
	{
		bReturn = pExtract->GetDetailedDriveInfo(	lpszInfo, 
													dwInfoSize );
	}
	return bReturn;
}
/*
** Copyright (C) 1999 Albert L. Faber
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
#include "CDRomSettings.h"
#include "Aspi.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CToc::FillEntry(BYTE* btStream)
{

//	m_reserved1=btStream[0];
	m_btaFlags[m_nNumTracks]=btStream[1];
	m_btaTrackNumber[m_nNumTracks]=btStream[2];
//	m_reserved2=btStream[3];
	memcpy(&m_dwaStartSector[m_nNumTracks],&btStream[4],sizeof(DWORD));

	// Swap DWORD order
    Swap (&m_dwaStartSector[m_nNumTracks], 4);

	// Increase the number of total tracks
	m_nNumTracks++;
}

// CONSTRUCTOR
CToc::CToc()
{
	// Initialize variables
	Clear();
}


void CToc::Clear()
{
	m_nNumTracks=0;
	memset(m_dwaStartSector,0x00,sizeof(m_dwaStartSector));
	memset(m_btaFlags,0x00,sizeof(m_btaFlags));
	memset(m_btaTrackNumber,0x00,sizeof(m_btaTrackNumber));
}


// DESTRUCTOR
CToc::~CToc()
{
}



void CToc::DumpToc(LPBYTE pbtBuffer,int nSize)
{
/*
	int			i;
	DWORD		dw;
	unsigned	mins;
	double		secnds;
	extern double fmod(double,double);


	// Open Dump File
	CADebug myLogFile(CString("C:\\CDEX_TOC.txt"));

	// A header would not hurt
	myLogFile.printf("Start of TOC dump\n\n");

	// Dump binary data
	myLogFile.printf("Dumping binary data first, buffer size=%d =>%d toc entries\n\n",nSize,nSize/8);
	
	myLogFile.DumpBinaray(pbtBuffer,nSize);

	dw = GetStartSector(m_wNumTracks-1);

	// Calculate number of minutes
	mins= dw / (TRACKSPERSEC*60L);

	secnds= (fmod(dw , 60*TRACKSPERSEC) / (DOUBLE)TRACKSPERSEC);


	myLogFile.printf("\n\nDumping Toc Entries\n");
	for (i=0; i<m_wNumTracks; i++) 
	{
		myLogFile.printf("Toc Entry=%2d  StartSector=%8d Flags=%2d",i,m_TrackInfo[i].GetStartSector(),m_TrackInfo[i].GetFlags());
		// Skip nonaudio tracks 
//		if (m_TrackInfo[i].GetFlags() != AUDIOTRKFLAG) 
//			continue;					


		if (m_TrackInfo[i].GetTrack() <= MAXTRK) 
		{
			DWORD dw2;

			dw2= m_TrackInfo[i].GetStartSector();

			dw = m_TrackInfo[i+1].GetStartSector();

			mins= (dw - dw2) / (TRACKSPERSEC*60L);
			secnds= fmod(dw-dw2 , 60*TRACKSPERSEC) / TRACKSPERSEC;


			CString strDuration=GetTrackDuration(i+1);
			CString strStart=GetStartTime(i+1);

			CString strNew;
			myLogFile.printf(" => Start time=%s Duration=%s\n",strStart,strDuration);
		}

	}
	myLogFile.printf("\n\nEnd of TOC dump\n");
*/
}

BOOL CToc::IsAudioTrack(int p_track)
{
	if ( !( m_btaFlags[p_track] & CDROMDATAFLAG) )
		return TRUE;
	return FALSE;
}

DWORD CToc::GetSize(int nTrack)
{
	DWORD dwSectors = GetStartSector(nTrack+1)-GetStartSector(nTrack);
	return dwSectors*CB_CDDASECTOR;
}




CDSETTINGSPARAMS::CDSETTINGSPARAMS()
{
	strcpy(lpszCDROMID,"DONTKNOW");
	nOffsetStart=0;
	nOffsetEnd=0;	
	nSpeed=32;
	nSpinUpTime=0;
	bJitterCorrection=1;
	bSwapLefRightChannel=0;
	nNumOverlapSectors=7;
	nNumReadSectors=26;	
	nNumCompareSectors=1;	
	nMultiReadCount=0;
	bMultiReadFirstOnly=FALSE;
	bLockDuringRead= TRUE;

	btTargetID=0;
	btAdapterID=0;
	btLunID=0;
	nAspiRetries=0;
	nAspiTimeOut=3000;


	DriveTable.DriveType=GENERIC;
	DriveTable.ReadMethod=READMMC;
	DriveTable.SetSpeed=SPEEDMMC;
	DriveTable.Endian=LITTLEENDIAN;
	DriveTable.EnableMode=ENABLENONE;
	DriveTable.nDensity=0;
	DriveTable.bAtapi=TRUE;

	bAspiPosting=FALSE;
	nRippingMode = 0;
	nParanoiaMode = 0;

}

CDSETTINGSPARAMS::~CDSETTINGSPARAMS()
{

}


char	CDRomSettings::m_lpszIniFname[255] = {'\0',};
BOOL	CDRomSettings::m_nTransportLayer = TRANSPLAYER_ASPI;

CDRomSettings::CDRomSettings()
{
	m_nActive = 0;
}

CDRomSettings::~CDRomSettings()
{
}


void CDRomSettings::AddCDRom(LPSTR lpszName,BYTE btAdapterID,BYTE btTargetID,BYTE btLunID)
{
	int nDeviceOffset=0;

	// Loop through Array and see if there are any cd rom with the same name
	for (int i=0;i<m_CDParams.size();i++)
	{
		// Check if name to add is equal to current indexed CDROM setting
		if (strstr(m_CDParams[i].lpszCDROMID,lpszName) !=NULL)
		{
			// Increase number
			nDeviceOffset++;
		}
	}

	// There appears to be CDROMs with the same name
	if (nDeviceOffset>0)
	{
		// Add (nDeviceOffset) to strName
		sprintf(lpszName,"(%d)",nDeviceOffset);
	}
	
	// Create new CDROM setting
	CDSETTINGSPARAMS newSettings;

	// Set device name
	strcpy(newSettings.lpszCDROMID,lpszName);
	newSettings.btTargetID=btTargetID;
	newSettings.btAdapterID=btAdapterID;
	newSettings.btLunID=btLunID;

	// Add to array of CDROM settings
	m_CDParams.push_back(newSettings);

	// Also add a new Table of contents
	m_Toc.push_back(CToc());

}

void CDRomSettings::SaveSettings()
{
	CIni	myIni;
	CIni*	pIni = &myIni;

	pIni->SetIniFileName( m_lpszIniFname );

	// Loop through Array of CD-ROM settings
	for ( int i=0; i < m_CDParams.size(); i++ )
	{
		char lpszKey[255];

		strcpy(lpszKey,m_CDParams[i].lpszCDROMID);

		// Replace spaces with underscores
		for (int j=0;j<strlen(lpszKey);j++)
			if (lpszKey[j]==' ')
				lpszKey[j]='_';

		pIni->SetValue(lpszKey, "nOffsetStart"			,m_CDParams[i].nOffsetStart );
		pIni->SetValue(lpszKey, "nOffsetEnd"			,m_CDParams[i].nOffsetEnd );
		pIni->SetValue(lpszKey, "nSpeed"				,m_CDParams[i].nSpeed );
		pIni->SetValue(lpszKey, "nSpinUpTime"			,m_CDParams[i].nSpinUpTime );
		pIni->SetValue(lpszKey, "bJitterCorrection"		,m_CDParams[i].bJitterCorrection );
		pIni->SetValue(lpszKey, "bSwapLefRightChannel"	,m_CDParams[i].bSwapLefRightChannel );
		pIni->SetValue(lpszKey, "nNumOverlapSectors"	,m_CDParams[i].nNumOverlapSectors );
		pIni->SetValue(lpszKey, "nNumReadSectors"		,m_CDParams[i].nNumReadSectors );
		pIni->SetValue(lpszKey, "nNumCompareSectors"	,m_CDParams[i].nNumCompareSectors );
		pIni->SetValue(lpszKey, "nAspiTimeOut"			,m_CDParams[i].nAspiTimeOut );
		pIni->SetValue(lpszKey, "nAspiRetries"			,m_CDParams[i].nAspiRetries );
		pIni->SetValue(lpszKey, "bEnableMultiRead"		,m_CDParams[i].bEnableMultiRead );
		pIni->SetValue(lpszKey, "bMultiReadFirstOnly"	,m_CDParams[i].bMultiReadFirstOnly );
		pIni->SetValue(lpszKey, "nMultiReadCount"		,m_CDParams[i].nMultiReadCount );
		pIni->SetValue(lpszKey, "bLockDuringRead"		,m_CDParams[i].bLockDuringRead );

		// Write Drive Settings
		pIni->SetValue(lpszKey, "DriveType"				,m_CDParams[i].DriveTable.DriveType );
		pIni->SetValue(lpszKey, "ReadMethod"			,m_CDParams[i].DriveTable.ReadMethod );
		pIni->SetValue(lpszKey, "SetSpeed"				,m_CDParams[i].DriveTable.SetSpeed );
		pIni->SetValue(lpszKey, "EnableMode"			,m_CDParams[i].DriveTable.EnableMode );
		pIni->SetValue(lpszKey, "nDensity"				,m_CDParams[i].DriveTable.nDensity );
		pIni->SetValue(lpszKey, "Endian"				,m_CDParams[i].DriveTable.Endian );
		pIni->SetValue(lpszKey, "bAtapi"				,m_CDParams[i].DriveTable.bAtapi );
		pIni->SetValue(lpszKey, "bAspiPosting"			,m_CDParams[i].bAspiPosting );
		pIni->SetValue(lpszKey, "nParanoiaMode"			,m_CDParams[i].nParanoiaMode );
		pIni->SetValue(lpszKey, "nRippingMode"			,m_CDParams[i].nRippingMode );

	}

	// Save active CD-ROM setting
	pIni->SetValue( "CD-ROM","nActive"	,m_nActive );

	pIni->SetValue( "CD-ROM","nTransportLayer",(int)m_nTransportLayer );

}


void CDRomSettings::LoadSettings()
{
	CIni	myIni;
	CIni*	pIni = NULL;

	pIni = &myIni;

	pIni->SetIniFileName( m_lpszIniFname );

	// Loop through Array of CD-ROM settings
	for (int i=0;i<m_CDParams.size();i++)
	{
		char lpszKey[255];
		
		strcpy(lpszKey,m_CDParams[i].lpszCDROMID);

		// Replace spaces with underscores
		for (int j=0;j<strlen(lpszKey);j++)
			if (lpszKey[j]==' ')
				lpszKey[j]='_';

		m_CDParams[i].nOffsetStart			= pIni->GetValue(lpszKey,"nOffsetStart"			,m_CDParams[i].nOffsetStart );
		m_CDParams[i].nOffsetEnd			= pIni->GetValue(lpszKey,"nOffsetEnd"			,m_CDParams[i].nOffsetEnd );
		m_CDParams[i].nSpeed				= pIni->GetValue(lpszKey,"nSpeed"				,m_CDParams[i].nSpeed );
		m_CDParams[i].nSpinUpTime			= pIni->GetValue(lpszKey,"nSpinUpTime"			,m_CDParams[i].nSpinUpTime );
		m_CDParams[i].bJitterCorrection		= pIni->GetValue(lpszKey,"bJitterCorrection"	,m_CDParams[i].bJitterCorrection );
		m_CDParams[i].bSwapLefRightChannel	= pIni->GetValue(lpszKey,"bSwapLefRightChannel" ,m_CDParams[i].bSwapLefRightChannel );
		m_CDParams[i].nNumOverlapSectors	= pIni->GetValue(lpszKey,"nNumOverlapSectors"	,m_CDParams[i].nNumOverlapSectors );
		m_CDParams[i].nNumReadSectors		= pIni->GetValue(lpszKey,"nNumReadSectors"		,m_CDParams[i].nNumReadSectors );
		m_CDParams[i].nNumCompareSectors	= pIni->GetValue(lpszKey,"nNumCompareSectors"	,m_CDParams[i].nNumCompareSectors );
		m_CDParams[i].nAspiTimeOut			= pIni->GetValue(lpszKey,"nAspiTimeOut"			,m_CDParams[i].nAspiTimeOut );
		m_CDParams[i].nAspiRetries			= pIni->GetValue(lpszKey,"nAspiRetries"			,m_CDParams[i].nAspiRetries );
		m_CDParams[i].bEnableMultiRead		= pIni->GetValue(lpszKey,"bEnableMultiRead"		,m_CDParams[i].bEnableMultiRead );
		m_CDParams[i].bMultiReadFirstOnly	= pIni->GetValue(lpszKey,"bMultiReadFirstOnly"	,m_CDParams[i].bMultiReadFirstOnly );
		m_CDParams[i].nMultiReadCount		= pIni->GetValue(lpszKey,"nMultiReadCount"		,m_CDParams[i].nMultiReadCount );
		m_CDParams[i].bLockDuringRead		= pIni->GetValue(lpszKey,"bLockDuringRead"		,m_CDParams[i].bLockDuringRead );

		// Custom Drive Settings
		m_CDParams[i].DriveTable.DriveType	=DRIVETYPE(pIni->GetValue(lpszKey,"DriveType",m_CDParams[i].DriveTable.DriveType ));
		m_CDParams[i].DriveTable.ReadMethod	=READMETHOD(pIni->GetValue(lpszKey,"ReadMethod",m_CDParams[i].DriveTable.ReadMethod ));
		m_CDParams[i].DriveTable.SetSpeed	=SETSPEED(pIni->GetValue(lpszKey,"SetSpeed",m_CDParams[i].DriveTable.SetSpeed ));
		m_CDParams[i].DriveTable.EnableMode	=ENABLEMODE(pIni->GetValue(lpszKey,"EnableMode",m_CDParams[i].DriveTable.EnableMode ));
		m_CDParams[i].DriveTable.nDensity	=pIni->GetValue(lpszKey,"nDensity",m_CDParams[i].DriveTable.nDensity );
		m_CDParams[i].DriveTable.Endian		=ENDIAN(pIni->GetValue(lpszKey,"Endian",m_CDParams[i].DriveTable.Endian ));
		m_CDParams[i].DriveTable.bAtapi		=pIni->GetValue(lpszKey,"bAtapi",m_CDParams[i].DriveTable.bAtapi );
		m_CDParams[i].bAspiPosting			=pIni->GetValue(lpszKey,"bAspiPosting"		,m_CDParams[i].bAspiPosting );
		m_CDParams[i].nRippingMode			=pIni->GetValue(lpszKey,"nRippingMode"		,m_CDParams[i].nRippingMode );
		m_CDParams[i].nParanoiaMode			=pIni->GetValue(lpszKey,"nParanoiaMode"		,m_CDParams[i].nParanoiaMode );
	}
	// Load active CD-ROM setting
	m_nActive=pIni->GetValue("CD-ROM","nActive",m_nActive );

	// Make sure selection if valid
	m_nActive=min(max(0,m_nActive),m_CDParams.size()-1);
	m_nTransportLayer = pIni->GetValue( "CD-ROM", "nTransportLayer", 0 );


	// Get default values when not a CUSTOM drive type
	UpdateDriveSettings();
}


void CDRomSettings::UpdateDriveSettings()
{
	if (m_CDParams[m_nActive].DriveTable.DriveType!=CUSTOMDRIVE)
	{
		// Set defaults	(SONY)
		m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
		m_CDParams[m_nActive].DriveTable.Endian		=LITTLEENDIAN;
		m_CDParams[m_nActive].DriveTable.nDensity	=0x00;
		m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLENONE;
		m_CDParams[m_nActive].DriveTable.bAtapi		=FALSE;
	}

	// Setup the specific drive properties
	switch (m_CDParams[m_nActive].DriveTable.DriveType)
	{
		case GENERIC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READMMC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDMMC;
			m_CDParams[m_nActive].DriveTable.bAtapi		=TRUE;
		break;

		case TOSHIBANEW:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDTOSHIBA;
			m_CDParams[m_nActive].DriveTable.nDensity	=0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
		break;
		case TOSHIBA:	
		case IBM:	
		case DEC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
			m_CDParams[m_nActive].DriveTable.nDensity	=0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
		break;
		case IMSCDD5:	//ReadTocCdrSCSI : ReadTocSCSI; not yet implemented yet
		case IMS:	
		case KODAK:	
		case RICOH:	
		case HP:	
		case PHILIPS:	
		case PLASMON:	
		case GRUNDIGCDR100IPW:
		case MITSUMICDR:
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDPHILIPS;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
			m_CDParams[m_nActive].DriveTable.Endian		=BIGENDIAN;
		break;
		case NRC:
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
		break;
		case YAMAHA:
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDYAMAHA;
		break;
		case SONY:
		case PLEXTOR:			// beware of type CD-ROM CDU55E can use MMC reading
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READSONY;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDSONY;
		break;
		case NEC:
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READNEC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNEC;
		break;
		case CUSTOMDRIVE:
		break;
		default:
			MessageBox(NULL,"Internal lameFE error, Device Type Not Supported","CDRip.DLL Error",MB_OK);
	}
}





int CDRomSettings::GetTransportLayer()
{
	return m_nTransportLayer;
}

void CDRomSettings::SetTransportLayer( int nValue )
{
	m_nTransportLayer = nValue;
}

void CDRomSettings::SetIniFileName( LPCSTR lpszIniFname)
{
	strcpy( m_lpszIniFname, lpszIniFname );
}



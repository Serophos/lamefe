/*
 *	Vorbis DLL Sample Code.
 *
 *	Copyright (c) 2000 A.L. Faber
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */


#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "VorbisDLL.h"



static int VorbisEncode(LPCSTR strFileIn)
{
	CHAR		strFileOut[255]	={'0',};
	HINSTANCE	hDLL			=NULL;
	FILE*		pFileIn			=NULL;
	FILE*		pFileOut		=NULL;
	VB_VERSION	Version			={0,};
	VB_CONFIG	vbConfig		={0,};


	DWORD		dwSamples		=0;
	DWORD		dwOggBuffer		=0;
	HVB_STREAM	hvbStream		=0;
	VB_ERR		err				=0;

	PBYTE		pOggBuffer		=NULL;
	PSHORT		pWAVBuffer		=NULL;


	VBENCOPEN	vbEncOpen		=NULL;
	VBENCWRITE	vbEncWrite		=NULL;
	VBENCCLOSE	vbEncClose		=NULL;
	VBVERSION	vbVersion		=NULL;

	VB_ALBUM_INFO vbAlbumInfo	={0,};

	CHAR		strTitle[]		={"Title Unknown"};
	CHAR		strArtist[]		={"Artist Unknown"};
	CHAR		strAlbum[]		={"Album Unknown"};
	CHAR		strComment[]	={"Encoded with Vorbis DLL example"};


	// Setup the file names
	strcpy(strFileOut,strFileIn);

	// Change the file name extention for the output file name
	strFileOut[strlen(strFileOut)-3]='o';
	strFileOut[strlen(strFileOut)-2]='g';
	strFileOut[strlen(strFileOut)-1]='g';

	// Try to load the DLL
	hDLL=LoadLibrary("Vorbis.dll");

	if (hDLL==NULL)
		hDLL=LoadLibrary(".\\Debug\\Vorbis.dll");
	if (hDLL==NULL)
		hDLL=LoadLibrary(".\\Release\\Vorbis.dll");

	// Check DLL loading result
	if(hDLL==NULL)
	{
		fprintf(stderr,"Error loading Vrobis.DLL");
		return -1;
	}

	// Get Interface functions
	vbEncOpen	= (VBENCOPEN)	GetProcAddress(hDLL, TEXT_VBENCOPEN);
	vbEncWrite	= (VBENCWRITE)	GetProcAddress(hDLL, TEXT_VBENCWRITE);
	vbEncClose	= (VBENCCLOSE)	GetProcAddress(hDLL, TEXT_VBENCCLOSE);
	vbVersion	= (VBVERSION)	GetProcAddress(hDLL, TEXT_VBVERSION);

	// Check if all interfaces are present
	if(	!	vbEncOpen || !vbEncWrite || !vbEncClose || !vbVersion)
	{
		printf("Unable to get Vorbis interfaces");
		return -1;
	}

	// Get the version number
	vbVersion(&Version);

	printf(
			"Vorbis.dll version %u.%02u (%u/%u/%u)\n"
			"Vorbis Engine %u.%02u\n"
			"Vorbis homepage at %s\n\n",	
			Version.byDLLMajorVersion, Version.byDLLMinorVersion,
			Version.byDay, Version.byMonth, Version.wYear,
			Version.byMajorVersion, Version.byMinorVersion,
			Version.zHomepage);

	// Try to open the WAV file, be sure to open it as a binary file!	
	pFileIn = fopen(strFileIn,"rb");

	// Check file open result
	if(pFileIn == NULL)
	{
		fprintf(stderr,"Error opening %s", strFileIn);
		return -1;
	}

	// Open Ogg file
	pFileOut= fopen(strFileOut,"wb+");

	// Check file open result
	if(pFileOut == NULL)
	{
		fprintf(stderr,"Error creating file %s", strFileOut);
		return -1;
	}

	// use the Vorbis config structure
	vbConfig.dwConfig						= VB_CONFIG_VORBIS;
	vbConfig.format.VBV1.dwStructVersion	= VB_ENC_STRUCT_VERSION;
	vbConfig.format.VBV1.dwStructSize		= sizeof(vbConfig);		
	vbConfig.format.VBV1.dwSampleRate		= 44100;
	vbConfig.format.VBV1.dwMinBitRate		= 0;
	vbConfig.format.VBV1.dwNomBitRate		= 0;
	vbConfig.format.VBV1.dwMaxBitRate		= 0;
	vbConfig.format.VBV1.dwInfoMode			= VB_MODE_C;
	vbConfig.format.VBV1.dwChannels			= 2;


	// setup album info

	vbAlbumInfo.dwSize=sizeof(vbAlbumInfo);
	vbAlbumInfo.dwVersion=VB_ALB_STRUCT_VERSION;
	vbAlbumInfo.dwRecords=1;

	vbAlbumInfo.infoRecord.dwSize=sizeof(vbAlbumInfo.infoRecord);
	vbAlbumInfo.infoRecord.dwVersion=VB_ALB_STRUCT_VERSION;

	vbAlbumInfo.infoRecord.strArtist=strArtist;
	vbAlbumInfo.infoRecord.strAlbum=strAlbum;
	vbAlbumInfo.infoRecord.strTitle=strTitle;
	vbAlbumInfo.infoRecord.strComment=strComment;
	vbAlbumInfo.infoRecord.dwTrackNumber=1;
	vbAlbumInfo.infoRecord.dwTotalTracks=1;


//#define NO_ALBUM_TAG
#ifdef NO_ALBUM_TAG
	if (vbEncOpen(&vbConfig,NULL, &dwSamples,&dwOggBuffer,&hvbStream)!=VB_ERR_OK)
#else
	if (vbEncOpen(&vbConfig,&vbAlbumInfo, &dwSamples,&dwOggBuffer,&hvbStream)!=VB_ERR_OK)
#endif
	{
		fprintf(stderr,"Failed to open the encoding stream\n");
		return -1;
	}

	// Allocate Ogg buffer
	pOggBuffer = new BYTE[dwOggBuffer];

	// Allocate WAV buffer
	pWAVBuffer = new SHORT[dwSamples];

	// Check if Buffer are allocated properly
	if(!pOggBuffer || !pWAVBuffer)
	{
		fprintf(stderr,"Out of memory");
		return -1;
	}

	DWORD dwRead=0;
	DWORD dwWrite=0;
	DWORD dwDone=0;
	DWORD dwFileSize=0;

	// Seek to end of file
	fseek(pFileIn,0,SEEK_END);

	// Get the file size
	dwFileSize=ftell(pFileIn);

	// Seek back to start of WAV file,
	// but skip the first 44 bytes, since that's the WAV header
	fseek(pFileIn,44,SEEK_SET);


	// Convert All PCM samples
	while ( (dwRead=fread(pWAVBuffer,sizeof(SHORT),dwSamples,pFileIn)) >0 )
	{
		// Encode samples
		err = vbEncWrite(hvbStream, dwRead, pWAVBuffer, pOggBuffer, &dwWrite);

		// Check result
		if(err != VB_ERR_OK)
		{
			fprintf(stderr,"vbEncWrite() failed (%lu)", err);
			return -1;
		}
		
		// write dwWrite bytes that are returned in tehe pOggBuffer to disk
		if(fwrite(pOggBuffer,1,dwWrite,pFileOut) != dwWrite)
		{
			fprintf(stderr,"Output file write error");
			return -1;
		}

		dwDone += dwRead*sizeof(SHORT);

		printf("Encoding: %0.2f%%     \r", 100 * (float)dwDone/(float)(dwFileSize));
	}


	// Close encoder stream an check result
	if( vbEncClose(hvbStream,pOggBuffer,&dwWrite) == VB_ERR_OK)
	{
		// write dwWrite bytes that are returned in tehe pOggBuffer to disk
		if(fwrite(pOggBuffer,1,dwWrite,pFileOut) != dwWrite)
		{
			fprintf(stderr,"Output file write error");
			return -1;
		}
		return -1;
	}
	else
	{
		fprintf(stderr,"Failed to close the encoder stream!\n");
		return -1;
	}

	// Delete WAV buffer
	delete [] pWAVBuffer;

	// Delete Ogg Buffer
	delete [] pOggBuffer;

	// Close input file
	fclose(pFileIn);

	// Close output file
	fclose(pFileOut);
}


static int VorbisDecode(LPCSTR strFileIn)
{
	CHAR		strFileOut[255]	={'0',};
	HINSTANCE	hDLL			=NULL;
	FILE*		pFileOut		=NULL;
	VB_VERSION	Version			={0,};
	VB_DEC_INFO	vbDecInfo		={0,};
	DWORD		dwSamples		=0;
	VB_ERR		err				=0;
	PBYTE		pWAVBuffer		=NULL;
	VBDECOPEN	vbDecOpen		=NULL;
	VBDECREAD	vbDecRead		=NULL;
	VBDECCLOSE	vbDecClose		=NULL;
	VBVERSION	vbVersion		=NULL;
	DWORD		dwWrite			=0;
	PVB_ALBUM_INFO pvbAlbumInfo	=NULL;

	// Setup the file names
	strcpy(strFileOut,strFileIn);

	// Change the file name extention for the output file name
	strFileOut[strlen(strFileOut)-3]='r';
	strFileOut[strlen(strFileOut)-2]='a';
	strFileOut[strlen(strFileOut)-1]='w';

	// Try to load the DLL
	hDLL=LoadLibrary("Vorbis.dll");

	if (hDLL==NULL)
		hDLL=LoadLibrary(".\\Debug\\Vorbis.dll");
	if (hDLL==NULL)
		hDLL=LoadLibrary(".\\Release\\Vorbis.dll");

	// Check DLL loading result
	if(hDLL==NULL)
	{
		fprintf(stderr,"Error loading Vrobis.DLL");
		return -1;
	}

	// Get Interface functions
	vbDecOpen	= (VBDECOPEN)	GetProcAddress(hDLL, TEXT_VBDECOPEN);
	vbDecRead	= (VBDECREAD)	GetProcAddress(hDLL, TEXT_VBDECREAD);
	vbDecClose	= (VBDECCLOSE)	GetProcAddress(hDLL, TEXT_VBDECCLOSE);
	vbVersion	= (VBVERSION)	GetProcAddress(hDLL, TEXT_VBVERSION);

	// Check if all interfaces are present
	if(	!	vbDecOpen || !vbDecRead || !vbDecClose || !vbVersion)
	{
		printf("Unable to get Vorbis interfaces");
		return -1;
	}

	// Get the version number
	vbVersion(&Version);

	printf(
			"Vorbis.dll version %u.%02u (%u/%u/%u)\n"
			"Vorbis Engine %u.%02u\n"
			"Vorbis homepage at %s\n\n",	
			Version.byDLLMajorVersion, Version.byDLLMinorVersion,
			Version.byDay, Version.byMonth, Version.wYear,
			Version.byMajorVersion, Version.byMinorVersion,
			Version.zHomepage);

	// Open Raw output file file
	pFileOut= fopen(strFileOut,"wb+");

	// Check file open result
	if(pFileOut == NULL)
	{
		fprintf(stderr,"Error creating file %s", strFileOut);
		return -1;
	}

	// setup the Vorbis decoder structure
	vbDecInfo.dwStructVersion	= VB_DEC_STRUCT_VERSION;
	vbDecInfo.dwStructSize		= sizeof(vbDecInfo);		

	if (vbDecOpen(&vbDecInfo,&pvbAlbumInfo,strFileIn)!=VB_ERR_OK)
	{
		fprintf(stderr,"Failed to open the decoding stream\n");
		return -1;
	}

	if (pvbAlbumInfo && pvbAlbumInfo->dwRecords>0 )
	{
		DWORD i=0;

		printf("\n");
		printf("********************** Ogg file info *********************\n");
		for (i=0;i<pvbAlbumInfo->dwRecords;i++)
		{
			PVB_ALBUM_INFO_RECORD pRecord=(PVB_ALBUM_INFO_RECORD)
				((PBYTE)(&pvbAlbumInfo->infoRecord)+pvbAlbumInfo->infoRecord.dwSize*i);
			printf("\n");
			printf("Record   :%d\n",i);
			printf("Title    :%s\n",pRecord->strTitle);
			printf("Artist   :%s\n",pRecord->strArtist);
			printf("Album    :%s\n",pRecord->strAlbum);
			printf("Comment  :%s\n",pRecord->strComment);
		}
		printf("*************************** ==#== ************************\n");
	}

	// Allocate WAV buffer
	pWAVBuffer = new BYTE[vbDecInfo.dwBufferSize];

	// Check if Buffer are allocated properly
	if(!pWAVBuffer)
	{
		fprintf(stderr,"Out of memory");
		return -1;
	}

	// Convert Ogg stream to PCM samples till end of stream
	while ( vbDecRead(&vbDecInfo, pWAVBuffer, &dwWrite )!=VB_ERR_EOS)
	{	
		// write dwWrite bytes that are returned in tehe pOggBuffer to disk
		if(fwrite(pWAVBuffer,1,dwWrite,pFileOut) != dwWrite)
		{
			fprintf(stderr,"Output file write error");
			return -1;
		}

		if (vbDecInfo.dwTotTime_in_ms)
		{
			printf("Decoding: %0.2f%%     \r", 100 * (float)vbDecInfo.dwCurTime_in_ms / (float)vbDecInfo.dwTotTime_in_ms);
		}
	}


	// Close decoder stream an check result
	if( vbDecClose(&vbDecInfo) != VB_ERR_OK)
	{
		fprintf(stderr,"Failed to close the encoder stream!\n");
		return -1;
	}

	// Delete WAV buffer
	delete [] pWAVBuffer;

	// Close output file
	fclose(pFileOut);

	return 0;
}


// Main program
int main(int argc, char *argv[])
{	
	// check number of arguments
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <filename.wav>\n", argv[0]);
		fprintf(stderr,"Descr: Short demo to show how to use the Vorbis.dll library file\n");
		fprintf(stderr,"Note : WAV file is assumed to to have the following parameters\n");
		fprintf(stderr,"     : 44100 Hz, stereo, 16 Bits per sample\n");
		return -1;
	}

	// make lower case
	strlwr(argv[1]);

	if ( strstr(argv[1],".ogg")!=NULL)
	{
		// Decoder example
		VorbisDecode(argv[1]);
	}
	else
	{
		// Encoder example
		VorbisEncode(argv[1]);
	}

	// Were done, return OK result
	return 0;
}


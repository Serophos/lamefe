/*
 * Vorbis DLL Interface for CDex.
 *
 * Copyright (c) 2000 A.L. Faber
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

#ifndef ___VORBIS_H_INCLUDED___
#define ___VORBIS_H_INCLUDED___

#pragma pack(push)
#pragma pack(1)

/* encoding formats */

#define		VB_CONFIG_MP3			0										
#define		VB_CONFIG_LAME			256		
#define		VB_CONFIG_VORBIS		1024		

#define		VB_DEC_STRUCT_VERSION 0
#define		VB_ENC_STRUCT_VERSION 0
#define		VB_ALB_STRUCT_VERSION 1

/* type definitions */

typedef		long					HVB_STREAM;
typedef		HVB_STREAM				*PHVB_STREAM;
typedef		unsigned long			VB_ERR;


/* error codes */

#define		VB_ERROR							-1
#define		VB_ERR_OK							0x00000000

// ENCODER ERROR NUMBERS
#define		VB_ERR_ENCODE						0x00001000
#define		VB_ERR_INVALID_FORMAT				0x00001001
#define		VB_ERR_INVALID_FORMAT_PARAMETERS	0x00001002
#define		VB_ERR_NO_MORE_HANDLES				0x00001003
#define		VB_ERR_INVALID_HANDLE				0x00001004
#define		VB_ERR_BUFFER_TOO_SMALL				0x00001005

// DECODER ERROR NUMBERS
#define		VB_ERR_DECODE						0x00002000
#define		VB_ERR_OPEN_FILE					0x00002001
#define		VB_ERR_TOO_MANY_CHANNELS			0x00002002
#define		VB_ERR_EOS							0x00002003


/* other constants */

#define		VB_MAX_HOMEPAGE			256

/* format specific variables */

#define		MPEG1	1
#define		MPEG2	0

#ifdef _VORBISDLL
#undef FLOAT
	#include <Windows.h>
#endif


#define VB_MODE_A 100
#define VB_MODE_B 200
#define VB_MODE_C 300
#define VB_MODE_D 400
#define VB_MODE_E 500

typedef struct VB_ALBUM_INFO_RECORD_TAG
{
	DWORD	dwSize;
	DWORD	dwVersion;

	LPCSTR	strArtist;
	LPCSTR	strAlbum;
	LPCSTR	strTitle;
	LPCSTR	strComment;

	DWORD	dwTrackNumber;
	DWORD	dwTotalTracks;

	// added in version 1
	LPCSTR	strGenre;
	LPCSTR	strEncodedBy;
	LPCSTR	strDateTime;

} VB_ALBUM_INFO_RECORD,*PVB_ALBUM_INFO_RECORD;

typedef struct VB_ALBUM_INFO_TAG
{
	DWORD					dwSize;
	DWORD					dwVersion;
	
	DWORD					dwRecords;
	VB_ALBUM_INFO_RECORD	infoRecord;

} VB_ALBUM_INFO,*PVB_ALBUM_INFO;



typedef struct	{
	DWORD	dwConfig;			// VB_CONFIG_XXXXX
								// Currently only VB_CONFIG_MP3 is supported
	union	
	{
		struct
		{
			// STRUCTURE INFORMATION
			DWORD			dwStructVersion;	
			DWORD			dwStructSize;

			// BASIC ENCODER SETTINGS
			DWORD			dwChannels;		// Number of channels
			DWORD			dwSampleRate;	// Sample rate of input file
			LONG			lMinBitRate;	// Minimum bit rate, in bits per second
			LONG			lNomBitRate;	// Nominal bit rate, in bits per second
			LONG			lMaxBitRate;	// Maximum bit rate, in bits per second
			DWORD			dwInfoMode;		// Vorbis Info Mode VB_MODE_A .. VB_MODE_F
			BYTE			btReserved[256-9*sizeof(DWORD)];	// FUTURE USE, SET TO 0
		} VBV1;					// Vorbis header version 1

	} format;
		
} VB_CONFIG, *PVB_CONFIG;


typedef struct
{
	// Vorbis DLL Version number

	BYTE	byDLLMajorVersion;
	BYTE	byDLLMinorVersion;

	// Vorbis Engine Version Number

	BYTE	byMajorVersion;
	BYTE	byMinorVersion;

	// DLL Release date

	BYTE	byDay;
	BYTE	byMonth;
	WORD	wYear;

	// Vorbis	Homepage URL

	CHAR	zHomepage[VB_MAX_HOMEPAGE + 1];	

} VB_VERSION, *PVB_VERSION;			


// Vorbis Decoder Info struct
typedef struct
{
	DWORD			dwStructSize;
	DWORD			dwStructVersion;

	HVB_STREAM		hStream;

	DWORD			dwTotTime_in_ms;
	DWORD			dwCurTime_in_ms;

	DWORD			dwBufferSize;

	LONG			nBitRate;
	DWORD			dwSampleRate;
	DWORD			dwChannels;

	CHAR			btReserved[256-sizeof(DWORD)*9];

} VB_DEC_INFO, *PVB_DEC_INFO;			


#ifndef _VORBISDLL

typedef VOID	(*VBVERSION)	(PVB_VERSION);

typedef VB_ERR	(*VBENCOPEN)	(PVB_CONFIG,PVB_ALBUM_INFO, PDWORD, PDWORD, PHVB_STREAM);
typedef VB_ERR	(*VBENCWRITE)	(HVB_STREAM, DWORD, PSHORT, PBYTE, PDWORD);
typedef VB_ERR	(*VBENCCLOSE)	(HVB_STREAM, PBYTE, PDWORD);

typedef VB_ERR	(*VBDECOPEN)	(PVB_DEC_INFO,PVB_ALBUM_INFO*,LPCSTR);
typedef VB_ERR	(*VBDECREAD)	(PVB_DEC_INFO,PBYTE,PDWORD);
typedef VB_ERR	(*VBDECSEEK)	(PVB_DEC_INFO,DWORD);
typedef VB_ERR	(*VBDECCLOSE)	(PVB_DEC_INFO);


// GENERAL EXPORT FUNCTIONS
#define	TEXT_VBVERSION	"vbVersion"

// ENCODER SPECIFIC EXPORT FUNCTIONS
#define	TEXT_VBENCOPEN	"vbEncOpen"
#define	TEXT_VBENCWRITE	"vbEncWrite"
#define	TEXT_VBENCCLOSE	"vbEncClose"

// DECODER SPECIFIC EXPORT FUNCTIONS
#define	TEXT_VBDECOPEN	"vbDecOpen"
#define	TEXT_VBDECREAD	"vbDecRead"
#define	TEXT_VBDECSEEK	"vbDecSeek"
#define	TEXT_VBDECCLOSE	"vbDecClose"

#else

// GENERAL EXPORT FUNCTIONS
__declspec(dllexport) VOID vbVersion(PVB_VERSION pbeVersion);


// ENCODER SPECIFIC EXPORT FUNCTIONS
__declspec(dllexport) VB_ERR vbEncOpen
	(
		PVB_CONFIG		pvbConfig, 
		PVB_ALBUM_INFO	pAlbumInfo,
		PDWORD			dwSamples, 
		PDWORD			dwBufferSize, 
		PHVB_STREAM		phbeStream
	);

__declspec(dllexport) VB_ERR vbEncWrite
	(
		HVB_STREAM	hbeStream, 
		DWORD		nSamples, 
		PSHORT		pSamples,
		PBYTE		pOutput,
		PDWORD		pdwOutput
	);

__declspec(dllexport) VB_ERR vbEncClose
	(
		HVB_STREAM	hbeStream,
		PBYTE		pOutput,
		PDWORD		pdwOutput
	);


// DECODER SPECIFIC EXPORT FUNCTIONS

__declspec(dllexport) VB_ERR	vbDecOpen
	(	
		PVB_DEC_INFO	pvbDecInfo,
		PVB_ALBUM_INFO*	ppAlbumInfo,
		LPCSTR			pszFileName
	);


__declspec(dllexport) VB_ERR vbDecRead
	(
		PVB_DEC_INFO	pvbDecInfo,
		PBYTE			pOutBuffer, 
		PDWORD			pdwBytesOut
	);

__declspec(dllexport) VB_ERR vbDecSeek
	(
		PVB_DEC_INFO	pvbDecInfo,
		DWORD			dwSeekTime_in_ms
	);


__declspec(dllexport) VB_ERR vbDecClose
	(	
		PVB_DEC_INFO	pvbDecInfo
	);


#endif

#pragma pack(pop)

#endif

/*
** Copyright (C) 2002-2003 Thees Winkler
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

#if !defined(AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_)
#define AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDTrack.h"
#include "BladeMP3EncDLL.h"
#include "OutPlugin.h"
#include "mmfile.h"	// Hinzugefügt von der Klassenansicht


#define  OUTPUT_WAVE  0
#define  OUTPUT_MP3   1

class CEncoder  
{
public:

	DWORD ProcessData(SHORT* pbsInSamples, DWORD dwNumSamples);
	DWORD ReSample(PSHORT psData, DWORD dwNumSamples);
	DWORD UpMixToStereo( PSHORT psData, PSHORT psOutData,DWORD dwNumSamples);
	DWORD DownMixToMono(PSHORT psData, DWORD dwNumSamples);
	
	CEncoder(CString wd);
	virtual ~CEncoder();

	// Public Initialisation and access methods
	BOOL Init();
	BOOL DeInit();
	void SetAlbumInfo(MMFILE_ALBUMINFO albumInfo);

	BOOL PrepareEncoding(CString strFilename, int nNumchannels, int nSamplerate, int nBitspersample);
	BOOL PrepareMP3(CString strFilename, int nNumchannels, int nSamplerate, int nBitspersample);
	int  GetEstimatedSize(int nSamplesPerSec, int nChannels, int wBitsperSample, __int64 nFileSize);
	unsigned long GetSamplesToRead();


	void PassBuffer(LPVOID pnBuffer, DWORD dwRead);
	void SetOutputFormat(CString outputDLL);

private:
	MMFILE_ALBUMINFO m_albumInfo;
	long nEstimatedSize;
	CString strOutputDLL;
	CString wd;

	FILE*			pFileOut;		
	BE_CONFIG		beConfig;	

	CString			strFileOut;	

	DWORD			dwSamples;
	DWORD			dwMP3Buffer;
	HBE_STREAM		hbeStream;
	BE_ERR			err;

	PBYTE			pMP3Buffer;
	PSHORT			pWAVBuffer;
	HINSTANCE		lameDLL;

	DWORD dwWrite;

	COutPlugin		*outputPlugin;
	LF_OUT*			outModule;


protected:
	int FormatBps(int nVersion, int bps);

	// Stuff for resampling
	SHORT*			m_psInputStream;
	DWORD			m_dwInBufferSize;				// IN SHORTS !
	DOUBLE			m_dResampleRatio;
	BOOL			m_bDownMixToMono;
	BOOL			m_bUpMixToStereo;

	BEINITSTREAM		beInitStream;
	BEENCODECHUNK		beEncodeChunk;
	BEDEINITSTREAM		beDeinitStream;
	BECLOSESTREAM		beCloseStream;
	BEWRITEVBRHEADER	beWriteVBRHeader;

//	BOOL PrepareMP3(CCDTrack *cdTrack);
	BOOL PrepareWave(CString outFile, int nNumchannels, int nSamplerate, int nBitspersample);
};

#endif // !defined(AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_)

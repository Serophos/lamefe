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

#if !defined(AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_)
#define AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MP3File.h"
#include "BladeMP3EncDLL.h"

#define  OUTPUT_WAVE  0
#define  OUTPUT_MP3   1

typedef struct
{
  BYTE  riff[4];            // must be "RIFF"             
  DWORD len;                // #bytes + 44 - 8 
  BYTE  cWavFmt[8];         // must be "WAVEfmt" 
  DWORD dwHdrLen;
  WORD  wFormat;
  WORD  wNumChannels;
  DWORD dwSampleRate;
  DWORD dwBytesPerSec;
  WORD  wBlockAlign;
  WORD  wBitsPerSample;
  BYTE  cData[4];            // must be "data"
  DWORD dwDataLen;           // #bytes 

} WAVHDR, *PWAVHDR;


class Encoder  
{
public:
	int getEstimatedSize(int nSamplesPerSec, int nChannels, int wBitsperSample, int nFileSize);
	unsigned long getSamplesToRead();

	Encoder(CString wd);
	virtual ~Encoder();

	BOOL deInit();
	void passBuffer(LPVOID pnBuffer, DWORD dwRead);
	BOOL prepareEncoding(MP3File *file);
	void setOutputFormat(int outPF);
	BOOL init();

private:
	long nEstimatedSize;
	int opf;
	CString wd;

	FILE*			pFileOut;		
	BE_CONFIG		beConfig;	

	CHAR			strFileIn[255];	
	CHAR			strFileOut[255];	

	DWORD			dwSamples;
	DWORD			dwMP3Buffer;
	HBE_STREAM		hbeStream;
	BE_ERR			err;

	PBYTE			pMP3Buffer;
	PSHORT			pWAVBuffer;
	HINSTANCE		lameDLL;

	DWORD dwWrite;


protected:
	BOOL writeWaveHeader();
	BOOL prepareMP3(MP3File *file);
	BOOL prepareWave(CString outFile);
};

#endif // !defined(AFX_ENCODER_H__A32F6493_DD48_47DC_B853_347FCCF8EE3F__INCLUDED_)

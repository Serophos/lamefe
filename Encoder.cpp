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
#include "Encoder.h"
#include "cfgFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

BEINITSTREAM		beInitStream=NULL;
BEENCODECHUNK		beEncodeChunk=NULL;
BEDEINITSTREAM		beDeinitStream=NULL;
BECLOSESTREAM		beCloseStream=NULL;
BEWRITEVBRHEADER	beWriteVBRHeader=NULL;

Encoder::Encoder(CString wdir)
{

	dwMP3Buffer = NULL;
	dwSamples   = NULL;
	err         = 0;
	hbeStream   = NULL;
	lameDLL     = NULL;
	opf         = -1;
	pMP3Buffer  = NULL;

	dwSamples   = 0;
	dwWrite     = 0;

	wd = wdir;	
}

Encoder::~Encoder()
{

}

BOOL Encoder::init()
{

	switch(opf){
		
	case OUTPUT_MP3:
		lameDLL=LoadLibrary(wd + "\\lame_enc.dll");

		if(lameDLL==NULL)
		{
			AfxMessageBox("Error Loading lam_enc.dll. Please check your installtion", MB_OK+MB_ICONEXCLAMATION,0);
			return FALSE;
		}

		// Get Interface functions
		beInitStream	= (BEINITSTREAM) GetProcAddress(lameDLL, TEXT_BEINITSTREAM);
		beEncodeChunk	= (BEENCODECHUNK) GetProcAddress(lameDLL, TEXT_BEENCODECHUNK);
		beDeinitStream	= (BEDEINITSTREAM) GetProcAddress(lameDLL, TEXT_BEDEINITSTREAM);
		beCloseStream	= (BECLOSESTREAM) GetProcAddress(lameDLL, TEXT_BECLOSESTREAM);
		beWriteVBRHeader= (BEWRITEVBRHEADER) GetProcAddress(lameDLL,TEXT_BEWRITEVBRHEADER);

		// Check if all interfaces are present
		if(!beInitStream || !beEncodeChunk || !beDeinitStream || !beCloseStream || !beWriteVBRHeader)
		{

			AfxMessageBox("Fatal error: Unable to get LAME interfaces.",MB_OK+MB_ICONEXCLAMATION,0);
			return false;
		}

		return true;

	case OUTPUT_WAVE:
		return true;
	
	}

	return false;

}

void Encoder::setOutputFormat(int outPF)
{

	opf = outPF;
}

BOOL Encoder::prepareEncoding(MP3File *file)
{
	
	switch(opf){

	case OUTPUT_WAVE:
		return prepareWave(file->getSaveAs());

	case OUTPUT_MP3:
		return prepareMP3(file);

	}

	return false;
}

void Encoder::passBuffer(LPVOID pnBuffer, DWORD dwRead)
{

	switch(opf){

	case OUTPUT_WAVE:

		if(fwrite((BYTE*)pnBuffer, sizeof(BYTE), dwRead, pFileOut) != dwRead){

			return;
		}
		break;

	case OUTPUT_MP3:
		
		beEncodeChunk(hbeStream, dwRead, (short*)pnBuffer, pMP3Buffer, &dwWrite);

		// Check result
		if(err != BE_ERR_SUCCESSFUL)
		{
			beCloseStream(hbeStream);
			
			return;
		}
		
		// write encoded samples to disk
		if(fwrite(pMP3Buffer, 1, dwWrite, pFileOut) != dwWrite){

			return; //something went wrong. can't guess what it is:)
		}

		break;
	}
}


BOOL Encoder::prepareWave(CString outFile)
{

	pFileOut = fopen(outFile.GetBuffer(10), "wb+");
	outFile.ReleaseBuffer();

	if(!pFileOut){

		return false;
	}



	return writeWaveHeader();
}


BOOL Encoder::prepareMP3(MP3File *file)
{

	CString outFile = file->getSaveAs();
	cfgFile cfg(wd);

	pFileOut = fopen(outFile.GetBuffer(10), "wb+");
	outFile.ReleaseBuffer();


////////////////////////////////////////////////////////////////////////////////
///////////// Now start encoding

	memset(&beConfig,0,sizeof(beConfig));					// clear all fields

	// fill lame config structure
	beConfig.dwConfig = BE_CONFIG_LAME;
	beConfig.format.LHV1.dwStructVersion	= 1;
	beConfig.format.LHV1.dwStructSize		= sizeof(beConfig);		
	beConfig.format.LHV1.dwSampleRate		= file->getWfx()->nSamplesPerSec;//(DWORD)cfg.GetValue(SAMPLES, TRUE);
	beConfig.format.LHV1.dwReSampleRate		= 0;
	beConfig.format.LHV1.nMode				= (LONG)cfg.GetValue(NUMCHNLS, TRUE);		
	beConfig.format.LHV1.dwBitrate			= (DWORD)(DWORD)cfg.GetValue(BITRATE, TRUE);				
	//beConfig.format.LHV1.nPreset			= LQP_HIGH_QUALITY;		
	beConfig.format.LHV1.dwMpegVersion		= (beConfig.format.LHV1.dwSampleRate>=32000)?MPEG1:MPEG2;				
	beConfig.format.LHV1.dwPsyModel			= 0;					
	beConfig.format.LHV1.dwEmphasis			= 0;	
	beConfig.format.LHV1.bCRC				= (BOOL)cfg.GetValue(CRC, FALSE);			
	beConfig.format.LHV1.bOriginal			= (BOOL)cfg.GetValue(ORIGINAL, FALSE); // SET ORIGINAL FLAG
	beConfig.format.LHV1.bCopyright			= (BOOL)cfg.GetValue(ORIGINAL, FALSE);// SET COPYRIGHT FLAG	
	beConfig.format.LHV1.bPrivate			= (BOOL)cfg.GetValue(PRIVATE, FALSE);// SET PRIVATE FLAG
	beConfig.format.LHV1.bNoRes				= TRUE;					// No Bit resorvoir


	if(beInitStream(&beConfig, &dwSamples, &dwMP3Buffer, &hbeStream) != BE_ERR_SUCCESSFUL)
	{

		return false;
	}

	//alocate mp3 buffers
	pMP3Buffer = new BYTE[dwMP3Buffer];


	if(!pMP3Buffer)
	{

		return false;
	}
	return true;
}


BOOL Encoder::deInit()
{

	switch(opf){

	case OUTPUT_MP3:
			err = beDeinitStream(hbeStream, pMP3Buffer, &dwWrite);

			// Check result
			if(err != BE_ERR_SUCCESSFUL)
			{

				beCloseStream(hbeStream);

				return false;
			}

			//some bytes left (returned by Deinitbla...) -> Write to disk!
			if(dwWrite)
			{
				if(fwrite(pMP3Buffer,1,dwWrite,pFileOut) != dwWrite)
				{

					return false;
				}
			}

			// close the MP3 Stream
			beCloseStream(hbeStream);

			// Delete MP3 Buffer
			fclose(pFileOut);
			
			delete [] pMP3Buffer;
			pFileOut = NULL;
			
			// Close output file

			// Write the VBR Tag
			//beWriteVBRHeader(strFileOut);

			FreeLibrary(lameDLL);
		
			break;
	}

	return true;
}

BOOL Encoder::writeWaveHeader()
{

	WAVHDR wav;

	if(!pFileOut){
		
		return false;
	}

	memcpy( wav.riff, "RIFF", 4 );
	memcpy( wav.cWavFmt, "WAVEfmt ", 8 );
	memcpy( wav.cData, "data", 4 );

	wav.len            = 44 - 8;
	wav.dwHdrLen       = 16;
	wav.wFormat        = 1;
	wav.wNumChannels   = 2;
	wav.dwSampleRate   = 44100;
	wav.dwBytesPerSec  = 44100*2*2;
	wav.wBlockAlign    = 4;
	wav.wBitsPerSample = 16;
	wav.dwDataLen      = 0;
	
	fseek(pFileOut, 0, SEEK_SET);
	fwrite(&wav, 1, sizeof(wav), pFileOut);

	return true;
}

unsigned long Encoder::getSamplesToRead()
{

	return dwSamples;
}

int Encoder::getEstimatedSize(int nSamplesPerSec, int nChannels, int wBitsPerSample, int nFileSize)
{
	//Calculate estimated file size
	//One MPEGII Layer 3 Frame with current setting is bitrate*576*samplerate bytes
	long bytesPerFrame = beConfig.format.LHV1.dwBitrate * 576 / (beConfig.format.LHV1.dwSampleRate/1000) / 8;
	//The lenght in seconds of the wavefile is
	long lenght = nFileSize * 8 / (nSamplesPerSec * nChannels * wBitsPerSample);
	//The size of the mp3 will be about
	nEstimatedSize = beConfig.format.LHV1.dwSampleRate / 576 * bytesPerFrame * lenght;
	

	return nEstimatedSize;
}

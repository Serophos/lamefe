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
#include "Ini.h"
#include "Encoder.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


CEncoder::CEncoder(CString wdir)
{

	dwMP3Buffer = NULL;
	dwSamples   = NULL;
	err         = 0;
	hbeStream   = NULL;
	lameDLL     = NULL;
	strOutputDLL = "lame_enc.dll";
	pMP3Buffer  = NULL;

	dwSamples   = 0;
	dwWrite     = 0;

	wd = wdir;	

	beInitStream		= NULL;
	beEncodeChunk		= NULL;
	beDeinitStream		= NULL;
	beCloseStream		= NULL;
	beWriteVBRHeader	= NULL;

	outputPlugin		= NULL;
	outModule			= NULL;

	m_albumInfo.album	= "";
	m_albumInfo.artist	= "";
	m_albumInfo.comment = "";
	m_albumInfo.genre	= "";
	m_albumInfo.song	= "";
	m_albumInfo.track	= 0;
	m_albumInfo.year	= 0;
}

CEncoder::~CEncoder()
{

}

BOOL CEncoder::Init()
{

	if(strOutputDLL == "lame_enc.dll"){

		TRACE("Initializing Encoder for MP3 Output\n");
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
			return TRUE;
		}

	}
	else{
		
		TRACE("Initializing Encoder for Plugin Output\n");

		outputPlugin = new COutPlugin(wd + "\\Plugins\\" + strOutputDLL, wd + "\\lameFE.ini");
		
		if(!outputPlugin->Load()){
	
			TRACE("Failed loading output plugin\n");
			return FALSE;
		}
		outModule = outputPlugin->GetOutModule();
		if(!outModule){

			TRACE("Failed loading outModule fromoutput plugin\n");
			return FALSE;
		}
		return TRUE;
	
	}

	return FALSE;

}

void CEncoder::SetOutputFormat(CString outputDLL)
{

	strOutputDLL = outputDLL;
}

BOOL CEncoder::PrepareEncoding(CString strFilename, int nNumchannels, int nSamplerate, int nBitspersample){

	if(strOutputDLL == "lame_enc.dll"){

		return PrepareMP3(strFilename, nNumchannels, nSamplerate, nBitspersample);
	}
	else{

		return PrepareWave(strFilename, nNumchannels, nSamplerate, nBitspersample);
	}
}

void CEncoder::PassBuffer(LPVOID pnBuffer, DWORD dwRead)
{

	//TRACE("Encoder::passBuffer dwRead=%d\n", dwRead);

	if(strOutputDLL != "lame_enc.dll"){

		if(!outModule){

			return;
		}
		outModule->Write((SHORT*)pnBuffer, dwRead);
	}
	else{

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
	}
}


BOOL CEncoder::PrepareWave(CString outFile, int nNumchannels, int nSamplerate, int nBitspersample)
{

	TRACE("Entering CEncoder::PrepareWave()\n");

	if(!outModule){

		return FALSE;
	}

	MMFILE_FORMAT mmf;
	mmf.dwSampleRate = nSamplerate;
	mmf.nBitsPerSample = nBitspersample;
	mmf.nChannels = nNumchannels;

	if((dwSamples = outModule->Open(outFile, &mmf, &m_albumInfo)) == -1){

		ASSERT(FALSE);
		return FALSE;
	}

	TRACE("Leaving CEncoder::PrepareWave()\n");
	return TRUE;
}


BOOL CEncoder::PrepareMP3(CString strFilename, int nNumchannels, int nSamplerate, int nBitspersample)
{

	TRACE("Entering CEncoder::PrepareMP3()\n");

	CIni cfg;
	CString strPreset;

	cfg.SetIniFileName(wd + "\\LameFE.ini");
	strPreset = cfg.GetValue("L.A.M.E.", "PresetName", "default");
	cfg.SetIniFileName(wd + "\\" + strPreset + ".ini");
	
	pFileOut = fopen(strFilename, "wb+");
	strFileOut = strFilename;

////////////////////////////////////////////////////////////////////////////////
///////////// Now start encoding

	memset(&beConfig,0,sizeof(beConfig));					// clear all fields

	// Structure information
	beConfig.dwConfig						= BE_CONFIG_LAME;
	beConfig.format.LHV1.dwStructVersion	= 1;
	beConfig.format.LHV1.dwStructSize		= sizeof(beConfig);
	

	//Basic Encoder Settings
	int	nCRC		= (BOOL)cfg.GetValue("L.A.M.E.", "Crc", FALSE);
	int nVBR		= (nCRC>>12)&0x0F;
	int nVbrMethod	= (nCRC>>16)&0x0F;

	//Samplerate of input file
	beConfig.format.LHV1.dwSampleRate	= nSamplerate;
	//Downsample rate
	beConfig.format.LHV1.dwReSampleRate	= 0;  //Encoder decides
	//Number of channels
	beConfig.format.LHV1.nMode			= (nNumchannels == 1 ? BE_MP3_MODE_MONO : (LONG)cfg.GetValue("L.A.M.E.", "Channels", 1));		
	//CBR bitrate / VBR min bitrate
	beConfig.format.LHV1.dwBitrate		= (DWORD)FormatBps(cfg.GetValue("L.A.M.E.", "Bitrate", 14));				
	//VBR max bitrate
	beConfig.format.LHV1.dwMaxBitrate	= (DWORD)FormatBps(cfg.GetValue("L.A.M.E.", "MaxBitrate", 16));
	//Quality preset
	beConfig.format.LHV1.nPreset		= cfg.GetValue("L.A.M.E.", "QualityPreset", 0);
	//MPEG Version. This is for future use and isn't supported by the lame_enc.dll yet
	beConfig.format.LHV1.dwMpegVersion	= (beConfig.format.LHV1.dwSampleRate>=32000)? MPEG1 : MPEG2;				
	//Future use
	beConfig.format.LHV1.dwPsyModel		= 0;					
	beConfig.format.LHV1.dwEmphasis		= 0;	
	//Bit stream settings
	beConfig.format.LHV1.bCRC			= nCRC&0x01;			
	beConfig.format.LHV1.bOriginal		= (BOOL)cfg.GetValue("L.A.M.E.", "Original", FALSE);
	beConfig.format.LHV1.bCopyright		= (BOOL)cfg.GetValue("L.A.M.E.", "Copyright", FALSE);	
	beConfig.format.LHV1.bPrivate		= (BOOL)cfg.GetValue("L.A.M.E.", "Private", FALSE);
	
	// allways write the VBR header, even for CBR file
	beConfig.format.LHV1.bWriteVBRHeader	= TRUE;

	//VBR stuff
	if(cfg.GetValue("L.A.M.E.", "VbrMethod", 0) != 0){

		//beConfig.format.LHV1.bWriteVBRHeader = TRUE;
		beConfig.format.LHV1.bEnableVBR		 = TRUE;
		beConfig.format.LHV1.nVBRQuality	 = cfg.GetValue("L.A.M.E.", "VbrQuality", 5);
		beConfig.format.LHV1.dwVbrAbr_bps	 = cfg.GetValue("L.A.M.E.", "Abr", 14) * 1000;
		beConfig.format.LHV1.nVbrMethod		 = (VBRMETHOD)(cfg.GetValue("L.A.M.E.", "VbrMethod", 0) - 1); 
		
	}
	else
	{
		// no ABR selected
		beConfig.format.LHV1.dwVbrAbr_bps = 0;
		beConfig.format.LHV1.nVBRQuality = nVBR ;
	}

	beConfig.format.LHV1.bNoRes			= TRUE;	
	beConfig.format.LHV1.bStrictIso		= FALSE;
	beConfig.format.LHV1.nPreset=( (nCRC >> 8 ) & 0x0F );



	if(beInitStream(&beConfig, &dwSamples, &dwMP3Buffer, &hbeStream) != BE_ERR_SUCCESSFUL)
	{

		return FALSE;
	}

	//alocate mp3 buffers
	pMP3Buffer = new BYTE[dwMP3Buffer];


	if(!pMP3Buffer)
	{

		return FALSE;
	}

	TRACE("Leaving CEncoder::PrepareMP3()\n");
	return TRUE;
}


BOOL CEncoder::DeInit()
{

	TRACE("Entering CEncoder::DeInit()\n");

	CIni cfg;
	cfg.SetIniFileName(wd + "\\LameFE.ini");

	if(strOutputDLL == "lame_enc.dll"){

			TRACE("Deinit Encoder Mode MP3\n");
			err = beDeinitStream(hbeStream, pMP3Buffer, &dwWrite);

			// Check result
			if(err != BE_ERR_SUCCESSFUL)
			{

				beCloseStream(hbeStream);

				return FALSE;
			}

			//some bytes left (returned by Deinitbla...) -> Write to disk!
			if(dwWrite)
			{
				if(fwrite(pMP3Buffer,1,dwWrite,pFileOut) != dwWrite)
				{

					return FALSE;
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
			if(cfg.GetValue("L.A.M.E.", "VbrMethod", 0) != 0){

				//Why the crash :-(
				beWriteVBRHeader(strFileOut);
			}

			FreeLibrary(lameDLL);
		
	}
	else{

		TRACE("Deiniting Encoder WAVE. Closing file\n");
		if(outModule){

			outModule->Close();
			outModule->Quit();
			outputPlugin->Unload();
		}
	}

	TRACE("Leaving CEncoder::DeInit()\n");
	return TRUE;
}


unsigned long CEncoder::GetSamplesToRead()
{

	TRACE("Entering CEncoder::GetSamplesToRead()\n");
	return dwSamples;
}

int CEncoder::GetEstimatedSize(int nSamplesPerSec, int nChannels, int wBitsPerSample, int nFileSize)
{
	
	TRACE("Entering CEncoder::GetEstimatedSize()\n");
	if(strOutputDLL == "lame_enc.dll"){

		//Calculate estimated file size
		//One MPEGII Layer 3 Frame with current setting is bitrate*576*samplerate bytes
		long bytesPerFrame = beConfig.format.LHV1.dwBitrate * 576 / (beConfig.format.LHV1.dwSampleRate/1000) / 8;
		//The lenght in seconds of the wavefile is
		long lenght = nFileSize * 8 / (nSamplesPerSec * nChannels * wBitsPerSample);
		//The size of the mp3 will be about
		nEstimatedSize = beConfig.format.LHV1.dwSampleRate / 576 * bytesPerFrame * lenght;
	}
	else{

		nEstimatedSize = nFileSize;
	}
	

	TRACE("Leaving CEncoder::GetEstimatedSize()\n");
	return nEstimatedSize;
}


void CEncoder::SetAlbumInfo(MMFILE_ALBUMINFO albumInfo)
{

	TRACE("Entering CEncoder::SetAlbumInfo()\n");

	m_albumInfo.album   = albumInfo.album;
	m_albumInfo.artist  = albumInfo.artist;
	m_albumInfo.comment = albumInfo.comment;
	m_albumInfo.genre   = albumInfo.genre;
	m_albumInfo.song    = albumInfo.song;
	m_albumInfo.track   = albumInfo.track;
	m_albumInfo.year    = albumInfo.year;

	TRACE("Leaving CEncoder::SetAlbumInfo()\n");

}

int CEncoder::FormatBps(int bps)
{

/*
 8 bps 0
16 bps 1
24 bps 2
32 bps 3
40 bps 4
48 bps 5
56 bps 6
64 bps 7 //
80 bps  8 
96  bps 9
112 bps 10
128 bps 11
144 bps 12
160 bps 13 //
192 bps 14 (default) 
224 bps 15
256 bps 16
320 bps 17
*/
	int res = 0;
	
	if (bps <= 7){

		res = 8 + 8 * (bps);
	}
	else if ((bps > 7) && (bps <= 13)){

		res = 80 + 16 * (bps - 8);
	}
	else if ((bps > 13) && (bps <= 16)){

		res = 160 + 32 * (bps - 13);
	}
	else{
		res = 320;
	}


	return res;
}

/*
** Copyright (C) 2002 Thees Winkler
**  
** Parts of the code in this file (wave_out.cpp) are from CDEx
** Copyright (C) 1999 - 2001 Albert L. Faber
** These codeparts are marked with a special comment
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
#include <afxdllx.h>
#include "../LibSndFile/src/sndfile.h"
#include "../out_plugin.h"
#include "ConfigDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE Wave_outDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WAVE_OUT.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(Wave_outDLL, hInstance))
			return 0;

		// Diese DLL in Ressourcenkette einfügen
		// HINWEIS: Wird diese Erweiterungs-DLL implizit durch eine reguläre
		//  MFC-DLL (wie z.B. ein ActiveX-Steuerelement) anstelle einer
		//  MFC-Anwendung eingebunden, dann möchten Sie möglicherweise diese
		//  Zeile aus DllMain entfernen und eine eigene Funktion einfügen,
		//  die von dieser Erweiterungs-DLL exportiert wird. Die reguläre DLL,
		//  die diese Erweiterungs-DLL verwendet, sollte dann explizit die 
		//  Initialisierungsfunktion der Erweiterungs-DLL aufrufen. Anderenfalls 
		//  wird das CDynLinkLibrary-Objekt nicht mit der Recourcenkette der 
		//  regulären DLL verbunden, was zu ernsthaften Problemen
		//  führen kann.

		new CDynLinkLibrary(Wave_outDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WAVE_OUT.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(Wave_outDLL);
	}
	return 1;   // OK
}

void  init();
void  quit(void);
int   open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo);
unsigned int  write(short *ptr, size_t items);
void  close(void);
void  configure();
void  about();

// LF_OUT Structure of this Plugin
LF_OUT lameFEOutModule = {

	VERSION,
	"Wave Plugin v 1.1",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	".wav",   
	init,		
	quit,		
	open,	
	write,   
	close,
	configure,
	about,
	NULL,
	NULL
};



SF_INFO			m_sndInfo;
SNDFILE*		m_sndFile			= NULL;
char*			inifile				= NULL;
unsigned int	m_dwResampleRatio	= 0;
BOOL			m_bDownMixToMono	= FALSE;
BOOL			m_bUpMixToStereo	= FALSE;
BOOL			m_nInputChannels	= 0;
SHORT*			m_psUpSampleStream  = NULL;
DWORD			m_dwInBufferSize;			// IN SHORTS !



////////////////////////////////////////////////////////////////////////
// void Init(void) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  init(){


	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("wave_out.dll_samplerate", buffer, sizeof(buffer), FALSE);

	switch(atoi(buffer)){

	case 0: // 44,1
		m_sndInfo.samplerate = 44100;
		break;
	case 1: // 22,05
		m_sndInfo.samplerate = 22050;
		break;
	case 2: // 11,025
		m_sndInfo.samplerate = 11025;
		break;
	}

	//m_sndInfo.pcmbitwidth = 16 ;
	

	lameFEOutModule.GetProfileString("wave_out.dll_channels", buffer, sizeof(buffer), FALSE);
	switch(atoi(buffer)){

	case 0: // Stereo
		m_sndInfo.channels = 2;
		break;
	case 1: // Mono
		m_sndInfo.channels = 1;
		break;
	}
	

	lameFEOutModule.GetProfileString("wave_out.dll_compression", buffer, sizeof(buffer), FALSE);
	switch(atoi(buffer)){

	case 0: // None (PCM)
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		break;
	case 1: // IDA-ADPCM
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM | SF_FORMAT_PCM_16;
		break;
	case 2: // MS-ADPCM
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM | SF_FORMAT_PCM_16;		
			break;
	case 3: // u-law
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_ULAW | SF_FORMAT_PCM_16;
			break;
	case 4: // A-LAW
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_ALAW | SF_FORMAT_PCM_16;
			break;
	default: // just in case :)
		m_sndInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	}
	
	delete buffer;
	buffer = NULL;
}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void quit(void){

	if(!m_sndFile){

		return;
	}
}

////////////////////////////////////////////////////////////////////////
// int Open(...) - Open file for read. Returns -1 on fail and 0 on success
//
// Parameters:
//  const char * path
//      The path of the file to open
////////////////////////////////////////////////////////////////////////
int open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo){

	//OK we don't need that albuminfo for wave :)

	if( !( m_sndFile = sf_open( path, SFM_WRITE, &m_sndInfo ) ) )
	{
		ASSERT( FALSE );
		return -1;
	}


	m_nInputChannels = pInputFormat->nChannels;

/********************************************************************
/* The following code is Copyright (C) 1999 - 2001 Albert L. Faber
/********************************************************************/

	m_dwResampleRatio = pInputFormat->dwSampleRate / m_sndInfo.samplerate;

	if( m_sndInfo.channels == 1 && (pInputFormat->nChannels == 2)){

		m_bDownMixToMono = TRUE;
	}
	else{

		m_bDownMixToMono=FALSE;
	}


	if( m_sndInfo.channels == 2 && (pInputFormat->nChannels == 1)){

		m_bUpMixToStereo=TRUE;
	}
	else{

		m_bUpMixToStereo=FALSE;
	}


	// Set converter size TODO: Remove hardcoded value
	// INPUT BUFFER SIZE IN SAMPLES, THUS SHORTS
	m_dwInBufferSize = 8192;

	if(m_bDownMixToMono){

		m_dwInBufferSize *= 2;
	}


	// Allocate Output Buffer size
//	m_pbtOutStream=new BYTE[m_dwInBufferSize];


	// Only expect half the number of samples
	// in case we have to upsample
	if (m_bUpMixToStereo)
	{
		// Allocate upsample Buffer size
		m_psUpSampleStream=new SHORT[m_dwInBufferSize];

		m_dwInBufferSize /= 2;
	}

/********************************************************************
/*  end of code by Albert L. Faber
/********************************************************************/
	return 8192;
}


////////////////////////////////////////////////////////////////////////
// size_t Read(...) - Reads from file and writes given number of items of
//    raw wave data to the buffer. returns the number of items read.
//
// Parameters:
//  short *ptr
//      Pointer to the buffer to recieve the raw wave data
//  size_t items
//      Items (samples) to read. Must be an integer product of the number
//      of channels or an error will occur.
////////////////////////////////////////////////////////////////////////
unsigned int  write(short *ptr, size_t items){

	if(!m_sndFile){

		return -1;
	}

	unsigned int dwNumSamples = items; //* m_nInputChannels;

/********************************************************************
/* The following code is Copyright (C) 1999 - 2001 Albert L. Faber
/********************************************************************/

	if(m_nInputChannels == 2){

		if(m_dwResampleRatio == 2.0){

			DWORD dwSample;

			for(dwSample = 0; dwSample < dwNumSamples / 4; dwSample++){

				ptr[dwSample * 2 + 0] = ptr[4 * dwSample + 0];
				ptr[dwSample * 2 + 1] = ptr[4 * dwSample + 1];
			}

			dwNumSamples /= 2;
		}

		if(m_dwResampleRatio == 4.0){

			DWORD dwSample;

			for(dwSample = 0; dwSample < dwNumSamples / 8; dwSample++){

				ptr[dwSample * 2 + 0] = ptr[8 * dwSample + 0];
				ptr[dwSample * 2 + 1] = ptr[8 * dwSample + 1];
			}

			dwNumSamples /= 4;
		}
	}
	else{

		if(m_dwResampleRatio == 2.0){

			DWORD dwSample;
			
			for(dwSample = 0; dwSample < dwNumSamples / 2; dwSample++){

				ptr[dwSample] = ptr[2 * dwSample];
			}

			dwNumSamples /= 2;
		}

		if(m_dwResampleRatio == 4.0){
			
			DWORD dwSample;

			for(dwSample = 0; dwSample < dwNumSamples / 4; dwSample++){

				ptr[dwSample] = ptr[4 * dwSample];
			}

			dwNumSamples /= 4;
		}
	}
	


//upmix to stereo
//

	// Downmix to mono?
	if (m_bDownMixToMono)
	{
		//DownMixToMono( pbsInSamples,dwNumSamples );
		DWORD dwSample;

		for (dwSample = 0; dwSample < dwNumSamples / 2; dwSample++){

			ptr[dwSample] = ptr[ 2* dwSample] / 2 + ptr[2 * dwSample + 1] / 2;
		}

		dwNumSamples /= 2;
	}

	// Downmix to mono?
	if (m_bUpMixToStereo)
	{
		//UpMixToStereo(pbsInSamples,m_psUpSampleStream,dwNumSamples);
		int dwSample;

		for (dwSample = dwNumSamples - 1; dwSample >= 0; dwSample--){

			m_psUpSampleStream[2 * dwSample + 0] = ptr[dwSample];
			m_psUpSampleStream[2 * dwSample + 1] = ptr[dwSample];
		}

		dwNumSamples *= 2;

		return sf_write_short(m_sndFile, m_psUpSampleStream, dwNumSamples);
		/*if (SndFileWrite(m_pSndFile,m_psUpSampleStream,dwNumSamples)!=dwNumSamples)
		{
			return CDEX_ERROR;
		}*/

	}
	else
	{
		return sf_write_short(m_sndFile, ptr, dwNumSamples);
		// Write chunk to disk
		/*if (SndFileWrite(m_pSndFile,pbsInSamples,dwNumSamples)!=dwNumSamples)
		{
			return CDEX_ERROR;
		}*/
	}

/********************************************************************
/* code by Albert L. Faber end
/********************************************************************/

}

////////////////////////////////////////////////////////////////////////
// void Close(...) - Close File after reading.
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  close(void){

	if(m_sndFile){

		sf_close(m_sndFile);

		delete m_psUpSampleStream;
		m_psUpSampleStream = NULL;
	}
}

// default is min 32 max 320 oder 350  mittel is bei 4 = 128 bei 5 = 160 bei 6 = 192 usw.

////////////////////////////////////////////////////////////////////////
// void Configure(...) - Show Configuration Dialog for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void configure(){

	CConfigDialog dlg(0);
	
	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("wave_out.dll_samplerate", buffer, sizeof(buffer), FALSE);
	dlg.m_nSamplingRate = atoi(buffer);
	lameFEOutModule.GetProfileString("wave_out.dll_compression", buffer, sizeof(buffer), FALSE);
	dlg.m_nCompression = atoi(buffer);
	lameFEOutModule.GetProfileString("wave_out.dll_channels", buffer, sizeof(buffer), FALSE);
	dlg.m_nChannels = atoi(buffer);

	if(dlg.DoModal() == IDOK){
		
		itoa(dlg.m_nSamplingRate, buffer, 10);
		lameFEOutModule.SetProfileString("wave_out.dll_samplerate", buffer);
		itoa(dlg.m_nCompression, buffer, 10);
		lameFEOutModule.SetProfileString("wave_out.dll_compression", buffer);
		itoa(dlg.m_nChannels, buffer, 10);
		lameFEOutModule.SetProfileString("wave_out.dll_channels", buffer);
	}
}

////////////////////////////////////////////////////////////////////////
// void About(...) - Show Aboutbox for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void about(){

	MessageBox(lameFEOutModule.hMainWindow, 
			"LameFE Wave Audio Writer v1.1\n(c) 2002 by Thees Ch. Winkler\n\nDistributed under GPL.",
			"LibsndFile plugin for lameFE", MB_OK+MB_ICONINFORMATION);

}




////////////////////////////////////////////////////////////////////////
// PLF_OUT  GetLFOutModule(...) - Only exported function of this DLL
//  will return a pointer to the LF_Out lameFEOutModule structure
//  that contains the function pointers of all API functions
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
__declspec( dllexport ) PLF_OUT GetLFOutModule(){

	return &lameFEOutModule;
}

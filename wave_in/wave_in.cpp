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
#include <afxdllx.h>
#include "../LibSndFile/src/sndfile.h"
#include "../plugin_api.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE Wave_inDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WAVE_IN.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(Wave_inDLL, hInstance))
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

		new CDynLinkLibrary(Wave_inDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WAVE_IN.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(Wave_inDLL);
	}
	return 1;   // OK
}

SF_INFO  m_sndInfo;
SNDFILE* m_sndFile  = NULL;



void  init(void);
void  quit(void);
int   open(const char* path);
unsigned int  read(short *ptr, size_t items);
void  close(void);
int   getfilesize();
void  getformat(PMMFILE_FORMAT pMMFileFormat);
void  configure();
void  about();

// LF_IN Structure of this Plugin
LF_IN lameFEInModule = {

	VERSION,
	"LameFE Multimedia File Reader v 1.1 for\nMicrosoft WAV, Apple/SGI AIFF, Sun/NeXT AU format, DEC AU format, RAW PCM data.",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	"wav;aiff;au;raw",   
	init,		
	quit,		
	open,	
	read,   
	close,
	getfilesize,
	getformat,
	configure,
	about,
	NULL,
	NULL,
};


////////////////////////////////////////////////////////////////////////
// void Init(void) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  init(void){


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
int open(const char* path){

	m_sndFile = sf_open(path, SFM_READ, &m_sndInfo);

	if(!m_sndFile){
		
		return -1; //0 success -1 failed
	}
	return 0;
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
unsigned int  read(short *ptr, size_t items){

	if(!m_sndFile){

		return -1;
	}

	return sf_read_short(m_sndFile, ptr, items);
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
	}
}

////////////////////////////////////////////////////////////////////////
// int GetFileSize(...) - Get Size of decompressed file (raw data size)
//    in Kilobyte
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
int getfilesize(){

	return 0;
}


////////////////////////////////////////////////////////////////////////
// void GetMMFileFormat(...) - Get the fileformat of the decompressed
//   raw wave data (samplingrate, bitrate, channels)
//
// Parameters:
//  PMMFILE_FORMAT pMMFileFormat * pMMFileFormat
//      pointer to a MMFILE_FORMAT structure that will recieve the data
////////////////////////////////////////////////////////////////////////
void getformat(PMMFILE_FORMAT pMMFileFormat){

	if(m_sndFile){

		pMMFileFormat->dwSampleRate	  = m_sndInfo.samplerate;
		//pMMFileFormat->nBitsPerSample = m_sndInfo.pcmbitwidth;
		pMMFileFormat->nChannels	  = m_sndInfo.channels;

		switch (m_sndInfo.format & SF_FORMAT_SUBMASK)
		{

			case SF_FORMAT_PCM_S8:
				pMMFileFormat->nBitsPerSample = 8;
			break;
			case SF_FORMAT_PCM_16:
				pMMFileFormat->nBitsPerSample = 16;
			break;
			case SF_FORMAT_PCM_24:
				pMMFileFormat->nBitsPerSample = 24;
			break;
			case SF_FORMAT_PCM_32:
				pMMFileFormat->nBitsPerSample = 32;
			break;
			default:
				pMMFileFormat->nBitsPerSample = -1;
				ASSERT(FALSE);
		}
	}
	else{

		pMMFileFormat->dwSampleRate	  = 0;
		pMMFileFormat->nBitsPerSample = 0;
		pMMFileFormat->nChannels	  = 0;

	}
}

////////////////////////////////////////////////////////////////////////
// void Configure(...) - Show Configuration Dialog for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void configure(){

	MessageBox(lameFEInModule.hMainWindow,
			"Nothing to configure.", "LibSndFile plugin for LameFE", MB_OK+MB_ICONINFORMATION);
}

////////////////////////////////////////////////////////////////////////
// void About(...) - Show Aboutbox for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void about(){

	MessageBox(lameFEInModule.hMainWindow, 
			"LameFE Multimedia File Reader v 1.0 for\nMicrosoft WAV, Apple/SGI AIFF, Sun/NeXT AU format, DEC AU format, RAW PCM data.",
			"LibsndFile plugin for lameFE", MB_OK+MB_ICONINFORMATION);

}


////////////////////////////////////////////////////////////////////////
// PLF_IN  GetLFINModule(...) - Only exported function of this DLL
//  will return a pointer to the LF_IN lameFEInModule structure
//  that contains the function pointers of all API functions
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
__declspec( dllexport ) PLF_IN GetLFINModule(){

	return &lameFEInModule;
}



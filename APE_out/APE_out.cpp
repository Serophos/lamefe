// APE_out.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "./MACLib/MACLib.h"
#include "../out_plugin.h"
#include "ConfigDlg.h"
//#include "cfgFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE APE_outDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("APE_OUT.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(APE_outDLL, hInstance))
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

		new CDynLinkLibrary(APE_outDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("APE_OUT.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(APE_outDLL);
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
	"Monkey Audio Encoder",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	".APE",   
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



IAPECompress	*pAPECompress		= NULL;
int				nErrorCode			= 0;
int				nBlockOffset		= 0;
MMFILE_FORMAT	mmfFileFormat;
//MMFILE_ALBUMINFO *mmfAlbumInfo		= NULL;

////////////////////////////////////////////////////////////////////////
// void Init(void) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  init(){

}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void quit(void){

}

////////////////////////////////////////////////////////////////////////
// int Open(...) - Open file for read. Returns -1 on fail and 0 on success
//
// Parameters:
//  const char * path
//      The path of the file to open
////////////////////////////////////////////////////////////////////////
int open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo){
	
	pAPECompress = CreateIAPECompress();

	if(!pAPECompress){

		TRACE("APE_out.dll :  CreatIAPECompress failed.\n");
		return -1;
	}

	//memcpy(mmfAlbumInfo, pAlbumInfo, sizeof(pAlbumInfo));

	int nCompressionLevel;
	char *buffer = new char[20];
	lameFEOutModule.GetProfileString("APE_out.dll_compression_level", buffer, sizeof(buffer), FALSE);
	nCompressionLevel = atoi(buffer);
	
	WAVEFORMATEX wfeAudioFormat; 
	FillWaveFormatEx(&wfeAudioFormat, pInputFormat->dwSampleRate, pInputFormat->nBitsPerSample, pInputFormat->nChannels);


	// TODO: remove MAX_AUDIO_BYTES_UNKNOWN 
	int nRetVal = pAPECompress->Start(path, &wfeAudioFormat, MAX_AUDIO_BYTES_UNKNOWN, 
		(nCompressionLevel + 1)*1000, NULL, CREATE_WAV_HEADER_ON_DECOMPRESSION);

	if (nRetVal != 0){

		//SAFE_DELETE(pAPECompress)
		TRACE("APE_out.dll :  Error starting encoder.\n");

		return -1;
	}

	
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


	if(!pAPECompress){

		TRACE("APE_out.dll :  Interface not present!!\n");
		return -1;
	}

	pAPECompress->AddData((unsigned char*)ptr, items * sizeof(SHORT));

	return items;
}

////////////////////////////////////////////////////////////////////////
// void Close(...) - Close File after reading.
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  close(void){

	if(!pAPECompress){

		TRACE("APE_out.dll :  Interface not present!!\n");
		return;
	}

	if (pAPECompress->Finish(NULL, 0, 0) != 0){

		TRACE("APE_out.dll :  Error finishing encoder.\n");
	}

	//SAFE_DELETE(pAPECompress)
}


////////////////////////////////////////////////////////////////////////
// void Configure(...) - Show Configuration Dialog for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void configure(){

	CConfigDlg dlg;

	char *buffer = new char[20];
	lameFEOutModule.GetProfileString("APE_out.dll_compression_level", buffer, sizeof(buffer), FALSE);
	dlg.m_nCompressionLevel = atoi(buffer);

	if(dlg.DoModal() == IDOK){

		itoa(dlg.m_nCompressionLevel, buffer, 10);
		lameFEOutModule.SetProfileString("APE_out.dll_compression_level", buffer);
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
			"Monkeys Lossless Audio Encoder. Copyright 2002 by Thees Ch. Winkler. Distributed under GPL\n",
			"APE Encoder (using MACLib 3.92)", MB_OK+MB_ICONINFORMATION);

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

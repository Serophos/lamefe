// APE_in.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include "./MACLib/MACLib.h"
#include "../plugin_api.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE APE_inDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("APE_IN.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(APE_inDLL, hInstance))
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

		new CDynLinkLibrary(APE_inDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("APE_IN.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(APE_inDLL);
	}
	return 1;   // OK
}


void  init(void);
void  quit(void);
int   open(const char* path);
unsigned int  read(short *ptr, size_t items);
void  close(void);
int   getfilesize();
void  getformat(PMMFILE_FORMAT pMMFileFormat);
void  configure();
void  about();


LF_IN lameFEInModule = {

	VERSION,
	"Monkeys Lossless Audio Compression File Format Reader for LameFE.",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	"APE",   
	init,		
	quit,		
	open,	
	read,   
	close,
	getfilesize,
	getformat,
	configure,
	about
};


IAPEDecompress	*apeFileDecompress	= NULL;
int				nErrorCode			= 0;
int				nBlockOffset		= 0;
MMFILE_FORMAT	mmfFileFormat;

void  init(void){


	mmfFileFormat.dwSampleRate    = 0;
	mmfFileFormat.nBitsPerSample  = 0;
	mmfFileFormat.nChannels		  = 0;
}

void quit(void){

}

int open(const char* path){

	apeFileDecompress = CreateIAPEDecompress(path, &nErrorCode);

	if(!apeFileDecompress){

		TRACE("APE_in.dll :  Could not open file\n");
		return -1; //Failed to create APE Interface
	}
	nBlockOffset = 0;

	mmfFileFormat.dwSampleRate		= apeFileDecompress->GetInfo(APE_INFO_SAMPLE_RATE);
	mmfFileFormat.nBitsPerSample	= apeFileDecompress->GetInfo(APE_INFO_BITS_PER_SAMPLE);
	mmfFileFormat.nChannels			= apeFileDecompress->GetInfo(APE_INFO_CHANNELS);

	return 0;
}


// Read: The file read items functions fill the 
// array pointed to by ptr with the requested number 
// of items. The items parameter must be an 
// integer product of the number of channels or an error 
// will occur.
// returns number of items read 
unsigned int  read(short *ptr, size_t items){


	if(!apeFileDecompress){
		
		TRACE("APE_in.dll :  Interface not present\n");
		return -1; // NO file is opened
	}
	
	if(!ptr){

		TRACE("APE_in.dll :  Hey, don't pass me a NULL-Pointer as a buffer u dumb ass!\n");
		return -1;
	}
	// OK, here we'll have to do some mathmatics to get see how many blocks we have 
	// to read to get the required number of channels :-)
	// -the terminology "Sample" refers to a single sample value, and "Block" refers 
	// to a collection	of "Channel" samples.  For simplicity, MAC typically uses blocks
	// everywhere so that channel mis-alignment cannot happen. (i.e. on a CD, a sample is
	// 2 bytes and a block is 4 bytes ([2 bytes / sample] * [2 channels] = 4 bytes))
	// So 
	int nBlocksToRead	 = items / mmfFileFormat.nChannels;
	// not as hard as I thought, hehe

	int  nBlocksRetrieved = 0;
	
	TRACE("nBlocksToRead = %d ; items = %d\n", nBlocksToRead, items);

	if(apeFileDecompress->GetData((char*)ptr, nBlocksToRead, &nBlocksRetrieved) != 0){

		TRACE("APE_in.dll :  Error reading from file.\n");
		return -1;
	}
	
	nBlockOffset += nBlocksRetrieved;

	return nBlocksRetrieved * mmfFileFormat.nChannels;
}

void  close(void){

	if(!apeFileDecompress){

		TRACE("APE_in.dll :  Interface not present\n");
		return;
	}

	apeFileDecompress->~IAPEDecompress();
}

int getfilesize(){

	if(!apeFileDecompress){

		TRACE("APE_in.dll :  Interface not present\n");
		return -1;
	}
	
	return apeFileDecompress->GetInfo(APE_INFO_WAV_TOTAL_BYTES);
}


void getformat(PMMFILE_FORMAT pMMFileFormat){


	if(!apeFileDecompress){

		TRACE("APE_in.dll :  Interface not present\n");
		pMMFileFormat->dwSampleRate	  = 0;
		pMMFileFormat->nBitsPerSample = 0;
		pMMFileFormat->nChannels	  = 0;
	}
	else{

		pMMFileFormat->dwSampleRate    = mmfFileFormat.dwSampleRate;
		pMMFileFormat->nBitsPerSample  = mmfFileFormat.nBitsPerSample;
		pMMFileFormat->nChannels	   = mmfFileFormat.nChannels;
	}
}

void configure(){

	AfxMessageBox("Nothing to configure here.", MB_OK, 0);
}

void about(){

	AfxMessageBox("Monkeys Lossless Audioformat File Reader Plugin.\nCopyright 2002 by Thees Ch. Winkler.\nDistributed under GPL", MB_OK, 0);
}

__declspec( dllexport ) PLF_IN GetLFINModule(){

	return &lameFEInModule;
}


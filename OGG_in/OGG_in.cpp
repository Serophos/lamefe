// OGG_in.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "../plugin_api.h"
#include "../vorbis/DLL/VorbisDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE OGG_inDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("OGG_IN.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(OGG_inDLL, hInstance))
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

		new CDynLinkLibrary(OGG_inDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("OGG_IN.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(OGG_inDLL);
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

	0x002,
	"OGG/Vorbis Decoder plugin.",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	"ogg",   
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


HINSTANCE		hDLL		= NULL;
VB_CONFIG		m_vbConfig;
VBDECOPEN		m_DecOpen	= NULL;
VBDECREAD		m_DecRead	= NULL;
VBDECCLOSE		m_DecClose	= NULL;
VB_DEC_INFO		m_DecInfo;
VB_ALBUM_INFO   m_AlbumInfo;
/*
VB_ERR	vbDecOpen(PVB_DEC_INFO pvbDecInfo, PVB_ALBUM_INFO*	ppAlbumInfo, LPCSTR	pszFileName);
VB_ERR vbDecRead(PVB_DEC_INFO pvbDecInfo, PBYTE pOutBuffer, PDWORD pdwBytesOut);
VB_ERR vbDecSeek(PVB_DEC_INFO pvbDecInfo, DWORD dwSeekTime_in_ms);
VB_ERR vbDecClose(PVB_DEC_INFO	pvbDecInfo);

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


*/
void  init(void){

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		
	CString	strWd = szBuffer;
	strWd = strWd.Left(strWd.ReverseFind('\\'));

	hDLL = LoadLibrary(strWd + "\\Plugins\\vorbis.dll");

	if(!hDLL){

		TRACE("OGG_in.dll :  Failed loading vorbis.dll\n");
		return;
	}

	// Get the DLL functions
	m_DecOpen		= (VBDECOPEN)	GetProcAddress(hDLL, TEXT_VBDECOPEN);
	m_DecRead		= (VBDECREAD)	GetProcAddress(hDLL, TEXT_VBDECREAD);
	m_DecClose		= (VBDECCLOSE)	GetProcAddress(hDLL, TEXT_VBDECCLOSE);
}

void quit(void){

}

int open(const char* path){

	if(!hDLL || !m_DecOpen){

		return -1;
	}

	if(m_DecOpen(&m_DecInfo, NULL, path) != VB_ERR_OK){

		return -1;
	}
	

	return 0;
}


// Read: The file read items functions fill the 
// array pointed to by ptr with the requested number 
// of items. The items parameter must be an 
// integer product of the number of channels or an error 
// will occur.
// returns number of items read 
unsigned int  read(short *ptr, size_t items){

	DWORD dwWrite  = 0;

	m_DecRead(&m_DecInfo, (PBYTE)ptr,&dwWrite);

	//SetPercent( (INT) ( (double)m_decInfo.dwCurTime_in_ms * 100.0 / m_decInfo.dwTotTime_in_ms ) );

	return dwWrite;
}

void  close(void){

	if (hDLL)
	{
		m_DecClose(&m_DecInfo);

		FreeLibrary(hDLL);

		hDLL = NULL;
	}
}

int getfilesize(){

	DWORD  dwTotTime_in_s = m_DecInfo.dwTotTime_in_ms *1000;
/*
	LONG			nBitRate;
	DWORD			dwSampleRate;
	DWORD			dwChannels;
*/
	int nFileSize = m_DecInfo.dwSampleRate * m_DecInfo.nBitRate * m_DecInfo.dwChannels * dwTotTime_in_s;
	
	return nFileSize;
}


void getformat(PMMFILE_FORMAT pMMFileFormat){

	pMMFileFormat->nChannels      = m_DecInfo.dwChannels;
	pMMFileFormat->nBitsPerSample = 16; //m_DecInfo.nBitRate;
	pMMFileFormat->dwSampleRate   = m_DecInfo.dwSampleRate;
}

void configure(){

	AfxMessageBox("Nothing to configure here.", MB_OK, 0);
}

void about(){

	AfxMessageBox("OGG/Vorbis decoder plugin.\nCopyright 2002 by Thees Ch. Winkler.\nDistributed under GPL", MB_OK, 0);
}

__declspec( dllexport ) PLF_IN GetLFINModule(){

	return &lameFEInModule;
}


// OGG_out.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "../vorbis/DLL/VorbisDLL.h"
#include "../out_plugin.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE OGG_outDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("OGG_OUT.DLL Initializing!\n");
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(OGG_outDLL, hInstance))
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

		new CDynLinkLibrary(OGG_outDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("OGG_OUT.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schließen
		AfxTermExtensionModule(OGG_outDLL);
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

//CString			m_strWd;
HINSTANCE		hDLL		= NULL;
VBENCOPEN		m_EncOpen	= NULL;
VBENCWRITE		m_EncWrite	= NULL;
VBENCCLOSE		m_EncClose	= NULL;
VBVERSION		m_Version	= NULL;
VB_CONFIG		m_vbConfig;

FILE*			m_pFile		= NULL;
DWORD			m_dwOutBufferSize;			// IN BYTES !
DWORD			m_dwInBufferSize;			// IN SHORTS !
HVB_STREAM		m_hvbStream;
BYTE*			m_pbtOutStream = NULL;				// OUTPUT BYTE BUFFER

// LF_OUT Structure of this Plugin
LF_OUT lameFEOutModule = {

	VERSION,
	"Ogg/Vorbis Encoder",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	".OGG",   
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



////////////////////////////////////////////////////////////////////////
// void Init(void) - will be called after loading the plugin and after
// filling in the hMainWindow and hDllInstance datamembers
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  init(){

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		
	CString	strWd = szBuffer;
	strWd = strWd.Left(strWd.ReverseFind('\\'));

	hDLL = LoadLibrary(strWd + "\\Plugins\\vorbis.dll");

	if(!hDLL){

		TRACE("OGG_out.dll :  Failed loading vorbis.dll\n");
		return;
	}

	// Get the DLL functions
	m_EncOpen		= (VBENCOPEN)	GetProcAddress(hDLL, TEXT_VBENCOPEN);
	m_EncWrite		= (VBENCWRITE)	GetProcAddress(hDLL, TEXT_VBENCWRITE);
	m_EncClose		= (VBENCCLOSE)	GetProcAddress(hDLL, TEXT_VBENCCLOSE);
	m_Version		= (VBVERSION)	GetProcAddress(hDLL, TEXT_VBVERSION);
}

////////////////////////////////////////////////////////////////////////
// void Quit(void) - will be called before unloading the plugin 
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void quit(void){

}


int getformatedvalue(int n){
	
	int nResult = 0;
	switch(n){

	case 0: // 	32 kb/s
		nResult = 32;
		break;
	case 1: // 40 kb/s
		nResult = 40;
		break;
	case 2: // 48 kb/s
		nResult = 48;
		break;
	case 3: //64 kb/s 
		nResult = 64;
		break;
	case 4: // 80 kb/s
		nResult = 80;
		break;
	case 5: // 96 kb/s
		nResult = 96;
		break;
	case 6: // 128 kb/s
		nResult = 128;
		break;
	case 7: // 160 kb/s
		nResult = 160;
		break;
	case 8: // 196 kb/s
		nResult = 196;
		break;
	case 9: // 256 kb/s
		nResult = 256;
		break;
	case 20: // 350 kb/s
		nResult = 350;
		break;
	}

	return nResult;
}
////////////////////////////////////////////////////////////////////////
// int Open(...) - Open file for read. Returns -1 on fail and 0 on success
//
// Parameters:
//  const char * path
//      The path of the file to open
////////////////////////////////////////////////////////////////////////
int open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo){
	
	VB_ALBUM_INFO vbAlbumInfo	={0,};

	//vbAlbumInfo
	vbAlbumInfo.dwSize=sizeof(vbAlbumInfo);
	vbAlbumInfo.dwVersion=VB_ALB_STRUCT_VERSION;
	vbAlbumInfo.dwRecords=1;

	vbAlbumInfo.infoRecord.dwSize=sizeof(vbAlbumInfo.infoRecord);
	vbAlbumInfo.infoRecord.dwVersion=VB_ALB_STRUCT_VERSION;

	vbAlbumInfo.infoRecord.strArtist = pAlbumInfo->artist;
	vbAlbumInfo.infoRecord.strAlbum = pAlbumInfo->album;
	vbAlbumInfo.infoRecord.strTitle = pAlbumInfo->song;
	vbAlbumInfo.infoRecord.strComment = pAlbumInfo->comment;
	vbAlbumInfo.infoRecord.dwTrackNumber = pAlbumInfo->track;
	vbAlbumInfo.infoRecord.dwTotalTracks = pAlbumInfo->track;
	vbAlbumInfo.infoRecord.strEncodedBy = "OGG/Vorbis Plugin for LameFE 2.1";
	
	CString strDateTime;
    struct tm*		newtime;
    time_t			long_time;

    time( &long_time );
    newtime = localtime( &long_time );

    strDateTime.Format( "%04d:%02d:%02d",
							newtime->tm_year + 1900,
							newtime->tm_mon + 1,
							newtime->tm_mday
						);
	vbAlbumInfo.infoRecord.strDateTime = strDateTime;

	m_vbConfig.format.VBV1.dwSampleRate	= pInputFormat->dwSampleRate;
	
	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("OGG_out.dll_minbitrate", buffer, sizeof(buffer), FALSE);
	m_vbConfig.format.VBV1.lMinBitRate	= getformatedvalue(atoi(buffer)) * 1000;
	lameFEOutModule.GetProfileString("OGG_out.dll_nombitrate", buffer, sizeof(buffer), FALSE);
	m_vbConfig.format.VBV1.lNomBitRate	= getformatedvalue(atoi(buffer)) * 1000;
	lameFEOutModule.GetProfileString("OGG_out.dll_maxbitrate", buffer, sizeof(buffer), FALSE);
	m_vbConfig.format.VBV1.lMaxBitRate	= getformatedvalue(atoi(buffer)) * 1000;

	m_vbConfig.format.VBV1.dwInfoMode	= VB_MODE_A;
	m_vbConfig.format.VBV1.dwChannels	= pInputFormat->nChannels;

	m_pFile = fopen(path, "wb+");

	if(!m_pFile){

		TRACE("OGG_out.dll :  Could not create/opren file\n");
		return -1;
	}

	if (m_EncOpen(&m_vbConfig,&vbAlbumInfo, &m_dwInBufferSize,&m_dwOutBufferSize,&m_hvbStream)==0x00000001)
	{
		TRACE("OGG_out.dll :  Invalid Input Stream\n");
		return -1;
	}

	// Allocate Output Buffer size
	m_pbtOutStream=new BYTE[m_dwOutBufferSize];

	return m_dwOutBufferSize;
}


////////////////////////////////////////////////////////////////////////
// size_t write(...) - Reads from file and writes given number of items of
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

	DWORD nBytesToWrite = 0;
	DWORD dwNumSamples  = items;

	ASSERT(ptr);
	
/*
__declspec(dllexport) VB_ERR vbEncWrite
	(
		HVB_STREAM	hbeStream, 
		DWORD		nSamples, 
		PSHORT		pSamples,
		PBYTE		pOutput,
		PDWORD		pdwOutput
	);
*/
	if ( m_EncWrite(m_hvbStream, dwNumSamples, ptr, m_pbtOutStream, &nBytesToWrite) == 1)
	{
		ASSERT(FALSE);
		return -1;
	}

	// Did we receive some output bytes
	if (nBytesToWrite > 0)
	{

		if (fwrite(m_pbtOutStream, nBytesToWrite, 1, m_pFile) != 1)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	return items;
}

////////////////////////////////////////////////////////////////////////
// void Close(...) - Close File after reading.
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  close(void){

	DWORD nBytesToWrite=0;

	// Get the last bytes out of the encoder
	if(m_EncClose(m_hvbStream, m_pbtOutStream, &nBytesToWrite) != 0)	
	{
		ASSERT(FALSE);

		return;
	}

	// Did we receive some output bytes
	if (nBytesToWrite > 0){

		// Guess so, write it to the output file
		if (fwrite(m_pbtOutStream, nBytesToWrite, 1, m_pFile) != 1){
			
			ASSERT(FALSE);
			return;
		}
	}

	// Close the Stream

	// Delete the output stream
	delete [] m_pbtOutStream;
	m_pbtOutStream=NULL;


	// Close the output file
	if (m_pFile){

		fclose(m_pFile);
		m_pFile = NULL;
	}

	if (hDLL){

		FreeLibrary(hDLL);
		hDLL = NULL;
	}
}


////////////////////////////////////////////////////////////////////////
// void Configure(...) - Show Configuration Dialog for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void configure(){

	CConfigDlg dlg(NULL);

	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("OGG_out.dll_minbitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nMinBitrate	= atoi(buffer);
	lameFEOutModule.GetProfileString("OGG_out.dll_nombitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nNomBitrate	= atoi(buffer);
	lameFEOutModule.GetProfileString("OGG_out.dll_maxbitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nMaxBitrate	= atoi(buffer);

	int nResult = dlg.DoModal();

	if(nResult == IDOK){

		itoa(dlg.m_nMinBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_minbitrate", buffer);
		itoa(dlg.m_nMaxBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_maxbitrate", buffer);
		itoa(dlg.m_nNomBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_nombitrate", buffer);
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
			"OGG/Vorbis Encoder Plugin for LameFE (c) 2002 by Thees Ch. Winkler\nDistributed under GPL.",
			"OGG/Vorbis Encoder Plugin", MB_OK+MB_ICONINFORMATION);

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

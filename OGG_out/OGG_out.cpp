// OGG_out.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "../BladeMP3EncDLL.h"
#include "vorbis/vorbisenc.h"
#include "../out_plugin.h"
#include "utf8.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _UNICODE
#undef W2A
#define W2A(a) (LPSTR)(LPCSTR)(a)
#undef A2W
#define A2W(a) a
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

HINSTANCE		m_hLibVorbisDLL;
HINSTANCE		m_hOggDLL;
HINSTANCE		m_hVorbisEncDLL;

CString			m_strStreamFileName;
const char*		m_lasterror;
int				m_brmode;
float			m_base_quality;
bool			m_eos;
ogg_stream_state m_os;
ogg_page         m_og;
ogg_packet       m_op;
vorbis_info      m_vi;
vorbis_comment   m_vc;
vorbis_dsp_state m_vd;
vorbis_block     m_vb;
INT				m_nBitrate;
INT				m_nMaxBitrate;
INT				m_nMinBitrate;
INT				m_nSampleRate;
INT				m_nOutSampleRate;
INT				m_nInputChannels;
INT				m_nMode;
DOUBLE			m_dResampleRatio;
BOOL			m_bDownMixToMono;
BOOL			m_bUpMixToStereo;
BYTE*			m_pbOutputStream;				// OUTPUT BYTE BUFFER
FILE*			m_fpOut;
DWORD			m_dwOutBufferSize;				// IN BYTES !
SHORT*			m_psInputStream;
DWORD			m_dwInBufferSize;				// IN SHORTS !


	// vorbisenc.dll function pointer
int (*vorbis_encode_init_func)(vorbis_info *vi,long channels,long rate,
	long max_bitrate,long nominal_bitrate,long min_bitrate);
int (*vorbis_encode_init_vbr_func)(vorbis_info *vi,long channels,
	long rate,float base_quality);

// vorbis.dll function pointer
void (*vorbis_info_init_func)(vorbis_info *vi);
void (*vorbis_info_clear_func)(vorbis_info *vi);
void (*vorbis_dsp_clear_func)(vorbis_dsp_state *v);
int  (*vorbis_block_init_func)(vorbis_dsp_state *v, vorbis_block *vb);
int  (*vorbis_block_clear_func)(vorbis_block *vb);

void (*vorbis_comment_init_func)(vorbis_comment *vc);
void (*vorbis_comment_add_func)(vorbis_comment *vc, char *comment);
void (*vorbis_comment_add_tag_func)(vorbis_comment *vc, char *tag, char *contents);
void (*vorbis_comment_clear_func)(vorbis_comment *vc);

int (*vorbis_analysis_init_func)(vorbis_dsp_state *v,vorbis_info *vi);
int (*vorbis_analysis_func)(vorbis_block *vb,ogg_packet *op);
int (*vorbis_analysis_headerout_func)(vorbis_dsp_state *v,vorbis_comment *vc,
	ogg_packet *op,ogg_packet *op_comm,ogg_packet *op_code);
float **(*vorbis_analysis_buffer_func)(vorbis_dsp_state *v,int vals);
int (*vorbis_analysis_wrote_func)(vorbis_dsp_state *v,int vals);
int (*vorbis_analysis_blockout_func)(vorbis_dsp_state *v,vorbis_block *vb);
int (*vorbis_synthesis_headerin_func)(vorbis_info *vi,vorbis_comment *vc,ogg_packet *op);
int (*vorbis_bitrate_addblock_func)(vorbis_block *vb);
int (*vorbis_bitrate_flushpacket_func)(vorbis_dsp_state *vd,ogg_packet *op);

// ogg.dll function pointer
int (*ogg_stream_init_func)(ogg_stream_state *os,int serialno);
int (*ogg_stream_packetin_func)(ogg_stream_state *os, ogg_packet *op);
int (*ogg_stream_flush_func)(ogg_stream_state *os, ogg_page *og);
int (*ogg_stream_pageout_func)(ogg_stream_state *os, ogg_page *og);
int (*ogg_page_eos_func)(ogg_page *og);
int (*ogg_stream_clear_func)(ogg_stream_state *os);



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

BOOL AddUtfTag( vorbis_comment *vc, const CString& strField, const CString& strFieldValue )
{
	CHAR*			lpszValue = NULL;
	wchar_t*		lpwszValue = NULL;
	unsigned int	nLen = 0;
	unsigned int	i =0;
	
	UINT uiActiveCodePage = GetACP();

	// first convert to wide character 
	lpwszValue = new wchar_t[ strFieldValue.GetLength() + 1 ];

#ifndef _UNICODE
	// convert ASCII string to WideCharacter string based on active code page
	MultiByteToWideChar( uiActiveCodePage , 0, strFieldValue, -1 ,lpwszValue, strFieldValue.GetLength() + 1  );

	// get the length
	nLen = WideCharToUTF8( NULL, lpwszValue );

	// allocate string
	lpszValue = new CHAR[ nLen ];

	// do conversion to UTF string
	WideCharToUTF8( lpszValue, lpwszValue );

#else
	// string is already in UNICODE, so no conversion needed
	lpszValue = (CHAR*)( (LPCTSTR)strFieldValue );
#endif

	// add comment
	vorbis_comment_add_tag_func( vc, W2A( strField ), lpszValue );

	// cleanup

	delete [] lpwszValue;
	delete [] lpszValue;

	return TRUE;
}

BOOL WriteVorbisFrame()
{
	BOOL bReturn = TRUE;

	ASSERT( m_fpOut );

	// write header to output file
	if ( 1 != fwrite( m_og.header, m_og.header_len, 1, m_fpOut ) )
	{
		ASSERT( FALSE );
		bReturn = FALSE;
		m_eos = true;
	}

	// write body to output file
	if ( 1 != fwrite( m_og.body, m_og.body_len, 1, m_fpOut ) )
	{
		ASSERT( FALSE );
		bReturn = FALSE;
		m_eos = true;
	}

	return bReturn;
}


DWORD DownMixToMono( SHORT* psData,DWORD dwNumSamples )
{
	DWORD dwSample;
	for (dwSample=0;dwSample<dwNumSamples/2;dwSample++)
	{
		psData[dwSample]=psData[ 2 * dwSample ] / 2 + psData[ 2 * dwSample + 1 ] / 2;
	}

	return dwNumSamples / 2;
}


DWORD UpMixToStereo( SHORT* psData, SHORT* psOutData, DWORD dwNumSamples )
{
	int dwSample;
	for ( dwSample = dwNumSamples - 1; dwSample >= 0; dwSample-- )
	{
		psOutData[ 2 * dwSample + 0 ] = psData[ dwSample ];
		psOutData[ 2 * dwSample + 1 ] = psData[ dwSample ];
	}
	return dwNumSamples * 2;
}
DWORD ProcessData( SHORT* pbsInSamples, DWORD dwNumSamples )
{
	DWORD	dwSample = 0;
	PSHORT 	psData = pbsInSamples;
	BOOL	bSilence = FALSE;
	PSHORT	pFifoBuffer = NULL;


	// Downmix to mono?
	if ( m_bDownMixToMono )
	{
		dwNumSamples = DownMixToMono( pbsInSamples, dwNumSamples );
	} else if ( m_bUpMixToStereo )
	{
		dwNumSamples = UpMixToStereo( pbsInSamples, m_psInputStream, dwNumSamples );
		psData = m_psInputStream;
	}


	return dwNumSamples;
}

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
	
	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		
	CString	strWd = szBuffer;
	strWd = strWd.Left(strWd.ReverseFind('\\'));
	strWd += "\\Plugins\\";

	m_hOggDLL		= LoadLibrary(strWd + "ogg.dll");
	m_hLibVorbisDLL = LoadLibrary(strWd + "vorbis.dll");
	m_hVorbisEncDLL = LoadLibrary(strWd + "vorbisend.dll");

	if (!m_hOggDLL  || !m_hLibVorbisDLL || !m_hVorbisEncDLL)
	{
		TRACE("Error Inititalising OGG_out.dll: couldn't load Vorbis DLLs\n");

	}
	else
	{

		// Get the function pointers of the vorbisenc DLL
		vorbis_encode_init_func = (int (*)(vorbis_info *vi,long channels,long rate,long max_bitrate,long nominal_bitrate,long min_bitrate))
			::GetProcAddress( m_hVorbisEncDLL, "vorbis_encode_init" );
		vorbis_encode_init_vbr_func = (int (*)(vorbis_info *vi,long channels,long rate,float base_quality))
			::GetProcAddress( m_hVorbisEncDLL, "vorbis_encode_init_vbr" );


		// Get the function pointers of the vorbis DLL
		vorbis_info_init_func = (void (*)(vorbis_info *vi))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_info_init" );

		vorbis_info_clear_func = (void (*)(vorbis_info *vi))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_info_clear" );

		vorbis_dsp_clear_func = (void (*)(vorbis_dsp_state *v))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_dsp_clear" );

		vorbis_block_init_func = (int  (*)(vorbis_dsp_state *v, vorbis_block *vb))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_block_init" );

		vorbis_block_clear_func = (int  (*)(vorbis_block *vb))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_block_clear" );

		vorbis_comment_init_func = (void (*)(vorbis_comment *vc))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_comment_init" );

		vorbis_comment_add_func = (void (*)(vorbis_comment *vc, char *comment))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_comment_add" );

		vorbis_comment_add_tag_func = (void (*)(vorbis_comment *vc, char *tag, char *contents))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_comment_add_tag" );

		vorbis_comment_clear_func = (void (*)(vorbis_comment *vc))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_comment_clear" );

		vorbis_analysis_init_func = (int (*)(vorbis_dsp_state *v,vorbis_info *vi))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_analysis_init" );

		vorbis_analysis_func = (int (*)(vorbis_block *vb,ogg_packet *op))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_analysis" );

		vorbis_analysis_headerout_func = (int (*)(vorbis_dsp_state *v,vorbis_comment *vc,ogg_packet *op,ogg_packet *op_comm,ogg_packet *op_code))
			::GetProcAddress(m_hLibVorbisDLL, "vorbis_analysis_headerout" );

		vorbis_analysis_buffer_func = (float **(*)(vorbis_dsp_state *v,int vals))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_analysis_buffer" );

		vorbis_analysis_wrote_func = (int (*)(vorbis_dsp_state *v,int vals))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_analysis_wrote" );

		vorbis_analysis_blockout_func = (int (*)(vorbis_dsp_state *v,vorbis_block *vb))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_analysis_blockout" );

		vorbis_synthesis_headerin_func = (int (*)(vorbis_info *vi,vorbis_comment *vc,ogg_packet *op))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_synthesis_headerin" );

		vorbis_bitrate_addblock_func = (int (*)(vorbis_block *vb))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_bitrate_addblock" );

		vorbis_bitrate_flushpacket_func = (int (*)(vorbis_dsp_state *vd,ogg_packet *op))
			::GetProcAddress( m_hLibVorbisDLL,  "vorbis_bitrate_flushpacket" );

		// Get the function pointers of the ogg DLL
		ogg_stream_init_func = (int (*)(ogg_stream_state *os,int serialno))
			::GetProcAddress( m_hOggDLL,  "ogg_stream_init" );
		ogg_stream_packetin_func = (int (*)(ogg_stream_state *os, ogg_packet *op))
			::GetProcAddress( m_hOggDLL,  "ogg_stream_packetin" );
		ogg_stream_flush_func = (int (*)(ogg_stream_state *os, ogg_page *og))
			::GetProcAddress( m_hOggDLL,  "ogg_stream_flush" );
		ogg_stream_pageout_func = (int (*)(ogg_stream_state *os, ogg_page *og))
			::GetProcAddress(m_hOggDLL,  "ogg_stream_pageout" );
		ogg_page_eos_func = (int (*)(ogg_page *og))
			::GetProcAddress( m_hOggDLL, "ogg_page_eos" );
		ogg_stream_clear_func = (int (*)(ogg_stream_state *os))
			::GetProcAddress( m_hOggDLL, "ogg_stream_clear" );


		if ( !(
			vorbis_encode_init_func != NULL && 
			vorbis_encode_init_vbr_func != NULL && 
			vorbis_info_init_func  != NULL && 
			vorbis_block_init_func != NULL &&
			vorbis_block_clear_func != NULL &&
			vorbis_comment_init_func != NULL &&
			vorbis_comment_add_func != NULL && 
			vorbis_comment_add_tag_func != NULL &&
			vorbis_comment_clear_func != NULL && 
			vorbis_analysis_init_func != NULL &&
			vorbis_analysis_func != NULL && 
			vorbis_analysis_headerout_func != NULL &&
			vorbis_analysis_buffer_func != NULL && 
			vorbis_analysis_wrote_func != NULL &&
			vorbis_analysis_blockout_func != NULL && 
			vorbis_synthesis_headerin_func != NULL &&
			vorbis_dsp_clear_func != NULL && 
			vorbis_info_clear_func != NULL &&
			ogg_stream_init_func != NULL && 
			ogg_stream_packetin_func != NULL &&
			ogg_stream_flush_func != NULL && 
			ogg_stream_pageout_func != NULL &&
			ogg_page_eos_func != NULL && 
			ogg_stream_clear_func != NULL ) )
		{
				ASSERT( FALSE );
		}

	}

	BOOL		bReturn		= TRUE;
	DWORD		dwInfoMode	= 0;
	bool		bStereo		= true;
	float		fQuality	= 1.0f;
	int			nReturn     = 0;


	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("OGG_out.dll_minbitrate", buffer, sizeof(buffer), FALSE);
	LONG	lMinBitRate	= getformatedvalue(atoi(buffer)) * 1000;

	lameFEOutModule.GetProfileString("OGG_out.dll_nombitrate", buffer, sizeof(buffer), FALSE);
	LONG	lNomBitRate	= getformatedvalue(atoi(buffer)) * 1000;
	
	lameFEOutModule.GetProfileString("OGG_out.dll_maxbitrate", buffer, sizeof(buffer), FALSE);
	LONG	lMaxBitRate		= getformatedvalue(atoi(buffer)) * 1000;
	
//	USES_CONVERSION;

	// clear end of stream flag
	m_eos = false;

	// setup bitrates
	if ( lMaxBitRate <= 0 ) 
	{
		lMaxBitRate = -1;
	}

	if ( lMinBitRate <= 0 ) 
	{
		lMinBitRate = -1;
	}
	
	if ( lNomBitRate <= 0 ) 
	{
		lNomBitRate = -1;
	}

	if ( -1 == lNomBitRate && -1 == lMinBitRate && -1 == lMaxBitRate )
	{
		lNomBitRate = 160000;
	}

	// setup number of input channels
	m_nInputChannels = pInputFormat->nChannels;

	// setup number of output channels
	if ( ( m_nMode & 0xFFFF ) == BE_MP3_MODE_MONO )
	{
		bStereo = false;
	}


	// mixer setup
	if ( ( false == bStereo ) && ( 2 == pInputFormat->nChannels ) )
	{
		m_bDownMixToMono  = TRUE;
	}
	else
	{
		m_bDownMixToMono = FALSE;
	}

	if (  ( true == bStereo ) && ( 1 == pInputFormat->nChannels ) )
	{
		m_bUpMixToStereo = TRUE;
	}
	else
	{
		m_bUpMixToStereo = FALSE;
	}

	// setup quality
/*	fQuality = ( (float) ((SHORT)LOWORD( GetUserN1() ) ) / 1000.0f );

	if ( fQuality < -0.1f )
	{
		fQuality = -0.1f;
	}

	if (  fQuality > 1.0f ) 
	{*/
		fQuality = 1.0f;
	//}

	// Have vorbisenc choose a mode for us
	vorbis_info_init_func( &m_vi );

	// choose proper encoder, based on user settings
	if ( 0 == HIWORD( 500 ) )
	{
		TRACE( _T( "OGG_out.dll: managed bit rate, %d %d %d\n" ),
				lMinBitRate, lNomBitRate, lMaxBitRate );

		/* setup the encoder parameters */
		nReturn = vorbis_encode_init_func(	&m_vi,
											(bStereo == true )? 2 : 1,
											pInputFormat->dwSampleRate,
											lMaxBitRate,
											lNomBitRate,
											lMinBitRate );

	}
	else
	{
		TRACE( _T( "OGG_out.dll open:, using quality option %f\n" ),
				fQuality );

		/* setup the encoder parameters for VBR encoding using quality parameter */
		nReturn = vorbis_encode_init_vbr_func(	&m_vi,
												(bStereo == true )? 2 : 1,
												pInputFormat->dwSampleRate,
												fQuality );
	}


	if ( nReturn != 0 )
	{
		
		return -1;
	}
	else
	{

		// add a comment
	   vorbis_comment_init_func( &m_vc );

		// Add tag info
		if ( pAlbumInfo )
		{
			CString strArtist;
			CString strAlbum;
			CString strTitle;
			CString strComment;
			CString strEncodedBy;
			CString	strYear;
			CString	strGenre;
			CString	strLang;
			CString strMCDI;

			DWORD	dwTrackNumber = pAlbumInfo->track;
			DWORD	dwTotalTracks = 0;

			strArtist	 = pAlbumInfo->artist;
			strAlbum	 = pAlbumInfo->album;
			strTitle	 = pAlbumInfo->song;
			strComment	 = pAlbumInfo->comment;
			strEncodedBy = "LameFE Ogg_out.dll"; // Hydra
			strMCDI		 = ""; // NOT USED YET

			strYear  = pAlbumInfo->year;
			strGenre = pAlbumInfo->genre;

			if ( strComment.IsEmpty() )
			{
				strComment = strEncodedBy;
			}

			AddUtfTag(&m_vc, _T( "COMMENT" ), strComment );

			if ( !strTitle.IsEmpty() )
			{
				AddUtfTag(&m_vc, _T( "TITLE" ), strTitle );
			}

			if ( !strArtist.IsEmpty() )
			{
				AddUtfTag(&m_vc, _T( "ARTIST" ), strArtist );
			}

			if ( !strAlbum.IsEmpty() )
			{
				AddUtfTag(&m_vc, _T( "ALBUM" ), strAlbum );
			}

			if ( dwTrackNumber > 0 )
			{
					CString strTrackNumber;

					strTrackNumber.Format( _T( "%02d"), dwTrackNumber );

					AddUtfTag( &m_vc, _T( "TRACKNUMBER" ), strTrackNumber );		
			}

			if ( !strEncodedBy.IsEmpty() ) 
			{
				AddUtfTag(&m_vc, _T( "ENCODEDBY" ), strEncodedBy );
			}


			if ( !strGenre.IsEmpty() ) 
			{
				AddUtfTag(&m_vc, _T( "GENRE" ), strGenre );
			}

			if ( !strYear.IsEmpty() ) 
			{
				AddUtfTag(&m_vc, _T( "DATE" ), strYear );
			}
		}

		// set up the analysis state and auxiliary encoding storage
		vorbis_analysis_init_func( &m_vd, &m_vi );
		vorbis_block_init_func( &m_vd, &m_vb );


		// set up our packet->stream encoder 
		// pick a random serial number; that way we can more likely build
		// chained streams just by concatenation

		srand( time( NULL ) );

		ogg_stream_init_func( &m_os, GetTickCount() );

		// Open output stream
		m_fpOut = _tfopen( path, _T( "wb+" ) );

		if ( NULL == m_fpOut )
		{
			
			return -1;
		}


		// Vorbis streams begin with three headers; the initial header (with
		// most of the codec setup parameters) which is mandated by the Ogg
		// bitstream spec.  The second header holds any comment fields.  The
		// third header holds the bitstream codebook.  We merely need to
		// make the headers, then pass them to libvorbis one at a time;
		// libvorbis handles the additional Ogg bitstream constraints */
		if (bReturn  = TRUE)
		{

			ogg_packet header;
			ogg_packet header_comm;
			ogg_packet header_code;

			// build packets
			vorbis_analysis_headerout_func( &m_vd,&m_vc,&header,&header_comm,&header_code );

			// stream them out
			ogg_stream_packetin_func(&m_os,&header ); 
			ogg_stream_packetin_func( &m_os, &header_comm );
			ogg_stream_packetin_func( &m_os, &header_code );

			// We don't have to write out here, but doing so makes streaming 
			// much easier, so we do, flushing ALL pages. This ensures the actual
			// audio data will start on a new page

			while( !m_eos && (bReturn = TRUE) )
			{
				int result = ogg_stream_flush_func( &m_os, &m_og );

				if( 0 == result )
					break;

				bReturn = WriteVorbisFrame();
			}
		}


		m_dwInBufferSize = 1024 * pInputFormat->nChannels;

		if ( m_bDownMixToMono )
		{
			m_dwInBufferSize *= 2;
		}
	}

	// Set output buffer size, no output buffer needed, write directly to file
	m_dwOutBufferSize = 0;

	if (bReturn == TRUE)
	{
		// Initialize input stream

		// allocate input stream
		m_psInputStream = new SHORT[ m_dwInBufferSize ];

		if ( m_bUpMixToStereo )
		{
			m_dwInBufferSize /= 2;
		}
		if ( NULL == m_psInputStream )
		{
			bReturn = FALSE;
		}
	}

	if(bReturn == FALSE){

		return -1;
	}
	
	return 0;
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

	BOOL bReturn = TRUE;
	DWORD nBytesToWrite=0;
	DWORD dwNumSamples = items;

	TRACE( _T( "Entering CEncoderVorbisDll::EncodeChunk(), dwNumSamples = %d \n" ),
					dwNumSamples );

	// Check the size of the input buffer
	ASSERT( dwNumSamples <= m_dwInBufferSize );


	// end of stream?
	if ( m_eos )
	{
		bReturn = FALSE;
	}
	else
	{
		dwNumSamples = ProcessData( ptr, dwNumSamples );
	
		FLOAT**	pfInputbuffer = NULL;
		DWORD	dwSample = 0;
		PSHORT	pSamples = m_psInputStream;

		// expose the buffer to submit data
		pfInputbuffer = vorbis_analysis_buffer_func( &m_vd, dwNumSamples/ (m_nMode == BE_MP3_MODE_MONO ? 1 : 2) ); 

		// get samples
		// uninterleave and copy the samples
		for( dwSample = 0; dwSample < dwNumSamples / (m_nMode == BE_MP3_MODE_MONO ? 1 : 2); dwSample++ )
		{
			pfInputbuffer[0][dwSample] = (float)(*pSamples++)/32768.0f;

			if ( 2 == (m_nMode == BE_MP3_MODE_MONO ? 1 : 2) )
			{
				pfInputbuffer[1][dwSample] = (float)(*pSamples++)/32768.0f;
			}
		}

		// tell the library how much we actually submitted
		vorbis_analysis_wrote_func( &m_vd, dwNumSamples / (m_nMode == BE_MP3_MODE_MONO ? 1 : 2) );

		TRACE( _T( "Fed Encoder %d samples\n" ),
				dwNumSamples );


		// vorbis does some data preanalysis, then divvies up blocks for
		// more involved (potentially parallel) processing.  Get a single
		// block for encoding now
		while( 1 == vorbis_analysis_blockout_func( &m_vd, &m_vb ) )
		{
			// analysis
			vorbis_analysis_func( &m_vb, NULL );

			vorbis_bitrate_addblock_func( &m_vb);

			// EXTRA LOOP ADDED 20011225 FOR RC3 BITRATE MANAGEMENT
			while( vorbis_bitrate_flushpacket_func( &m_vd, &m_op ) )
			{
				// weld the packet into the bitstream
				ogg_stream_packetin_func( &m_os, &m_op );
    
				// write out pages (if any)
				do
				{
					int nResult = 0;

					nResult = ogg_stream_pageout_func( &m_os, &m_og );
					if ( 0 == nResult ) 
					{
						break;
					}
		
					// write the data
					bReturn = WriteVorbisFrame();

					if( ogg_page_eos_func( &m_og ) )
					{
						m_eos = true;;
					}

				} while( bReturn == TRUE );
			}
		}
	}

	TRACE( _T( "Leaving write, return status %d" ),
					bReturn );

	return items;
}

////////////////////////////////////////////////////////////////////////
// void Close(...) - Close File after reading.
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void  close(void){



	// get the last samples out of the encoder
	// feed the encoder with zero samples, otherwhise we don't get 
	// the last frame
	write( NULL, 0 );


   /* clean up and exit.  vorbis_info_clear() must be called last */
	ogg_stream_clear_func( &m_os );
	vorbis_block_clear_func( &m_vb );
	vorbis_dsp_clear_func( &m_vd );
	vorbis_comment_clear_func( &m_vc );
	vorbis_info_clear_func( &m_vi );

	if ( m_fpOut )
	{
		fclose( m_fpOut );
		m_fpOut = NULL;
	}


	if ( m_hLibVorbisDLL )
	{
		::FreeLibrary( m_hLibVorbisDLL );
		m_hLibVorbisDLL = NULL;
	}
	if ( m_hVorbisEncDLL )
	{
		::FreeLibrary( m_hVorbisEncDLL );
		m_hVorbisEncDLL = NULL;
	}
	if ( m_hOggDLL )
	{
		::FreeLibrary( m_hOggDLL );
		m_hOggDLL = NULL;
	}
	TRACE( _T( "Leaving close" ) );

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

/*
** Copyright (C) 2002 - 2003 Thees Winkler
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
#include "ConfigDlg.h"
#include "AboutDlg.h"
#include "../out_plugin.h"
#include <vorbis/vorbisenc.h>

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

void   init();
int    quit(void);
int    open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo);
DWORD  write(short *ptr, DWORD items);
__int64 get_est_size(__int64 nSizeInBytes, DWORD dwNumChannels, DWORD dwSamplerate, DWORD dwBitrate);
int    close(void);
void   configure();
void   about();



// LF_OUT Structure of this Plugin
LF_OUT lameFEOutModule = {

	VERSION,
	45678,
	"Ogg/Vorbis Encoder",
	NULL,	 // MainWindow Filled in by lameFE
	NULL, 	 //  HINSTANCE  Filled in by lameFE
	".OGG",   
	init,		
	quit,		
	open,	
	get_est_size,
	write,   
	close,
	configure,
	about,
	NULL,
	NULL
};

// File information
DWORD g_dwSamplerate	= -1;
int   g_nBitrate		= -1;
int   g_nNumChannels	= -1;

// Encoder settings
int g_nMode			= -1;
int g_nMinBitrate	= -1;
float g_nNomBitrate	= -1;
int g_nMaxBitrate	= -1;
int g_nQuality		= -1;

#define READ_SIZE	1024

ogg_stream_state os; /* take physical pages, weld into a logical
			  stream of packets */
ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
ogg_packet       op; /* one raw packet of data for decode */
 
vorbis_info      vi; /* struct that stores all the static vorbis bitstream
			  settings */
vorbis_comment   vc; /* struct that stores all the user comments */

vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
vorbis_block     vb; /* local working space for packet->PCM decode */

int eos	= 0;

FILE*	g_pFileOut		= NULL;



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
int quit(void){

	return 0;
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
// int Open(...) - Open file for write. 
//     Following return values if an error occurs:
//     -1 : File could not be created
//     -2 : Invalid file format
//     -3 : Other error
//     On success this function returns the number off samples (items) to pass
//     to write [some encoders expect a certain number of samples]
//     
//
// Parameters:
//  const char * path
//      The path of the file to open
// MMFILE_FORMAT * pInputFormat
//      Pointer to a MMFILE_FORMAT structure containing the format of 
//		the input file
// MMFILE_ALBUMINFO * pAlbumInfo
//      Pointer to a MMFILE_FORMAT structure containing the album infos for
//      this tracks. Some fileformats support ID3 Tags or something similar.
//      So you might need it.
////////////////////////////////////////////////////////////////////////
int open(const char* path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo){
	
	g_dwSamplerate = pInputFormat->dwSampleRate;
	g_nBitrate     = pInputFormat->nBitsPerSample;
	g_nNumChannels = pInputFormat->nChannels;

	if(g_nBitrate != 16){

		//AfxMessageBox("Invalid Samplerate or Bitrate. Ogg/Vobis supports only 44.100 Hz with 16 bit!", MB_OK+MB_ICONSTOP);
		TRACE("Invalid bitrate\n");
		return -2;
	}

	// Get Settings from inifile
	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("OGG_out.dll_nMinBitrate", buffer, sizeof(buffer), FALSE);
	g_nMinBitrate	= getformatedvalue(atoi(buffer));
	lameFEOutModule.GetProfileString("OGG_out.dll_nNomBitrate", buffer, sizeof(buffer), FALSE);
	g_nNomBitrate	= getformatedvalue(atoi(buffer));
	lameFEOutModule.GetProfileString("OGG_out.dll_nMaxBitrate", buffer, sizeof(buffer), FALSE);
	g_nMaxBitrate	= getformatedvalue(atoi(buffer));
	lameFEOutModule.GetProfileString("Ogg_out.dll_nQuality", buffer, sizeof(buffer), FALSE);
	g_nQuality = atoi(buffer);
	lameFEOutModule.GetProfileString("Ogg_out.dll_nMode", buffer, sizeof(buffer), FALSE);
	g_nMode = atoi(buffer);
	
	delete buffer;

	float fQuality = g_nQuality / 1000.f;


	/********** Encode setup ************/
	int nResult  = 1;

	vorbis_info_init(&vi);

	/* choose an encoding mode.  A few possibilities commented out, one
	 actually used: */

	switch(g_nMode){

	/* Encode using a qulity mode, but select that quality mode by asking for
	an approximate bitrate.  This is not ABR, it is true VBR, but selected
	using the bitrate interface, and then turning bitrate management off: */

	case 0:
		nResult = vorbis_encode_setup_vbr(&vi, g_nNumChannels, g_dwSamplerate,
			               fQuality);
		break;
	
	/*********************************************************************
	Encoding using a VBR mode */
	case 1:
		nResult = vorbis_encode_init(&vi, g_nNumChannels, g_dwSamplerate,
			               g_nMaxBitrate*1000, -1, g_nMinBitrate*1000);
		break;

	/*********************************************************************
	Encoding using an average bitrate mode (ABR).
	example: 44kHz stereo coupled, average 128kbps VBR */
	case 2:

		nResult = vorbis_encode_init(&vi,g_nNumChannels, g_dwSamplerate,
			               -1,g_nNomBitrate*1000,-1);
		break;

	/*********************************************************************
	Encoding using a CBR quality mode.  Emulated by using same bitrates for VBR
	*/
	case 3:
		nResult = vorbis_encode_init(&vi,g_nNumChannels, g_dwSamplerate,
			               g_nNomBitrate*1000, g_nNomBitrate*1000, g_nNomBitrate*1000);
		break;
	
	}

	/* do not continue if setup failed; this can happen if we ask for a
	 mode that libVorbis does not support (eg, too low a bitrate, etc,
	 will return 'OV_EIMPL') */

	if(nResult){

		TRACE("Ogg Encoder could not be initialized. nResult = %d\n", nResult);
		return -3;
	}
	vorbis_encode_setup_init(&vi);

	/* add a comment */
	vorbis_comment_init(&vc);
	vorbis_comment_add_tag(&vc,"ENCODER","OGG/Vorbis Encoder for LameFE");

	vorbis_comment_add_tag(&vc, "TITLE", (char*)pAlbumInfo->song);
	vorbis_comment_add_tag(&vc, "ARTIST",(char*)pAlbumInfo->artist);
	vorbis_comment_add_tag(&vc, "ALBUM", (char*)pAlbumInfo->album);
	//vorbis_comment_add_tag(&vc, "DATE", ); // have to work on that

	vorbis_comment_add_tag(&vc, "COMMENT", (char*)pAlbumInfo->comment); // should be DESCRIPTION?
	char * cTrack = new char[3];
	itoa(pAlbumInfo->track, cTrack, 10);
	vorbis_comment_add_tag(&vc, "TRACKNUMBER", cTrack);

    //vorbis_comment_add_tag(&vc,"GENRE",buffer);  // gotta work on that one


	/* set up the analysis state and auxiliary encoding storage */
	vorbis_analysis_init(&vd, &vi);
	vorbis_block_init(&vd,&vb);

	/* set up our packet->stream encoder */
	/* pick a random serial number; that way we can more likely build
	 chained streams just by concatenation */
	srand(time(NULL));
	ogg_stream_init(&os,rand());

	g_pFileOut = fopen(path, "wb+´");

	if(g_pFileOut == NULL){

		return -1;
	}


	/* Vorbis streams begin with three headers; the initial header (with
	  most of the codec setup parameters) which is mandated by the Ogg
	  bitstream spec.  The second header holds any comment fields.  The
	  third header holds the bitstream codebook.  We merely need to
	  make the headers, then pass them to libvorbis one at a time;
	  libvorbis handles the additional Ogg bitstream constraints */

	{
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;

		vorbis_analysis_headerout(&vd, &vc, &header, &header_comm, &header_code);
		/* automatically placed in its own page */
		ogg_stream_packetin(&os, &header); 

		ogg_stream_packetin(&os, &header_comm);
		ogg_stream_packetin(&os, &header_code);

	  /* We don't have to write out here, but doing so makes streaming 
		 much easier, so we do, flushing ALL pages. This ensures the actual
		 audio data will start on a new page */
		while(!eos){

			int result = ogg_stream_flush(&os, &og);
			
			if(result == 0){

				break;
			}

			fwrite(og.header, 1, og.header_len, g_pFileOut);
			fwrite(og.body, 1, og.body_len, g_pFileOut);
		}
	}
	
	
	return READ_SIZE;
}

__int64 get_est_size(__int64 nSizeInBytes, DWORD dwNumChannels, DWORD dwSamplerate, DWORD dwBitrate){

	return 2048;
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
DWORD  write(short *ptr, DWORD items){


	BOOL	bReturn		  = TRUE;
	FLOAT**	pfInputbuffer = NULL;
	DWORD	dwSample = 0;
	PSHORT	pSamples = ptr;

	// expose the buffer to submit data
	pfInputbuffer = vorbis_analysis_buffer(&vd, items / g_nNumChannels); 

	// get samples
	// uninterleave and copy the samples
	for(dwSample = 0; dwSample < items / g_nNumChannels; dwSample++)
	{
		pfInputbuffer[0][dwSample] = (float)(*pSamples++)/32768.0f;

		if(g_nNumChannels == 2)
		{
			pfInputbuffer[1][dwSample] = (float)(*pSamples++)/32768.0f;
		}
	}

	// tell the library how much we actually submitted
	vorbis_analysis_wrote(&vd, items / g_nNumChannels );

	// vorbis does some data preanalysis, then divvies up blocks for
	// more involved (potentially parallel) processing.  Get a single
	// block for encoding now
	while(vorbis_analysis_blockout(&vd, &vb) == 1){

		// analysis
		vorbis_analysis(&vb, NULL);

		vorbis_bitrate_addblock(&vb);

		// EXTRA LOOP ADDED 20011225 FOR RC3 BITRATE MANAGEMENT
		while(vorbis_bitrate_flushpacket(&vd, &op)){

			// weld the packet into the bitstream
			ogg_stream_packetin(&os, &op);

			// write out pages (if any)
			do{

				int nResult = 0;

				nResult = ogg_stream_pageout(&os, &og);
				if (nResult == 0){

					break;
				}
	
				// write the data
				//bReturn = WriteVorbisFrame();
				// write header to output file
				if(fwrite(og.header, og.header_len, 1, g_pFileOut) != 1){

					ASSERT(FALSE);
					bReturn = FALSE;
					items = -1;
					eos = true;
				}

				// write body to output file
				if (fwrite(og.body, og.body_len, 1, g_pFileOut) != 1){

					ASSERT(FALSE);
					bReturn = FALSE;
					items = -1;
					eos = true;
				}

				if(ogg_page_eos(&og)){

					eos = true;
				}

			} while(bReturn);
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
int close(void){


  /* clean up and exit.  vorbis_info_clear() must be called last */
  write(0,0);

  ogg_stream_clear(&os);
  vorbis_block_clear(&vb);
  vorbis_dsp_clear(&vd);
  vorbis_comment_clear(&vc);
  vorbis_info_clear(&vi);
  fclose(g_pFileOut);

  return 0;
}


////////////////////////////////////////////////////////////////////////
// void Configure(...) - Show Configuration Dialog for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void configure(){

	CConfigDlg dlg(NULL, &lameFEOutModule);

	char *buffer = new char[256];
	lameFEOutModule.GetProfileString("OGG_out.dll_nMinBitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nMinBitrate	= atoi(buffer);
	lameFEOutModule.GetProfileString("OGG_out.dll_nNomBitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nNomBitrate	= atoi(buffer);
	lameFEOutModule.GetProfileString("OGG_out.dll_nMaxBitrate", buffer, sizeof(buffer), FALSE);
	dlg.m_nMaxBitrate	= atoi(buffer);
	lameFEOutModule.GetProfileString("Ogg_out.dll_nQuality", buffer, sizeof(buffer), FALSE);
	dlg.m_nQuality = atoi(buffer);
	lameFEOutModule.GetProfileString("Ogg_out.dll_nMode", buffer, sizeof(buffer), FALSE);
	dlg.m_nMode = atoi(buffer);

	int nResult = dlg.DoModal();

	if(nResult == IDOK){

		itoa(dlg.m_nMinBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_nMinBitrate", buffer);
		itoa(dlg.m_nNomBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_nNomBitrate", buffer);
		itoa(dlg.m_nMaxBitrate, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_nMaxBitrate", buffer);
		itoa(dlg.m_nQuality, buffer, 10);
		lameFEOutModule.SetProfileString("OGG_out.dll_nQuality", buffer);
		itoa(dlg.m_nMode, buffer, 10);
		lameFEOutModule.SetProfileString("Ogg_out.dll_nMode", buffer);
	}

	delete buffer;
	buffer = 0;
}

////////////////////////////////////////////////////////////////////////
// void About(...) - Show Aboutbox for this plugin
//
// Parameters:
//  none
////////////////////////////////////////////////////////////////////////
void about(){

	CAboutDlg dlg;
	dlg.DoModal();
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

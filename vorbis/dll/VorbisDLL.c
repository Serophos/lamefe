/*
 *	DLL Interface for Ogg Vorbis.
 *
 *	Copyright (c) 2000 - 2001 A.L. Faber
 *
 *  Work is partly derived from:
 *		THE OggSQUISH SOURCE CODE IS (C) COPYRIGHT 1994-2000
 *		by Monty <monty@xiph.org> and The XIPHOPHORUS Company
 *		http://www.xiph.org/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */

#include "VorbisDLL.h"
#include <assert.h>
#include "vorbis/vorbisfile.h"
#include "vorbis/vorbisenc.h"
#include "utf8.h"

 
#ifdef _DEBUG
//	#define _DEBUGDLL 1
#endif

#define _DEBUGDLL 1

#ifdef _DEBUGDLL
	CHAR cbuffer[255];
	#define TRACE(msg)	OutputDebugString(msg)
#else
#endif


const int MAJORVERSION= 1;
const int MINORVERSION= 7;

static const int VB_OUT_BUFFER_SIZE=32000;
static DWORD dwSampleBufferSize=0;

#ifdef _DEBUGDLL
void dump_config( char *inPath, char *outPath);
#endif


typedef struct VB_STREAM_TAG
{
	ogg_sync_state		oy; // sync and verify incoming physical bitstream 
	ogg_stream_state	os; // take physical pages, weld into a logical stream of packets 
	ogg_page			og; // one Ogg bitstream page.  Vorbis packets are inside 
	ogg_packet			op; // one raw packet of data for decode 
	vorbis_info			vi; // struct that stores all the static vorbis bitstream settings 
	vorbis_comment		vc; // struct that stores all the bitstream user comments 
	vorbis_dsp_state	vd; // central working state for the packet->PCM decoder 
	vorbis_block		vb; // local working space for packet->PCM decode 
	OggVorbis_File		vf; // input file handle
	PVB_ALBUM_INFO		pvbAlbumInfo;
	BOOL				bFirstFrame;
} VB_STREAM,* PVB_STREAM;


// Currently only one stream is supported (stream zero for input, stream 1 for output)
VB_STREAM vbStreams[2];


//BYTE	sample_buffer[4096];


static void DispErr(LPSTR strErr);
static void InitParams(HVB_STREAM hStream);



static void InitParams(HVB_STREAM hStream)
{
	// Clear all the streams info
	memset(&vbStreams[hStream],0x00,sizeof(vbStreams[hStream]));
}




// ***************** VORBIS GENERIC EXPORT ROUTINES **************************


BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:break;
		case DLL_THREAD_ATTACH:break;
		case DLL_THREAD_DETACH:break;
		case DLL_PROCESS_DETACH:break;
    }
    return TRUE;
}



__declspec(dllexport) VOID vbVersion
	(
		PVB_VERSION pvbVersion
	)
{
	// DLL Release date
	char lpszDate[20] = {'\0',};
	char lpszTemp[5] = {'\0',};


	// Set DLL interface version
	pvbVersion->byDLLMajorVersion = MAJORVERSION;
	pvbVersion->byDLLMinorVersion = MINORVERSION;

	// Set Engine version number (Same as Lame version)
	pvbVersion->byMajorVersion = 1;
	pvbVersion->byMinorVersion = 4;

	// Get compilation date
	strcpy( lpszDate,__DATE__ );

	// Get the first three character, which is the month
	strncpy( lpszTemp, lpszDate, 3 );
	pvbVersion->byMonth=1;

	// Set month
	if (strcmp(lpszTemp,"Jan")==0)	pvbVersion->byMonth=1;
	if (strcmp(lpszTemp,"Feb")==0)	pvbVersion->byMonth=2;
	if (strcmp(lpszTemp,"Mar")==0)	pvbVersion->byMonth=3;
	if (strcmp(lpszTemp,"Apr")==0)	pvbVersion->byMonth=4;
	if (strcmp(lpszTemp,"May")==0)	pvbVersion->byMonth=5;
	if (strcmp(lpszTemp,"Jun")==0)	pvbVersion->byMonth=6;
	if (strcmp(lpszTemp,"Jul")==0)	pvbVersion->byMonth=7;
	if (strcmp(lpszTemp,"Aug")==0)	pvbVersion->byMonth=8;
	if (strcmp(lpszTemp,"Sep")==0)	pvbVersion->byMonth=9;
	if (strcmp(lpszTemp,"Oct")==0)	pvbVersion->byMonth=10;
	if (strcmp(lpszTemp,"Nov")==0)	pvbVersion->byMonth=11;
	if (strcmp(lpszTemp,"Dec")==0)	pvbVersion->byMonth=12;

	// Get day of month string (char [4..5])
	pvbVersion->byDay=atoi( lpszDate + 4 );

	// Get year of compilation date (char [7..10])
	pvbVersion->wYear=atoi( lpszDate + 7 );

	memset( pvbVersion->zHomepage, 0x00, VB_MAX_HOMEPAGE );

	strcpy( pvbVersion->zHomepage, "http://www.xiph.org/ogg/vorbis/" );
}

// ***************** VORBIS ENCODER EXPORT ROUTINES **************************


static void AddUtfTag( vorbis_comment *vc, LPCSTR lpszField, LPCSTR lpszFieldValue );
static void AddUtfTag( vorbis_comment *vc, LPCSTR lpszField, LPCSTR lpszFieldValue )
{
	char*			lpszValue = NULL;
	wchar_t*		lpwszValue = NULL;
	unsigned int	nLen = 0;
	unsigned int	i =0;

	// first convert to wide character 
	lpwszValue = calloc( strlen( lpszFieldValue ) + 1 , sizeof( wchar_t ) );

	// convert to wide string
	for ( i = 0; i < strlen( lpszFieldValue ) + 1; i++ )
	{
		lpwszValue[i] = lpszFieldValue[i];
	}

	// get the length
	nLen = WideCharToUTF8( NULL, lpwszValue );

	// allocate string
	lpszValue = calloc( nLen, sizeof( char ) );

	// do conversion to UTF string
	WideCharToUTF8( lpszValue, lpwszValue );

	// add comment
	vorbis_comment_add_tag( vc, (LPSTR)lpszField, lpszValue );

	// cleanup
	free( lpwszValue );
	free( lpszValue );
}

__declspec(dllexport) VB_ERR vbEncOpen
	(
		PVB_CONFIG		pvbConfig,
		PVB_ALBUM_INFO	pAlbumInfo,
		PDWORD			dwSamples,
		PDWORD			dwBufferSize,
		PHVB_STREAM		phvbStream
	)
{
	PVB_STREAM	pStream=NULL;
	BOOL		bComment=FALSE;

	// only one stream right now, stream zero is used for endcoding
	*phvbStream=0;

	// clear all the stuff (FIXME FOR MULTIPLE CHANNELS)
	InitParams(*phvbStream);

	// Easier to work with a pointer to the current stream
	pStream=&vbStreams[*phvbStream];

	/* set default parameters */
	if ( pvbConfig->format.VBV1.lMaxBitRate <= 0 ) 
		pvbConfig->format.VBV1.lMaxBitRate = -1;
	if ( pvbConfig->format.VBV1.lMinBitRate <= 0 ) 
		pvbConfig->format.VBV1.lMinBitRate = -1;
	if ( pvbConfig->format.VBV1.lNomBitRate <= 0 ) 
		pvbConfig->format.VBV1.lNomBitRate = 160000;

	/* init the vorbis stream */
	vorbis_info_init(&pStream->vi);

	/* setup the encoder parameters */
	vorbis_encode_init(	&pStream->vi,
						pvbConfig->format.VBV1.dwChannels,
						pvbConfig->format.VBV1.dwSampleRate,
						pvbConfig->format.VBV1.lMaxBitRate,
						pvbConfig->format.VBV1.lNomBitRate,
						pvbConfig->format.VBV1.lMinBitRate);


	// add a comment
	vorbis_comment_init(&pStream->vc);

	if (pAlbumInfo)
	{
		DWORD i;
		PVB_ALBUM_INFO_RECORD pRecord=&(pAlbumInfo->infoRecord);

		for (i=0;i<pAlbumInfo->dwRecords;i++)
		{
			if (pRecord->strComment)
			{
				bComment=TRUE;
				vorbis_comment_add(&pStream->vc,(LPSTR)pRecord->strComment);
			}
			else
			{
				vorbis_comment_add(&pStream->vc,"Track encoded using Vorbis.dll interface.");
			}

			if ( pRecord->strTitle )
				AddUtfTag(&pStream->vc, "TITLE", pRecord->strTitle );
			if ( pRecord->strArtist )
				AddUtfTag(&pStream->vc, "ARTIST", pRecord->strArtist );
			if ( pRecord->strAlbum )
				AddUtfTag(&pStream->vc, "ALBUM", pRecord->strAlbum );

			if ( pRecord->dwTrackNumber > 0 )
			{
				char lpszTemp[10] = { '\0',};
				sprintf( lpszTemp, "%02d", pRecord->dwTrackNumber );
				AddUtfTag( &pStream->vc, "TRACKNUMBER", lpszTemp );		
			}
			if ( pRecord->dwVersion > 0 && pRecord->strEncodedBy ) 
			{
				AddUtfTag(&pStream->vc, "ENCODEDBY", pRecord->strEncodedBy );
			}
			if ( pRecord->dwVersion > 0 && pRecord->strGenre ) 
			{
				AddUtfTag(&pStream->vc, "GENRE", pRecord->strGenre );
			}
			if ( pRecord->dwVersion > 0 && pRecord->strDateTime ) 
			{
				AddUtfTag(&pStream->vc, "DATE", pRecord->strDateTime );
			}

			// GENRE
			// skip to next record
			pRecord+= pRecord->dwSize;
		}
	}

	if ( FALSE == bComment )
	{
		vorbis_comment_add( &pStream->vc, "Track encoded using Vorbis.dll interface." );
	}

	// set up the analysis state and auxiliary encoding storage
	vorbis_analysis_init(&pStream->vd, &pStream->vi );
	vorbis_block_init( &pStream->vd, &pStream->vb );



	// set up our packet->stream encoder
	// pick a random serial number; that way we can more likely build
	// chained streams just by concatenation
	srand( GetTickCount() );
	ogg_stream_init( &pStream->os,rand() );


	// Vorbis streams begin with three headers; the initial header (with
    // most of the codec setup parameters) which is mandated by the Ogg
    // bitstream spec.  The second header holds any comment fields.  The
    // third header holds the bitstream codebook.  We merely need to
    // make the headers, then pass them to libvorbis one at a time;
    // libvorbis handles the additional Ogg bitstream constraints */

	{
		ogg_packet hd;
		ogg_packet hd_comm;
		ogg_packet hd_code;

		vorbis_analysis_headerout(&pStream->vd,&pStream->vc,&hd,&hd_comm,&hd_code);
		ogg_stream_packetin(&pStream->os,&hd); // automatically placed in its own page */
		ogg_stream_packetin(&pStream->os,&hd_comm);
		ogg_stream_packetin(&pStream->os,&hd_code);

		// no need to write out here.  We'll get to that in the main loop
		pStream->bFirstFrame = TRUE;
	}


	//LAME encoding call will accept any number of samples.  
	*dwSamples=1152*pStream->vi.channels;

	// Set the input sample buffer size, so we know what we can expect
	dwSampleBufferSize = *dwSamples;

	// Set buffer size
	*dwBufferSize = VB_OUT_BUFFER_SIZE;


	// Everything went OK, thus return SUCCESSFUL
	return VB_ERR_OK;
}


__declspec(dllexport) VB_ERR vbEncClose
(
	HVB_STREAM	hbeStream,
	PBYTE		pOutput,
	PDWORD		pdwOutput
)
{
	int eos=0,bytes = 0;

	PVB_STREAM pStream = NULL;

	// Easier to work with a pointer to the current stream
	pStream = &vbStreams[ hbeStream ];


	// initialize the number of samples to be written
	*pdwOutput=0;

	vorbis_analysis_wrote( &pStream->vd, 0 );

	while( 1 == vorbis_analysis_blockout( &pStream->vd, &pStream->vb ) )
	{
    
		// analysis
		vorbis_analysis( &pStream->vb,&pStream->op );

		// weld the packet into the bitstream
		ogg_stream_packetin( &pStream->os,&pStream->op );

		/* write out pages (if any) */
		while(!eos)
		{
			int result = ogg_stream_pageout( &pStream->os, &pStream->og );
			if(result==0)break;

			// check if mp3buffer is big enough for the output
			bytes += pStream->og.header_len + pStream->og.body_len;

			// FIXME
			if ( bytes > VB_OUT_BUFFER_SIZE )
			{
				DispErr("BufferOverflow");
				return -5;
			}
	
			memcpy(pOutput,pStream->og.header,pStream->og.header_len);
			memcpy(pOutput+pStream->og.header_len,pStream->og.body,pStream->og.body_len);

			// this could be set above, but for illustrative purposes, I do
			// it here (to show that vorbis does know where the stream ends)
			if(ogg_page_eos(&pStream->og))eos=1;
      }
    }


	// clean up and exit.  vorbis_info_clear() must be called last
	ogg_stream_clear( &pStream->os );
	vorbis_block_clear( &pStream->vb );
	vorbis_dsp_clear( &pStream->vd );

  
	// ogg_page and ogg_packet structs always point to storage in
	// libvorbis.  They're never freed or manipulated directly
	*pdwOutput  =bytes;

	return VB_ERR_OK;
}


__declspec(dllexport) VB_ERR vbEncWrite
	(	
		HVB_STREAM	hbeStream, 
		DWORD		nSamples, 
		PSHORT		pSamples, 
		PBYTE		pOutput, 
		PDWORD		pdwOutput
	)
{
	//  lame_internal_flags *gfc=gfp->internal_flags;
	DWORD i=0;
	int eos=0,bytes=0;
	float **buffer=NULL;

  
	// pointer to the current stream
	PVB_STREAM pStream = &vbStreams[ hbeStream ];

	int nChannels = pStream->vi.channels;

	// initialize the number of samples to be written
	*pdwOutput = 0;

	/* We don't have to write out here, but doing so makes streaming 
	 * much easier, so we do, flushing ALL pages. This ensures the actual
	 * audio data will start on a new page
	 */
	if ( pStream->bFirstFrame )
	{
		while ( TRUE  )
		{
			int result = ogg_stream_flush( &pStream->os,&pStream->og );

			if(result==0) break;

			memcpy( pOutput, pStream->og.header, pStream->og.header_len );
			pOutput += pStream->og.header_len;
			bytes+=pStream->og.header_len;
			memcpy( pOutput, pStream->og.body, pStream->og.body_len );
			pOutput += pStream->og.body_len;
			bytes+=pStream->og.body_len;
		}
	}

	pStream->bFirstFrame = FALSE;



	// expose the buffer to submit data
	buffer=vorbis_analysis_buffer( &pStream->vd, nSamples/nChannels ); 

	// uninterleave and copy the samples
	for( i=0; i<nSamples/nChannels; i++ )
	{
		buffer[0][i]=(float)(*pSamples++)/32768.0f;
		if (nChannels==2)
		{
			buffer[1][i]=(float)(*pSamples++)/32768.0f;
		}
	}
  
	// tell the library how much we actually submitted
	vorbis_analysis_wrote( &pStream->vd, nSamples / nChannels );

	// vorbis does some data preanalysis, then divvies up blocks for
	// more involved (potentially parallel) processing.  Get a single
	// block for encoding now
	while( 1 == vorbis_analysis_blockout( &pStream->vd, &pStream->vb ) )
	{
		int result;
		// analysis
		vorbis_analysis( &pStream->vb, &pStream->op );
    
		// weld the packet into the bitstream
		ogg_stream_packetin( &pStream->os, &pStream->op );
    
		// write out pages (if any)
		do
		{
			result=ogg_stream_pageout( &pStream->os, &pStream->og );
			if (result==0) 
				break;
	
			// check if mp3buffer is big enough for the output
			bytes += pStream->og.header_len + pStream->og.body_len;
			
			/*
			DEBUGF("\n\n*********\ndecoded bytes=%i  %i \n",bytes,mp3buf_size);
			*/

			// FIXME
			if ( bytes > VB_OUT_BUFFER_SIZE )
			{
				DispErr("BufferOverflow");
				return -6;
			}
      
			// COPY THE DATA
			memcpy( pOutput, pStream->og.header, pStream->og.header_len );
			pOutput += pStream->og.header_len;
			memcpy( pOutput, pStream->og.body, pStream->og.body_len );
			pOutput += pStream->og.body_len;
      
			if( ogg_page_eos( &pStream->og ) )
			{
				eos=1;
			}
		} while ( 1 );
	}

	*pdwOutput=bytes;

	return VB_ERR_OK;
}




static void DispErr(LPSTR strErr)
{
	MessageBox(NULL,strErr,"",MB_OK);
}



#ifdef _DEBUGDLL
void dump_config( char *inPath, char *outPath)
{
}
#endif


// ***************** VORBIS ENCODER EXPORT ROUTINES **************************



// vorbis helper functions
size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	HANDLE file = (HANDLE)datasource;
	unsigned long bytesread;

	if(ReadFile(file, ptr, (unsigned long)(size*nmemb), &bytesread, NULL))
	{
		return bytesread/size;
	}
	else
		return 0; /* It failed */
}

int seek_func( void *datasource, ogg_int64_t offset, int whence )
{ /* Note that we still need stdio.h even though we don't use stdio, 
   * in order to get appropriate definitions for SEEK_SET, etc.
   */
	HANDLE file = (HANDLE)datasource;
	int seek_type;
	unsigned long retval;
	int seek_highword = (int)(offset>>32);

	switch(whence)
	{
		case SEEK_SET:
			seek_type = FILE_BEGIN;
			break;
		case SEEK_CUR:
			seek_type = FILE_CURRENT;
			break;
		case SEEK_END:
			seek_type = FILE_END;
			break;
	}

	/* On failure, SetFilePointer returns 0xFFFFFFFF, which is (int)-1 */
	
	retval=SetFilePointer(file, (int)(offset&0xffffffff), &seek_highword, seek_type);

	if(retval == 0xFFFFFFFF)
		return -1;
	else
		return 0; /* Exactly mimic stdio return values */
}

int close_func( void *datasource )
{
	HANDLE file = (HANDLE)datasource;

	return ( CloseHandle( file ) ? 0:EOF ); /* Return value meaning is inverted from fclose() */
}

long tell_func( void *datasource )
{
	HANDLE file = (HANDLE)datasource;

	return (long)SetFilePointer(file, 0, NULL, FILE_CURRENT); /* This returns the right number */
}


void GetAlbumInfo(PVB_STREAM pStream)
{
	vorbis_comment*	pComment=NULL;
	pComment = ov_comment(&pStream->vf, -1);

	// just to be sure
	pStream->pvbAlbumInfo=NULL;

	if(pComment)
	{

		// Alloc new record for album info
		pStream->pvbAlbumInfo=(PVB_ALBUM_INFO)malloc(sizeof(VB_ALBUM_INFO));

		pStream->pvbAlbumInfo->dwSize=sizeof(VB_ALBUM_INFO);
		pStream->pvbAlbumInfo->dwVersion=VB_ALB_STRUCT_VERSION;
		pStream->pvbAlbumInfo->dwRecords=1;

		pStream->pvbAlbumInfo->infoRecord.dwSize=sizeof(VB_ALBUM_INFO_RECORD);
		pStream->pvbAlbumInfo->infoRecord.dwVersion=VB_ALB_STRUCT_VERSION;

		pStream->pvbAlbumInfo->infoRecord.strArtist=vorbis_comment_query(pComment, "artist", 0);
		pStream->pvbAlbumInfo->infoRecord.strAlbum=vorbis_comment_query(pComment, "album", 0);
		pStream->pvbAlbumInfo->infoRecord.strTitle=vorbis_comment_query(pComment, "title", 0);
		pStream->pvbAlbumInfo->infoRecord.strComment=pComment->user_comments[0];
		pStream->pvbAlbumInfo->infoRecord.dwTrackNumber=1;
		pStream->pvbAlbumInfo->infoRecord.dwTotalTracks=1;
	}
}

__declspec(dllexport) VB_ERR	vbDecOpen
	(	
		PVB_DEC_INFO	pvbDecInfo,
		PVB_ALBUM_INFO*	ppAlbumInfo,
		LPCSTR			pszFileName
	)
{
	HANDLE			stream=NULL;
	vorbis_info*	vi = NULL;
	ov_callbacks	callbacks = {read_func, seek_func, close_func, tell_func};
	PVB_STREAM		pStream=NULL;

	if (pvbDecInfo==NULL)
		return VB_ERR_INVALID_FORMAT;


	// Clear the pvbDecInfo fields, except version and size info
	memset(&pvbDecInfo->hStream,0,sizeof(VB_DEC_INFO)-8);

	// currently only one stream supported
	pvbDecInfo->hStream=1;

	// Easier to work with a pointer to the current stream
	pStream=&vbStreams[pvbDecInfo->hStream];

	// Clear Album info at this point in time
	if (ppAlbumInfo)
	{
		*ppAlbumInfo=NULL;
	}

	// check if filename is valid
	if (pszFileName != NULL && pszFileName[0] != 0) 
	{

		stream = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(stream == INVALID_HANDLE_VALUE)
			return VB_ERR_OPEN_FILE;

		// The ov_open() function performs full stream detection and machine
		// initialization.  If it returns 0, the stream *is* Vorbis and we're
		// fully ready to decode.
		

		if (ov_open_callbacks(stream, &pStream->vf, NULL, 0, callbacks) < 0)
		{
			CloseHandle(stream);
			return VB_ERR_OPEN_FILE;
		}

		// Get album info
		GetAlbumInfo(pStream);		

		// Set album return pointer, if possible
		if (ppAlbumInfo)
		{
			*ppAlbumInfo=pStream->pvbAlbumInfo;
		}

	} 
	else 
	{
		// Could not open the input file, return error
		return VB_ERR_OPEN_FILE;
	}

	vi = ov_info(&pStream->vf, -1);

	// fill out the decoder info structure fields
	pvbDecInfo->dwTotTime_in_ms = (int)ov_time_total(&pStream->vf, -1) * 1000;
	pvbDecInfo->dwCurTime_in_ms	= 0;
	pvbDecInfo->dwSampleRate	= vi->rate;
	pvbDecInfo->dwChannels		= vi->channels;
	pvbDecInfo->nBitRate		= ov_bitrate(&pStream->vf, -1);
	pvbDecInfo->dwBufferSize	= 4096;

	// check if we can handle this amount of channels
	if(pvbDecInfo->dwChannels > 2 || pvbDecInfo->dwChannels < 1)
	{
		ov_clear(&pStream->vf);
		return VB_ERR_TOO_MANY_CHANNELS;
	}

	return VB_ERR_OK;
}


int current_section = -1;

__declspec(dllexport) VB_ERR vbDecRead
	(
		PVB_DEC_INFO	pvbDecInfo,
		PBYTE			pOutBuffer, 
		PDWORD			pdwBytesOut
	)
{
	PVB_STREAM pStream=NULL;

	if (pvbDecInfo==NULL)
		return VB_ERR_INVALID_FORMAT;

	// Easier to work with a pointer to the current stream
	pStream=&vbStreams[pvbDecInfo->hStream];

	*pdwBytesOut = ov_read(	&pStream->vf, 
							pOutBuffer,
							pvbDecInfo->dwBufferSize, 
							0, 2, 1, &current_section);

	// update position field
	if (pvbDecInfo->dwSampleRate)
	{
		pvbDecInfo->dwCurTime_in_ms+=*pdwBytesOut*1000/pvbDecInfo->dwSampleRate/sizeof(SHORT)/pvbDecInfo->dwChannels;
	}

	// update bitrate field
	pvbDecInfo->nBitRate = ov_bitrate_instant(&pStream->vf);

	if (*pdwBytesOut==0)
		return VB_ERR_EOS;

	return VB_ERR_OK;
}

__declspec(dllexport) VB_ERR vbDecSeek
	(
		PVB_DEC_INFO	pvbDecInfo,
		DWORD			dwSeekTime_in_ms
	)
{
	PVB_STREAM pStream=NULL;

	if (pvbDecInfo==NULL)
		return VB_ERR_INVALID_FORMAT;

	// Easier to work with a pointer to the current stream
	pStream=&vbStreams[pvbDecInfo->hStream];

	// update current time position
	pvbDecInfo->dwCurTime_in_ms = dwSeekTime_in_ms;

	// And seek to position
	ov_time_seek(&pStream->vf, dwSeekTime_in_ms / 1000);

	return VB_ERR_OK;

}

__declspec(dllexport) VB_ERR vbDecClose
	(	
		PVB_DEC_INFO	pvbDecInfo
	)
{
	PVB_STREAM pStream=NULL;

	if (pvbDecInfo==NULL)
		return VB_ERR_INVALID_FORMAT;

	// Easier to work with a pointer to the current stream
	pStream=&vbStreams[pvbDecInfo->hStream];

	// And close it
	ov_clear(&pStream->vf);

	// Free album info 
	if (pStream->pvbAlbumInfo!=NULL)
	{
		free(pStream->pvbAlbumInfo);
	}

	return VB_ERR_OK;
}



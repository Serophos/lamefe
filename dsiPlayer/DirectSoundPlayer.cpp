// DirectSoundPlayer.cpp: Implementierung der Klasse DirectSoundPlayer.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectSoundPlayer.h"

#define INITGUID
#include <dmusici.h>
#include <string>

IDirectMusicLoader8*  g_pLoader;
IDirectMusicPerformance8* g_pPerformance;
IDirectMusicSegment8*   g_pSegment;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////



DirectSoundPlayer::DirectSoundPlayer()
{

	g_pLoader		= NULL;
	g_pPerformance  = NULL;
	g_pSegment		= NULL;
}

DirectSoundPlayer::~DirectSoundPlayer()
{

}

BOOL DirectSoundPlayer::init()
{

	CoInitialize(NULL);
  
	CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
		CLSCTX_INPROC, IID_IDirectMusicLoader8,
		(void**)&g_pLoader);

	CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
		CLSCTX_INPROC, IID_IDirectMusicPerformance8,
        (void**)&g_pPerformance );

	g_pPerformance->InitAudio( 
		NULL,      // IDirectMusic interface not needed.
		NULL,      // IDirectSound interface not needed.
		NULL,      // Window handle.
		DMUS_APATH_SHARED_STEREOPLUSREVERB,  // Default audiopath type.
		1,        // Number of performance channels.
		DMUS_AUDIOF_ALL,   // Features on synthesizer.
		NULL       // Audio parameters; use defaults.
	  );

	return true;
	
}

void DirectSoundPlayer::play(LPCTSTR path, LPCSTR filename)
{

	// Find the Windows media directory.


	WCHAR wstrSearchPath[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, path, -1, 
		 wstrSearchPath, MAX_PATH );

	// Set the search directory.

	g_pLoader->SetSearchDirectory( 
		GUID_DirectMusicAllTypes, // Types of files sought.
		wstrSearchPath,     // Where to look.
		FALSE         // Don't clear object data.
	);

	WCHAR wstrFileName[MAX_PATH];// = L"giana.wav";
	MultiByteToWideChar( CP_ACP, 0, filename, -1,
			wstrFileName, MAX_PATH);

	if (FAILED(g_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment, // Class identifier.
		IID_IDirectMusicSegment8, // ID of desired interface.
		wstrFileName,     // Filename.
		(LPVOID*) &g_pSegment   // Pointer that receives interface.
		)))
	{
		MessageBox( NULL, "File not found.", 
			  "lameFE", MB_OK );
		return;
	}

	g_pSegment->Download( g_pPerformance );

	g_pPerformance->PlaySegmentEx(
				g_pSegment,  // Segment to play.
				NULL,    // Used for songs; not implemented.
				NULL,    // For transitions. 
				0,     // Flags.
				0,     // Start time; 0 is immediate.
				NULL,    // Pointer that receives segment state.
				NULL,    // Object to stop.
				NULL   // Audiopath, if not default.
			);  
	
/*	while(isPlaying() && !abort){

		Sleep(50);
	}*/

}

BOOL DirectSoundPlayer::deInit()
{
	
	g_pPerformance->Stop(
		NULL, // Stop all segments.
		NULL, // Stop all segment states.
		0,  // Do it immediately.
		0   // Flags.
	);

	g_pPerformance->CloseDown();

	g_pLoader->Release(); 
	g_pPerformance->Release();
	g_pSegment->Release();

	CoUninitialize();

	return TRUE;  
}  



BOOL DirectSoundPlayer::isPlaying()
{

	return g_pPerformance->IsPlaying(g_pSegment, NULL);
}

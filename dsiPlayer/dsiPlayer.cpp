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
#include "dxutil.h"
#include "dsutil.h"
#include "dsiPlayer.h"

CSoundManager* g_pSoundManager = NULL;
CSound*        g_pSound = NULL;
BOOL           g_bBufferPaused;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

    return TRUE;
}

DLLEXPORT void CCONV DSIP_Init(HWND hWnd){

	HRESULT hr;

	g_pSoundManager = new CSoundManager();

    if( FAILED( hr = g_pSoundManager->Initialize( hWnd, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( NULL, "Error initializing DirectSound.", 
                            "dsiPlayer.dll", MB_OK | MB_ICONERROR );
        return;
    }

    g_bBufferPaused = FALSE;

}

DLLEXPORT BOOL CCONV DSIP_Open(char* strFileName){

    HRESULT hr;

	if( g_pSound )
    {
        g_pSound->Stop();
        g_pSound->Reset();
    }

    // Free any previous sound, and make a new one
    SAFE_DELETE( g_pSound );

    // Load the wave file into a DirectSound buffer
    if( FAILED( hr = g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL ) ) )
    {
        // Not a critical failure, so just update the status
        DXTRACE_ERR_NOMSGBOX( TEXT("Create"), hr );
        //SetDlgItemText( hDlg, IDC_FILENAME, TEXT("Could not create sound buffer.") );
        return FALSE;
    }

	
	return TRUE;
}


DLLEXPORT HRESULT CCONV DSIP_Play(){

	HRESULT hr;

	if( g_pSound->IsSoundPlaying() )
	{
		// To pause, just stop the buffer, but don't reset the position
		if( g_pSound ){

			g_pSound->Stop();
		}

		g_bBufferPaused = TRUE;
	}
	else{

		if( g_bBufferPaused )
		{
			if( FAILED( hr = g_pSound->Play( 0, 0L ) ) ){

				return DXTRACE_ERR( TEXT("Play"), hr );
			}

			g_bBufferPaused = FALSE;
		}
		else
		{

			if( FAILED( hr = g_pSound->Play( 0, 0L ) ) ){

				return DXTRACE_ERR( TEXT("Play"), hr );
			}	
		}
	}
	
	return S_OK;

}

// Deprecated
DLLEXPORT void CCONV DSIP_Pause(){
	
	DSIP_Play();
}

DLLEXPORT void CCONV DSIP_Stop(){

    if( g_pSound )
    {
        g_pSound->Stop();
        g_pSound->Reset();
    }
}

DLLEXPORT BOOL CCONV DSIP_IsAudioPlaying(){
	
	if(g_pSound){

		return g_pSound->IsSoundPlaying();
	}

	return false;
}
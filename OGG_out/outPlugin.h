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


#ifndef OUT_PLUGIN_H___INCLUDED
#define OUT_PLUGIN_H___INCLUDED

#include "../mmfile.h"

#define VERSION 0x001

typedef struct 
{

	// data membmers of the API
	int version;				// which api vertsion?
	char *description;			// description of plugin, with version string

	HWND hMainWindow;			// lameFE's main window (filled in by lameFE)
	HINSTANCE hDllInstance;		// DLL instance handle (filled in by lameFE)

	char *FileExtensions;		//".wav" //this is the fileextension the output file will have
								// multiple extions are not allow. leading "dot" required!!


	// initialisation functions

	////////////////////////////////////////////////////////////////////////
	// void Init(...) - will be called after loading the plugin and after
	// filling in the hMainWindow and hDllInstance datamembers
	//
	// Parameters:
	//  const char* inifilename
	//      The full path to the lameFE ini file (may be u need infos from it
	////////////////////////////////////////////////////////////////////////
	void (*Init)(const char* inifilenam);


	////////////////////////////////////////////////////////////////////////
	// void Quit(void) - will be called before unloading the plugin 
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Quit)(void);


	////////////////////////////////////////////////////////////////////////
	// int Open(...) - Open file for write. Returns -1 on fail and 0 on success
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
	int		(*Open)(const char *path, MMFILE_FORMAT *pInputFormat, MMFILE_ALBUMINFO *pAlbumInfo);	


	////////////////////////////////////////////////////////////////////////
	// size_t Write(...) - Reads from file and writes given number of items of
	//    raw wave data to the buffer. returns the number of items read.
	//
	// Parameters:
	//  short *ptr
	//      Pointer to the buffer with raw wave data to be written
	//  size_t items
	//      Items (samples) to write. Is integer product of the number
	//      of channels or an error will occur.
	////////////////////////////////////////////////////////////////////////
	size_t  (*Write)(short *ptr, size_t items); 


	////////////////////////////////////////////////////////////////////////
	// void Close(...) - Close File after reading.
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Close)(void);		


	////////////////////////////////////////////////////////////////////////
	// void Configure(...) - Show Configuration Dialog for this plugin
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void   (*Configure)(void);		// Configure Dialog


	////////////////////////////////////////////////////////////////////////
	// void About(...) - Show Aboutbox for this plugin
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void   (*About)(void);			// About Dialog

} LF_OUT, *PLF_OUT;


typedef LF_OUT*	(*GETOUTMODULE)(void);

#endif
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


#ifndef PLUGIN_API_H___INCLUDED
#define PLUGIN_API_H___INCLUDED


#include "mmfile.h"

#define VERSION 0x002

typedef struct 
{

	// data membmers of the API
	int version;				// which api vertsion?
	char *description;			// description of plugin, with version string

	HWND hMainWindow;			// lameFE's main window (filled in by lameFE)
	HINSTANCE hDllInstance;		// DLL instance handle (filled in by lameFE)

	char *FileExtensions;		//"wav;riff;aiff" for example


	// initialisation functions

	////////////////////////////////////////////////////////////////////////
	// void Init(void) - will be called after loading the plugin and after
	// filling in the hMainWindow and hDllInstance datamembers
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Init)(void);

	////////////////////////////////////////////////////////////////////////
	// void Quit(void) - will be called before unloading the plugin 
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Quit)(void);

	////////////////////////////////////////////////////////////////////////
	// int Open(...) - Open file for read. Returns -1 on fail and 0 on success
	//
	// Parameters:
	//  const char * path
	//      The path of the file to open
	////////////////////////////////////////////////////////////////////////
	int		(*Open)(const char *path);	

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
	size_t  (*Read)(short *ptr, size_t items); 

	////////////////////////////////////////////////////////////////////////
	// void Close(...) - Close File after reading.
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	void (*Close)(void);		

	////////////////////////////////////////////////////////////////////////
	// int GetFileSize(...) - Get Size of decompressed file (raw data size)
	//    in Kilobyte
	//
	// Parameters:
	//  none
	////////////////////////////////////////////////////////////////////////
	int		(*GetFileSize)(void);


	////////////////////////////////////////////////////////////////////////
	// void GetMMFileFormat(...) - Get the fileformat of the decompressed
	//   raw wave data (samplingrate, bitrate, channels)
	//
	// Parameters:
	//  PMMFILE_FORMAT pMMFileFormat * pMMFileFormat
	//      pointer to a MMFILE_FORMAT structure that will recieve the data
	////////////////////////////////////////////////////////////////////////
	void    (*GetMMFileFormat)(PMMFILE_FORMAT pMMFileFormat); 
															 
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

	////////////////////////////////////////////////////////////////////////
	// void GetProfileString(...) - Get An entry from the LameFE.ini
	//                              returns TRUE on success
	//                              Filled in by LameFE. Not implemented in the plugin
	// Parameters:
	//  const char * entry
	//     string that defines the entry to query
	//  char *value
	//     buffer that recieves the value of the entry
	//  int size
	//     size of the value-buffer
	//  int formated
	//     some values of the LameFE.ini are written in a special format.
	//     get the unformated or the formated value? (e.g. bitrates)
	/////////////////////////////////////////////////////////////////////////
	int (*GetProfileString)(const char* entry, char* value, int size, int formated);


	////////////////////////////////////////////////////////////////////////
	// void SetProfileString(...) - Set an entry to the LameFE.ini
	//                              returns TRUE on success
	//                              Filled in by LameFE. Not implemented in the plugin
	// Parameters:
	//  const char * entry
	//     string that defines the entry to set (e.g. "bitrate")
	//  char *value
	//     value of the entry (e.g. "192")   [written is bitrate=192 in that case]
	////////////////////////////////////////////////////////////////////////
	int (*SetProfileString)(const char* entry, const char* value);

} LF_IN, *PLF_IN;


typedef LF_IN*	(*GETMODULE)(void);

#endif
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


typedef struct
{

	DWORD dwSampleRate;
	int	  nChannels;
	int   nBitsPerSample;
} MMFILE_FORMAT, *PMMFILE_FORMAT;


#define VERSION 0x001

typedef struct 
{
	int version;				// which api vertsion?
	char *description;			// description of plugin, with version string

	HWND hMainWindow;			// lameFE's main window (filled in by lameFE)
	HINSTANCE hDllInstance;		// DLL instance handle (Also filled in by lameFE)

	char *FileExtensions;		//"wav;riff;aiff;"

	void (*Init)(void);			// called before using dll
	void (*Quit)(void);			// called after using dll


	//File accesss
	
	// Open: return zero on success, -1 on file-not-found,
	int		(*Open)(const char *path);	

	//Read: The file read items functions fill the 
	//array pointed to by ptr with the requested number 
	//of items. The items parameter must be an 
	//integer product of the number of channels or an error 
	//will occur. 
	size_t  (*Read)(short *ptr, size_t items); 

	//close file
	void (*Close)(void);		

	int		(*GetFileSize)(void);	// Get file size in KB
	void    (*GetMMFileFormat)(PMMFILE_FORMAT pMMFileFormat);  //Fill in the PMMFILE_FORMAT
															  //Structure for Open file
	//Dialogs
	void   (*Configure)(void);		// Configure Dialog
	void   (*About)(void);			// About Dialog

} LF_IN, *PLF_IN;


typedef LF_IN*	(*GETMODULE)(void);

#endif
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


#if !defined(AFX_WINAMPPLUGIN_H__7C2BB3E1_D509_4ADB_91F6_51899EA5F398__INCLUDED_)
#define AFX_WINAMPPLUGIN_H__7C2BB3E1_D509_4ADB_91F6_51899EA5F398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CPlugin.h"
#include "winamp/in2.h"

typedef In_Module*	(*WINAMPGETINMODULE2)( void );
typedef Out_Module*	(*WINAMPGETOUTMODULE)( void );

class CWinampPlugin : public CPlugin  
{
public:
	// Construction / Destruction
	CWinampPlugin(CString path2plugin);
	virtual ~CWinampPlugin(void);

	// 
	BOOL	Load(HWND hWnd);
	BOOL	Unload(void);
	void	SetOutputModule(Out_Module* outModule);
	void Init(void);

	// Get Info about this plugin
	BOOL	UsesOutput(void);
	void	About(HWND hWnd);
	CString GetSzFilter(void);
	CString GetInfoString(void);
	In_Module* GetModule();
	// Configure plugin
	void	Configure(HWND hWnd);

	// Find plugin that matches the given file-extension 
	// in the specified path (pluginPath) and write the file-path to "name"
	static BOOL		FindPlugin(CString fileExtension, CString &name, CString pluginPath);

private:
	WINAMPGETINMODULE2	GetInModule;  //Pointer to the GetInModule Function of the dll
	In_Module*			inModule;	  //Pointer to the In_Module structure of the dll

};

#endif // !defined(AFX_WINAMPPLUGIN_H__7C2BB3E1_D509_4ADB_91F6_51899EA5F398__INCLUDED_)

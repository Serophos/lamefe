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


#if !defined(AFX_CPLUGIN_H__DB5CD37A_3FF3_4022_B8A2_0DE44FE7B4CB__INCLUDED_)
#define AFX_CPLUGIN_H__DB5CD37A_3FF3_4022_B8A2_0DE44FE7B4CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "plugin.h"
#include <mmreg.h>
#include "Encoder.h"
//#include "WINAMP\in2.h"	// Hinzugefügt von der Klassenansicht


class CPlugin  
{
public:
	void Init(void);
	
	// Construction / Destruction
	CPlugin(CString path2plugin = "");
	virtual ~CPlugin(void);

	// 
	BOOL	Load(HWND hWnd = NULL);
	BOOL	Unload(void);

	// Get Info about this plugin
	CString GetExtension();
	void	About(HWND hWnd = NULL);
	CString GetSzFilter(void);
	CString GetInfoString(void);

	// Configure plugin
	void	Configure(HWND hWnd);
	// Find plugin that matches the given file-extension 
	// in the specified path (pluginPath) and write the file-path to "name"


protected:

	HINSTANCE			hDLL;		  //Handle of the plugins dll
	CString				m_strExt;	  //the file extion this module can handle
	CString				m_strFilename;//path and filename of this plugin	
};

#endif // !defined(AFX_CPLUGIN_H__DB5CD37A_3FF3_4022_B8A2_0DE44FE7B4CB__INCLUDED_)

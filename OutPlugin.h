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

#if !defined(AFX_OUTPLUGIN_H__241438AD_B3B6_4625_A493_CEEB764DBCC2__INCLUDED_)
#define AFX_OUTPLUGIN_H__241438AD_B3B6_4625_A493_CEEB764DBCC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "out_plugin.h"

class COutPlugin  
{
public:

	COutPlugin(CString path2plugin = "", CString wd = "");
	virtual ~COutPlugin();

	BOOL	Load(HWND hWnd = NULL);
	BOOL	Init(void);
	BOOL	Unload(void);
	// Get Info about this plugin
	void	About(HWND hWnd = NULL);
	
	CString GetExtension(void) const;
	CString GetSzFilter(void) const;
	CString GetInfoString(void) const;

	// Configure plugin
	void	Configure(HWND hWnd = NULL);
	PLF_OUT  GetOutModule(){ return outModule; }
	static int SetProfileString(const char* entry, const char *value);
	static int GetProfileString(const char* entry, char* value, int size, int formated);

protected:

	HINSTANCE	hDLL;		  //Handle of the plugins dll
	CString		m_strExt;	  //the file extion this module can handle
	CString		m_strFilename;//path and filename of this plugin	
	PLF_OUT		outModule;
	GETOUTMODULE GetModule;
	CString		m_strWd;

};

#endif // !defined(AFX_OUTPLUGIN_H__241438AD_B3B6_4625_A493_CEEB764DBCC2__INCLUDED_)

/*
** Copyright (C) 2002-2003 Thees Winkler
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

#if !defined(AFX_LAMEFEPLUGIN_H__7812463D_7B1F_4C01_85DE_94B3D10F37C9__INCLUDED_)
#define AFX_LAMEFEPLUGIN_H__7812463D_7B1F_4C01_85DE_94B3D10F37C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cplugin.h"
#include "plugin_api.h"

class CLameFEPlugin : public CPlugin  
{
public:
	static int SetProfileString(const char* entry, const char *value);
	static int GetProfileString(const char* entry, char* value, int size, int formated);
	static BOOL FindPlugin(CString fileExtension, CString &name, CString pluginPath);
	void About();
	void Configure();
	void Unload();
	CString GetInfoString();
	CString GetFileExtensions();
	BOOL Load();

	CLameFEPlugin(CString path);
	virtual ~CLameFEPlugin();

private:
	
	PLF_IN inModule;
	GETMODULE GetModule;
};

#endif // !defined(AFX_LAMEFEPLUGIN_H__7812463D_7B1F_4C01_85DE_94B3D10F37C9__INCLUDED_)



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
#include "stdafx.h"
#include "WinampPlugin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CWinampPlugin::CWinampPlugin(CString path2plugin)
	:CPlugin(path2plugin)
{

	GetInModule	  = NULL;
	inModule      = NULL;
		
}

CWinampPlugin::~CWinampPlugin()
{

}


BOOL CWinampPlugin::Load(HWND hWnd)
{

	TRACE("Loading %s\n", m_strFilename);
	hDLL = LoadLibrary(m_strFilename);

	if(!hDLL){

		TRACE("Failed loading library\n");
		
		return FALSE;
	}

	GetInModule = (WINAMPGETINMODULE2) GetProcAddress(hDLL, "winampGetInModule2");

	if(!GetInModule){

		TRACE("GetProcAddress(hDLL, \"winampGetOutModule\") failed\n");
		return FALSE;
	}
	
	inModule = GetInModule();
	
	if(!inModule){

		return FALSE;
	}

	inModule->hMainWindow	= hWnd;
	inModule->hDllInstance  = hDLL;
	
	inModule->Init();
	m_strExt = inModule->FileExtensions;
	m_strExt.MakeLower();
	return TRUE;
}

BOOL CWinampPlugin::Unload()
{

	BOOL bResult;
	if(hDLL){

		inModule->Quit();
		TRACE("Unloaded %s\n", m_strFilename);
		bResult = FreeLibrary(hDLL);
		hDLL = NULL;
	}

	return bResult;
}


CString CWinampPlugin::GetInfoString()
{
	
	CString strResult;
	strResult.Format("%s", inModule->description);
	return 	strResult;
}

CString CWinampPlugin::GetSzFilter()
{

	//"Wave Audio (*.wav)|*.wav|";
	CString strSzFilter;
	CString tmpSz;
	int start = 4;
	int end = 0;
	strSzFilter.Format("%s Audio File (*.%s)|*.%s|", m_strExt.Mid(0, 3), m_strExt.Mid(0, 3), m_strExt.Mid(0, 3));
	
	while((end = m_strExt.Find(";", start)) != -1){
		
		//end = m_strExt.Find(";", start);
		tmpSz.Format("%s (*.%s)|*.%s|", m_strExt.Mid(start, end - start), m_strExt.Mid(start, end - start),m_strExt.Mid(start, end - start));
		strSzFilter += tmpSz;
		tmpSz.Empty();
		start = end + 1;
	}

	return strSzFilter;
}


void CWinampPlugin::Configure(HWND hWnd)
{
	
	inModule->Config(hWnd);
}

void CWinampPlugin::About(HWND hWnd)
{

	inModule->About(hWnd);
}


void CWinampPlugin::SetOutputModule(Out_Module* outModule)
{

	inModule->outMod = outModule;
}


BOOL CWinampPlugin::FindPlugin(CString fileExtension, CString &name, CString pluginPath)
{

	CFileFind		finder;

	BOOL bResult = finder.FindFile(pluginPath + "\\in_*.dll");
	BOOL found   = FALSE;
	while(bResult){

		bResult = finder.FindNextFile();
		name  = finder.GetFilePath();
		CWinampPlugin plugin(name);
		plugin.Load(NULL);
		CString ext = plugin.GetExtension();
		ext.MakeLower();
		fileExtension.MakeLower();
		if(ext.Find(fileExtension) != -1){
			
			bResult = FALSE;
			found = TRUE;
			plugin.Unload();
		}
		plugin.Unload();
	}
		
	
	return found;
}

BOOL CWinampPlugin::UsesOutput()
{

	return inModule->UsesOutputPlug;
}

void CWinampPlugin::Init()
{

	inModule->Init();
}

In_Module* CWinampPlugin::GetModule()
{

	return inModule;
}





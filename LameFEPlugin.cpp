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
#include "LameFEPlugin.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CLameFEPlugin::CLameFEPlugin(CString _path)
	:CPlugin(_path)
{

	inModule	= NULL;
	hDLL		= NULL;
	GetModule	= NULL;
}

CLameFEPlugin::~CLameFEPlugin()
{

}

BOOL CLameFEPlugin::Load()
{

	hDLL = LoadLibrary(m_strFilename);
	if(!hDLL){

		TRACE("Failed Loading Library\n");
		return FALSE;
	}
	
	GetModule = (GETMODULE)GetProcAddress(hDLL, "GetLFINModule");
	if(!GetModule){

		TRACE("Failed getting proc address\n");
		return FALSE;
	}

	inModule = GetModule();
	if(!inModule){

		TRACE("Failed getting module\n");
		return FALSE;
	}
	
	inModule->hDllInstance = hDLL;
	inModule->hMainWindow  = NULL;
	inModule->GetProfileString = GetProfileString;
	inModule->SetProfileString = SetProfileString;
	if(inModule->version != 0x002){

		TRACE("Wrong Interface version. WARNING THIS PLUGIN CANNOT BE LOADED BY THIS VERSION OF LAMEFE\n");
		FreeLibrary(hDLL);
		return FALSE;
	}

	inModule->Init();
	m_strExt = inModule->FileExtensions;
	return TRUE;
}

CString CLameFEPlugin::GetFileExtensions()
{

	if(inModule){

		return inModule->FileExtensions;
	}
	
	return "";
}

CString CLameFEPlugin::GetInfoString()
{

	if(!inModule){

		return "";
	}

	CString strResult;
	strResult.Format("%s\nThis plugin supports following file extensions %s", inModule->description,  inModule->FileExtensions);
	return 	strResult;
}

void CLameFEPlugin::Unload()
{

	if(hDLL){

//		inModule->Quit();
		FreeLibrary(hDLL);
	}
}

void CLameFEPlugin::Configure()
{

	if(inModule){

		inModule->Configure();
	}
}

void CLameFEPlugin::About()
{
	
	if(inModule){

		inModule->About();
	}
}


BOOL CLameFEPlugin::FindPlugin(CString fileExtension, CString &name, CString pluginPath)
{

	CFileFind		finder;

	BOOL bResult = finder.FindFile(pluginPath + "\\*_in.dll");
	BOOL found   = FALSE;
	while(bResult){

		bResult = finder.FindNextFile();
		name  = finder.GetFilePath();
		CLameFEPlugin plugin(name);
		plugin.Load();
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

int CLameFEPlugin::GetProfileString(const char *entry, char *value, int size, int formated)
{

	TCHAR	szBuffer[_MAX_PATH]; 
	CString strTmp;
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	strTmp = szBuffer;
	strTmp = strTmp.Left(strTmp.ReverseFind('\\'));

	CIni cfg;
	cfg.SetIniFileName(strTmp + "\\LameFE.ini");
	
	strTmp = cfg.GetValue("Plugins", entry, "");
	strcpy(value, strTmp);
	//strTmp.ReleaseBuffer();
	return TRUE;
}

int CLameFEPlugin::SetProfileString(const char *entry, const char *value)
{

	TCHAR	szBuffer[_MAX_PATH]; 
	CString strTmp;
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	strTmp = szBuffer;
	strTmp = strTmp.Left(strTmp.ReverseFind('\\'));

	CIni cfg;
	cfg.SetIniFileName(strTmp + "\\LameFE.ini");
	cfg.SetValue("Plugins", entry, value);
	return TRUE;
}

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
#include "OutPlugin.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

COutPlugin::COutPlugin(CString path2plugin /* = "" */, CString wd /*= "" */)
{

	m_strFilename = path2plugin;
	m_strWd		= wd;
	outModule	= NULL;
	hDLL		= NULL;
	GetModule	= NULL;
}

COutPlugin::~COutPlugin()
{

}

BOOL COutPlugin::Load(HWND hWnd)
{

	hDLL = LoadLibrary(m_strFilename);
	if(!hDLL){

		TRACE("Failed Loading Library %s\n", m_strFilename);
		return FALSE;
	}
	
	GetModule = (GETOUTMODULE)GetProcAddress(hDLL, "GetLFOutModule");
	if(!GetModule){

		TRACE("Failed getting proc address\n");
		return FALSE;
	}

	outModule = GetModule();
	if(!outModule){

		TRACE("Failed getting module\n");
		return FALSE;
	}
	
	outModule->hDllInstance = hDLL;
	outModule->hMainWindow  = NULL;
	outModule->GetProfileString = GetProfileString;
	outModule->SetProfileString = SetProfileString;
	outModule->Init();
	m_strExt = outModule->FileExtensions;
	return TRUE;
}

BOOL COutPlugin::Init(void)
{

	return TRUE;
}

BOOL COutPlugin::Unload(void)
{

	if(outModule){

		outModule->Quit();
	}

	BOOL bResult = FreeLibrary(hDLL);
	hDLL = NULL;
	//delete hDLL;
	//delete outModule;

	return bResult;
}

// Get Info about this plugin
void COutPlugin::About(HWND hWnd)
{

	if(!outModule){

		return;
	}
	
	outModule->About();
}


CString COutPlugin::GetExtension(void) const
{
	
	return m_strExt;
}

CString COutPlugin::GetSzFilter(void) const
{
	return "";
}

CString COutPlugin::GetInfoString(void) const
{

	if(!outModule){

		return "";
	}

	return outModule->description;
}


// Configure plugin
void COutPlugin::Configure(HWND hWnd)
{

	if(!outModule){

		return;
	}

	outModule->Configure();
}

int COutPlugin::GetProfileString(const char *entry, char *value, int size, int formated)
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

	return TRUE;
}

int COutPlugin::SetProfileString(const char *entry, const char *value)
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

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
#include "CPlugin.h"
#include "Encoder.h"
#include "EncoderFeeder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


CPlugin::CPlugin(CString path2plugin)
{

	m_strFilename = path2plugin;
}

CPlugin::~CPlugin()
{

}


BOOL CPlugin::Load(HWND hWnd)
{

	return TRUE;
}

BOOL CPlugin::Unload()
{

	return TRUE;
}


CString CPlugin::GetInfoString()
{
	
	CString strResult;
	return 	strResult;
}

CString CPlugin::GetSzFilter()
{

	if(m_strExt.IsEmpty()){

		return "";
	}
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

CString CPlugin::GetExtension()
{

	CString strExt = m_strExt;
	TRACE(strExt + "\n");
	int start = 0;
	int end = 0;
	strExt.TrimRight();
	strExt.TrimLeft();
	strExt.Insert(0, "*.");
	while((end = strExt.Find(";", start)) != -1){
		
		//end = m_strExt.Find(";", start);
		if(end < strExt.GetLength()){

			strExt.Insert(end+1, "*.");
		}
		start = end + 3;
	}
	
	return strExt;
}

void CPlugin::Configure(HWND hWnd)
{
	
}

void CPlugin::About(HWND hWnd)
{

}


//DEL void CPlugin::SetOutputModule(Out_Module* outModule)
//DEL {
//DEL 
//DEL }


void CPlugin::Init()
{

	
}





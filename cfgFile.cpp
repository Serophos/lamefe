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
#include "cfgFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

cfgFile::cfgFile(CString dir)
{

	if(dir = ""){

		TCHAR	szBuffer[_MAX_PATH]; 
		VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
		
		wd = szBuffer;
		wd = wd.Left(wd.ReverseFind('\\'));
	}
	else{

		wd = dir;
	}
	CFileException e;
	CStdioFile file;
	CString tmp;
	TRY{
		file.Open(wd + "\\lameFE.ini", CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText,&e);
		while( file.GetPosition() != file.GetLength() ){
			file.ReadString(tmp);
			if (( tmp == "[setup]" ) || (tmp.Left(1) == "#") || (tmp.GetLength() == 0)){
				//do nothing ignore
			}
			else{
				cfg.Add(tmp);
			}
		}
		file.Close();
	}
	CATCH(CFileException, e){
		AfxMessageBox("Error opening/reading lameFE.ini",MB_OK+MB_ICONSTOP,0);
	}END_CATCH

}

cfgFile::~cfgFile()
{

}

BOOL cfgFile::SetValue(CString item, int value)
{

	CString result;

	result.Format("%s=%d", item, value);

	CString prefix = result.Left(result.Find("="));
	BOOL    found  = FALSE;
	for(int i = 0; i < cfg.GetSize(); i++){
		
		if(cfg[i].Left(cfg[i].Find("=")) == prefix){

			cfg.SetAt(i, result);
			found = TRUE;
			break;
		}
	}
	if(!found){

		cfg.Add(result);
	}	

	return writeToDisk();
}

BOOL cfgFile::SetStringValue(CString item, CString value)
{

	CString result;
	result.Format("%s=%s", item, value);

	CString prefix = result.Left(result.Find("="));
	BOOL    found  = FALSE;

	for(int i = 0; i < cfg.GetSize(); i++){
		
		if(cfg[i].Left(cfg[i].Find("=")) == prefix){

			cfg.SetAt(i, result);
			found = TRUE;
			break;
		}
	}
	if(!found){

		cfg.Add(result);
	}	

	return writeToDisk();
}

int cfgFile::GetValue(LPCTSTR item, BOOL formated)
{
	std::string result;
	
	if((item == "bitrate") || (item ==  "maxbitrate")){

		result = (formated ? FormatBps(findItem(item)) : findItem(item));
	}
	else{

		result = findItem(item);
	}

	return atoi(result.c_str());
}

CString cfgFile::GetStringValue(CString item)
{

	CString result;

	result = findItem(item).c_str();
	
	if(result == "-1"){

		result.Empty();
	}
	return result;
}


std::string cfgFile::FormatBps(std::string nBps)
{
/*
 8 bps 0
16 bps 1
24 bps 2
32 bps 3
40 bps 4
48 bps 5
56 bps 6
64 bps 7 //
80 bps  8 
96  bps 9
112 bps 10
128 bps 11
144 bps 12
160 bps 13 //
192 bps 14 (default) 
224 bps 15
256 bps 16
320 bps 17
*/
	int bps = atoi(nBps.c_str());
	int res = 0;
	
	if (bps <= 7){

		res = 8 + 8 * (bps);
	}
	else if ((bps > 7) && (bps <= 13)){

		res = 80 + 16 * (bps - 8);
	}
	else if ((bps > 13) && (bps <= 16)){

		res = 160 + 32 * (bps - 13);
	}
	else{
		res = 320;
	}

	std::string sRes;

	CString tmp;
	tmp.Format("%d", res);
	sRes = tmp.GetBuffer(10);
	tmp.ReleaseBuffer();

	return sRes;
}

std::string cfgFile::findItem(CString itemPrefix)
{
	
	CString cResult = "";

	for(int i = 0; i < cfg.GetSize(); i++){
		
		if(cfg[i].Left(cfg[i].Find("=")) == itemPrefix){

			cResult = cfg[i].Right(cfg[i].GetLength() - cfg[i].Find("=") -1);
			break;
		}
	}
	
	std::string val;

	if(cResult.IsEmpty()){

		val="-1";
	}
	else{

		val = cResult.GetBuffer(10);
	}
	cResult.ReleaseBuffer();

	return val;
}

BOOL cfgFile::writeToDisk()
{

	BOOL bResult = TRUE;

	CStdioFile file;
	CFileException e;

	TRY{
		
		
		file.Open(wd + "\\lameFE.ini", CFile::modeWrite | CFile::modeCreate | CFile::typeText, &e);
		file.WriteString("# lameFE.ini | lameFE configuration file\n");
		file.WriteString("# Do not edit manually unless you are knowing what you are doing!\n");
		file.WriteString("\n[setup]\n");

		for(int i = 0; i < cfg.GetSize(); i++){

			file.WriteString(cfg[i] + "\n");
		}

		file.Flush();
		file.Close();
	}

	CATCH(CFileException, e){

		bResult = FALSE; //We failed
	}
	END_CATCH;

	return bResult;
}



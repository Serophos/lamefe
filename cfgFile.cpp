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
	wd = dir;
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
			/*else if((tmp.Left(6) == "output") || (tmp.Left(6) == "player") || (tmp.Left(4) == "beep") ||
	 			(tmp.Left(6) == "dialog") || (tmp.Left(4) == "play") || (tmp.Left(3) == "m3u")){
				cfg.Add(tmp);
			}*/
			else{
			//	Presets.Add(tmp);
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

//	delete val;
}

BOOL cfgFile::SetValue(int item, LPVOID value)
{
	CString result;

	switch(item){
	
	case BITRATE:
		result.Format("bitrate=%d", (int)value);
		break;
	case NUMCHNLS:
		result.Format("channels=%d", (int)value);
		break;
	case COPYRIGHT:
		result.Format("copyright=%d", (int)value);
		break;
	case ORIGINAL:
		result.Format("original=%d", (int)value);
		break;
	case PRIVATE:
		result.Format("private=%d", (int)value);
		break;
	case CRC:
		result.Format("crc=%d", (int)value);
		break;
	case ID3V2:
		result.Format("id3v2=%d", (int)value);
		break;
	case RENAME:
		result.Format("rename=%d", (int)value);
		break;
	case FORMATSTR:
		result.Format("formatstr=%s", (const char*)value);
		break;

	case OUTPUT:
		result.Format("output=%s", (const char*)value);
		break;
	case PLAYER:
		result.Format("extplayer=%s", (const char*)value);
		break;
	case BEEP:
		result.Format("beep=%d", (int)value);
		break;
	case PLAY:
		result.Format("play=%d", (int)value);
		break;
	case M3U:
		result.Format("m3u=%d", (int)value);
		break;
	case QUIT:
		result.Format("quit=%d", (int)value);
		break;
	case DIALOG:
		result.Format("dialog=%d", (int)value);
		break;
	case SHUTDOWN:
		result.Format("shutdown=%d", (int)value);
		break;
	case PLUGINDIR:
		result.Format("plugindir=%s", (char)value);
		break;
	case SILENT:
		result.Format("silent=%d", (int)value);
		break;

	case CDDBSERVER:
		result.Format("cddbserver=%d", (int)value);
		break;
	case PROXY:
		result.Format("useproxy=%d", (int)value);
		break;
	case PROXYADDR:
		result.Format("proxyaddress=%s", (const char*)value);
		break;
	case PROXYPORT:
		result.Format("proxyport=%d", (int)value);
		break;
	case USERNAME:
		result.Format("username=%s", (const char*)value);
		break;
	case PASSWORD:
		result.Format("passwd=%s", (const char*)value);
		break;
	case EMAIL:
		result.Format("email=%s", (const char*)value);
		break;
	case AUTH:
		result.Format("authentication=%d", (int)value);
		break;
	case TIMEOUT:
		result.Format("timeout=%d", (int)value);
		break;
	case EJECT:
		result.Format("eject=%d", (int)value);
		break;
	case LOCK:
		result.Format("lock=%d", (int)value);
		break;
	case SWAPCHNLS:
		result.Format("swapchannels=%d", (int)value);
		break;
	case SELECT:
		result.Format("selectall=%d", (int)value);
			break;
	default:
		OutputDebugString("FATAL CFGFILE ERROR: ITEM NOT DEFINED\n");

	}
	
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

int cfgFile::GetValue(int item, BOOL formated)
{
	std::string result;

	switch(item){

	case BITRATE:
		result = (formated ? FormatBps(findItem("bitrate")) : findItem("bitrate"));
		break;
	case NUMCHNLS:
		result = findItem("channels");
		break;
	case COPYRIGHT:
		result = findItem("copyright");
		break;
	case ORIGINAL:
		result = findItem("original");
		break;
	case PRIVATE:
		result = findItem("private");
		break;
	case CRC:
		result = findItem("crc");
		break;
	case ID3V2:
		result = findItem("id3v2");
		break;
	case RENAME:
		result = findItem("rename");
		break;

	case BEEP:
		result = findItem("beep");
		break;
	case PLAY:
		result = findItem("play");
		break;
	case M3U:
		result = findItem("m3u");
		break;
	case QUIT:
		result = findItem("quit");
		break;
	case DIALOG:
		result = findItem("dialog");
		break;
	case SHUTDOWN:
		result = findItem("shutdown");
		break;
	case SILENT:
		result = findItem("silent");
		break;

	case CDDBSERVER:
		result = findItem("cddbserver");
		break;
	case PROXY:
		result = findItem("useproxy");
		break;
	case PROXYPORT:
		result = findItem("proxyport");
		break;
	case AUTH:
		result = findItem("authentication");
		break;
	case TIMEOUT:
		result = findItem("timeout");
		break;
	case EJECT:
		result = findItem("eject");
		break;
	case LOCK:
		result = findItem("lock");
		break;
	case SWAPCHNLS:
		result = findItem("swapchannels");
		break;
	case SELECT:
		result = findItem("selectall");
		break;
	default:
		OutputDebugString("\nFATAL CFGFILE ERROR: ITEM NOT DEFINED\n");
	}

	return atoi(result.c_str());
}

CString cfgFile::GetStringValue(int item)
{

	CString result;

	switch(item){

	case PROXYADDR:
		result = findItem("proxyaddress").c_str();
		break;
	case USERNAME:
		result = findItem("username").c_str();
		break;
	case PASSWORD:
		result = findItem("passwd").c_str();
		break;
	case EMAIL:
		result = findItem("email").c_str();
		break;
	case OUTPUT:
		result = findItem("output").c_str();
		break;
	case PLAYER:
		result = findItem("extplayer").c_str();
		break;
	case FORMATSTR:
		result = findItem("formatstr").c_str();
		break;
	case PLUGINDIR:
		result = findItem("plugindir").c_str();
		break;
	default: 
		OutputDebugString("FATAL CFGFILE ERROR: ITEM NOT DEFINED");
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
	

	std::string val = cResult.GetBuffer(10);
	cResult.ReleaseBuffer();

	return val;
}

BOOL cfgFile::writeToDisk()
{

	BOOL bResult = true;

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

		bResult = false; //We failed
	}
	END_CATCH;

	return bResult;
}


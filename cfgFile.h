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

#if !defined(AFX_CFGFILE_H__32EE79AC_6A1F_4EBF_90A0_2F17B64206FA__INCLUDED_)
#define AFX_CFGFILE_H__32EE79AC_6A1F_4EBF_90A0_2F17B64206FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#define BITRATE		0
#define NUMCHNLS	2
#define COPYRIGHT	3
#define ORIGINAL	4
#define PRIVATE		5
#define CRC			6
#define ID3V2		7
#define FORMATSTR   9

#define OUTPUT		10
#define PLAYER		11
#define BEEP		12
#define DIALOG		13
#define PLAY		14
#define	M3U			15
#define QUIT		16
#define SHUTDOWN    17
#define PLUGINDIR   18
#define SILENT      19

#define CDDBSERVER  20
#define PROXY		21
#define PROXYADDR	22
#define PROXYPORT	23
#define USERNAME	24
#define PASSWORD	25
#define EMAIL       26
#define AUTH		27
#define TIMEOUT		28

#define EJECT       30
#define SWAPCHNL	31
#define LOCK		32
#define SWAPCHNLS	33
#define RENAME		34
#define SELECT		35

#define		MODE_STEREO		0
#define		MODE_JSTEREO		1
#define		MODE_DUALCHANNEL	2
#define		MODE_MONO		3

struct preset{
	CString name;
	int bitrate;
	int samples;
	int mode;
	int smode;
	BOOL copy;
	BOOL original;
};		

class cfgFile  
{
public:

	BOOL    SetValue(int item, LPVOID value);
	
	int     GetValue(int item, BOOL formated = FALSE);
	CString GetStringValue(int item);

	cfgFile(CString dir);
	virtual ~cfgFile();
	
	CStringArray cfg;

private:

	BOOL		writeToDisk();
	std::string findItem(CString itemPrefix);
	std::string	FormatBps(std::string nBps);
	CString wd;
};

#endif // !defined(AFX_CFGFILE_H__32EE79AC_6A1F_4EBF_90A0_2F17B64206FA__INCLUDED_)

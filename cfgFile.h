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

#define		MODE_STEREO		0
#define		MODE_JSTEREO		1
#define		MODE_DUALCHANNEL	2
#define		MODE_MONO		3


class cfgFile  
{
public:
	BOOL	SetStringValue(CString item, CString value);
	BOOL    SetValue(CString item, int value);
	
	int     GetValue(LPCTSTR item, BOOL formated = FALSE);
	CString GetStringValue(CString item);

	cfgFile(CString dir = "");
	virtual ~cfgFile();
	
	CStringArray cfg;

private:

	BOOL		writeToDisk();
	std::string findItem(CString itemPrefix);
	std::string	FormatBps(std::string nBps);
	CString wd;
};

#endif // !defined(AFX_CFGFILE_H__32EE79AC_6A1F_4EBF_90A0_2F17B64206FA__INCLUDED_)
